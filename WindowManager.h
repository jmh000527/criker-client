#pragma once

#include <QObject>

#include "ChatWindowShell.h"


class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();
	static WindowManager* getInstance();

	QWidget* findWindowByName(const QString& windowName);
	void deleteWindowByName(const QString& windowName);
	void addWindowName(QWidget* windowWidgetconst, const QString& windowName);
	void addNewChatWindow(const QString& uid);

	ChatWindowShell* getChatWindowShell();


private:
	ChatWindowShell* m_chatWindowShell;
	QMap<QString, QWidget*> m_windowMap;
};
