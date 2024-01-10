#pragma once

#include <QWidget>
#include <QTcpSocket>

#include "basicwindow.h"
#include "ChatWindowItem.h"
#include "EmojiWindow.h"
#include "ui_ChatWindowShell.h"
#include "ChatWindow.h"

enum class GroupType {
	Company = 0,
	Personal,
	Development,
	Marketing,
	P2P,
};


class ChatWindowShell : public BasicWindow {
	Q_OBJECT

public:
	ChatWindowShell(QWidget* parent = nullptr);
	~ChatWindowShell() override;

	//添加新的聊天窗口
	void addChatWindow(ChatWindow* chatWindow, ChatWindowItem* chatWindowItem, const QString& uid, bool isGroupChat);
	//设置当前聊天窗口
	void setCurrentWidget(QWidget* widget) const;

	QMap<QListWidgetItem*, QWidget*> getChatWindowItemMap()const;

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
	void onEmojiBtnClicked(bool);	//表情按钮被点击

private slots:
	void onChatWindowItemClicked(QListWidgetItem* item);	//左侧列表项被点击
	void onEmojiItemClicked(int emojiNum) const;	//表情被选中

private:
	Ui::ChatWindowShellClass ui;

	void initControl();

	QStringList getEmployeesID();

	QMap<QListWidgetItem*, QWidget*> m_chatWindowItemMap;	//当前打开的聊天窗口的映射
	EmojiWindow* m_emojiWindow;
};
