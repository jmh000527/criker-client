﻿#pragma once

#include <QWidget>
#include <QListWidgetItem>

#include "basicwindow.h"
#include "QNChatMessage.h"
#include "ui_ChatWindow.h"

enum class GroupType;

class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	ChatWindow(QWidget* parent, const QString& uid);
	~ChatWindow();

	void addEmojiImage(int emojiNum);
	void setWindowName(const QString& name);

private:
	void addPeopInfo(QTreeWidgetItem* pRootItem, int employeeID);

	void initP2PChat();		//初始化单聊
	int getCompDepID();
	void initTalkWindow();	//初始化群聊

	void initControl();
	void initGroupChatStatus();
	void loadStyleSheet(const QString& sheetName);

	void dealMessage(QNChatMessage* messageW, QListWidgetItem* item, QString text, QString time, QNChatMessage::User_Type type);
	void dealMessageTime(QString curMsgTime);

	Ui::ChatWindow ui;
	QString m_chatId;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	//所有分组联系人姓名
	bool m_isGroupChat;

private slots:
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int colum);
};