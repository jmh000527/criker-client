#pragma once

#include <QWidget>
#include <QListWidgetItem>

#include "basicwindow.h"
#include "Group.h"
#include "QNChatMessage.h"
#include "ui_ChatWindow.h"

enum class GroupType;

class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	ChatWindow(QWidget* parent, const QString& uid, bool isGroupChat);
	~ChatWindow() override;

	void addEmojiImage(int emojiNum) const;
	void setWindowName(const QString& name);

private:
	void addGroupUser(QTreeWidgetItem* pRootItem, const GroupUser& groupUser);

	void initP2PChat();		//初始化单聊
	int getCompDepID();
	void initChatWindow();	//初始化群聊

	void initControl();
	// void initGroupChatStatus();
	void loadStyleSheet(const QString& sheetName);

	void dealMessage(QNChatMessage* messageW, QListWidgetItem* item, const QString& text, const QString& time, const QString& senderId, QNChatMessage::User_Type type);
	void dealMessageTime(const QString& curMsgTime);
	void sendMessage(const QString& msg, const QString& time, const QString& senderId);

	Ui::ChatWindow ui;
	QString m_chatId;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	//所有分组联系人姓名
	bool m_isGroupChat;

private slots:
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int colum);

public slots:
	void onRecieveMessage(const QString msg, const QString time, const QString senderId);
};
