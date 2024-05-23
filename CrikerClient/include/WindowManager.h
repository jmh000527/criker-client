#pragma once

#include <mutex>
#include <QObject>

#include "CCMainWindow.h"
#include "ChatWindowShell.h"
#include "MsgType.h"


class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager() override;
	static WindowManager* getInstance();

	QWidget* findWindowByName(const QString& windowName) const;
	void deleteWindowByName(const QString& windowName);
	void addWindowName(QWidget* windowWidgetconst, const QString& windowName);
	void addNewChatWindow(const QString& uid, bool isGroupChat);

	void setMainWindowPointer(CCMainWindow* pMainWindow);
	CCMainWindow* getMainWindowPointer() const;

	ChatWindowShell* getChatWindowShell();

	std::optional<std::tuple<QByteArray, MsgType>> pullMessage();
	void pushMessage(const std::tuple<QByteArray, MsgType>& message);

private:
	ChatWindowShell* m_chatWindowShell;
	QMap<QString, QWidget*> m_windowMap;
	CCMainWindow* m_mainWindow;

	std::mutex m_mutex;
	std::condition_variable m_condition;
	std::list<std::tuple<QByteArray, MsgType>> m_messageQueue;
};
