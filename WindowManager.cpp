#include <QSqlQueryModel>

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

void WindowManager::addNewChatWindow(const QString& uid) {
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
		ChatWindow* chatWindow{ new ChatWindow{ m_chatWindowShell, uid } };
		ChatWindowItem* chatWindowItem{ new ChatWindowItem{ chatWindow } };

		//判断群聊还是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID=%1").arg(uid);
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();

		//单聊
		if (rows == 0) {
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID=%1").arg(uid);
			sqlDepModel.setQuery(sql);
		}

		QModelIndex indexDepIndex = sqlDepModel.index(0, 0);
		QModelIndex signIndex = sqlDepModel.index(0, 1);
		QString strWindowName = sqlDepModel.data(signIndex).toString();
		QString strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		chatWindow->setWindowName(strWindowName);
		chatWindowItem->setMsgLabelContent(strMsgLabel); //左侧文本显示

		m_chatWindowShell->addTalkWindow(chatWindow, chatWindowItem, uid);
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

ChatWindowShell* WindowManager::getChatWindowShell() {
	return m_chatWindowShell;
}
