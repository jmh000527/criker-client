﻿#include <QSqlQueryModel>

#include "WindowManager.h"
#include "ChatWindowShell.h"
#include "CommonUtils.h"
#include "UserManager.h"

//单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstance)

WindowManager::WindowManager()
	: QObject{ nullptr }, m_chatWindowShell{ nullptr } {}

WindowManager::~WindowManager() = default;

WindowManager* WindowManager::getInstance() {
	return theInstance;
}

QWidget* WindowManager::findWindowByName(const QString& windowName) const {
	if (m_windowMap.contains(windowName)) {
		return m_windowMap.value(windowName);
	} else {
		return nullptr;
	}
}

void WindowManager::deleteWindowByName(const QString& windowName) {
	m_windowMap.remove(windowName);
}

void WindowManager::addWindowName(QWidget* windowWidget, const QString& windowName) {
	if (!m_windowMap.contains(windowName)) {
		m_windowMap.insert(windowName, windowWidget);
	}
}

void WindowManager::addNewChatWindow(const QString& uid, bool isGroupChat) {
	if(uid.toInt() == UserManager::getCurrentUser().getId()) {
		return;
	}

	if (m_chatWindowShell == nullptr) {
		m_chatWindowShell = new ChatWindowShell;

		connect(m_chatWindowShell, &ChatWindowShell::destroyed,
		        [this](QObject* obj) {
			        m_chatWindowShell = nullptr;
		        }
		);
	}

	QWidget* widget{ findWindowByName(uid) };
	if (widget == nullptr) {
		ChatWindow* chatWindow{ new ChatWindow{ m_chatWindowShell, uid, isGroupChat} };
		ChatWindowItem* chatWindowItem{ new ChatWindowItem{ chatWindow } };
		chatWindow->setWindowName(uid);

		if(!isGroupChat) {
			const auto friendUser{ UserManager::getFriend(uid) };
			chatWindowItem->setMsgLabelContent(friendUser.getName().c_str()); //左侧文本显示
			chatWindowItem->setHeadPixmap(CommonUtils::base64ToQPixmap(friendUser.getHeadImage()));
		}

		m_chatWindowShell->addChatWindow(chatWindow, chatWindowItem, uid, isGroupChat);
	} else {
		//设置右侧聊天窗口
		m_chatWindowShell->setCurrentWidget(widget);

		//设置左侧列表选中
		auto* item{ m_chatWindowShell->getChatWindowItemMap().key(widget) };
		item->setSelected(true);
	}

	m_chatWindowShell->show();
	m_chatWindowShell->activateWindow();
}

void WindowManager::setMainWindowPointer(CCMainWindow* pMainWindow) {
	m_mainWindow = pMainWindow;
}

CCMainWindow* WindowManager::getMainWindowPointer() const {
	return m_mainWindow;
}

ChatWindowShell* WindowManager::getChatWindowShell() {
	return m_chatWindowShell;
}

std::optional<std::tuple<QByteArray, MsgType>> WindowManager::pullMessage() {
	std::unique_lock<std::mutex> uniqueLock{ m_mutex };
	m_condition.wait(uniqueLock, [this]() { return !m_messageQueue.empty(); });
	if (m_messageQueue.empty()) {
		return std::nullopt;
	}
	auto message = m_messageQueue.front();
	m_messageQueue.pop_front();
	return message;
}

void WindowManager::pushMessage(const std::tuple<QByteArray, MsgType>& message) {
	std::lock_guard<std::mutex> lockGuard{ m_mutex };
	m_messageQueue.push_back(message);
	m_condition.notify_one();
}
