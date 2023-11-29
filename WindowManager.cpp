#include "WindowManager.h"

#include "ChatWindowShell.h"

//单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstance)

WindowManager::WindowManager()
	: QObject{ nullptr }, m_chatWindowShell{ nullptr } {}

WindowManager::~WindowManager() {}

WindowManager* WindowManager::getInstance() {
	return theInstance;
}

QWidget* WindowManager::findWindowByName(const QString& windowName) {
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

void WindowManager::addNewChatWindow(const QString& windowName, GroupType groupType, const QString& people) {
	if (m_chatWindowShell == nullptr) {
		m_chatWindowShell = new ChatWindowShell;

		connect(m_chatWindowShell, &ChatWindowShell::destroyed,
		        [this](QObject* obj) {
			        m_chatWindowShell = nullptr;
		        }
		);
	}

	QWidget* widget{ findWindowByName(windowName) };
	if (widget == nullptr) {
		ChatWindow* chatWindow{ new ChatWindow{ m_chatWindowShell, windowName, groupType } };
		ChatWindowItem* chatWindowItem{ new ChatWindowItem{ chatWindow } };

		switch (groupType) {
			case GroupType::Company: {
				chatWindow->setWindowName(QStringLiteral("信息科学与技术学院"));
				chatWindowItem->setMsgLabelContent(QStringLiteral("信息科学与技术学院signature"));
				break;
			}
			case GroupType::Personal: {
				chatWindow->setWindowName(QStringLiteral("文学院"));
				chatWindowItem->setMsgLabelContent(QStringLiteral("文学院"));
				break;
			}
			case GroupType::Development: {
				chatWindow->setWindowName(QStringLiteral("外国语学院"));
				chatWindowItem->setMsgLabelContent(QStringLiteral("外国语学院"));
				break;
			}
			case GroupType::Marketing: {
				chatWindow->setWindowName(QStringLiteral("考古学院"));
				chatWindowItem->setMsgLabelContent(QStringLiteral("考古学院"));
				break;
			}
			case GroupType::P2P: {
				chatWindow->setWindowName(QStringLiteral("P2P"));
				chatWindowItem->setMsgLabelContent(people);
				break;
			}
			default: {
				break;
			}
		}

		m_chatWindowShell->addTalkWindow(chatWindow, chatWindowItem, groupType);
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
