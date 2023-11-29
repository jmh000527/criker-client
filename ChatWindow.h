#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "ui_ChatWindow.h"

enum class GroupType;

class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	ChatWindow(QWidget* parent, const QString& uid, GroupType groupType);
	~ChatWindow();

	void addEmojiImage(int emojiNum);
	void setWindowName(const QString& name);

private:
	void addPeopInfo(QTreeWidgetItem* pRootItem);
	void initCompanyTalk();
	void initPersonalTalk();
	void initMarketingTalk();
	void initDevelopmentTalk();
	void initP2PChat();
	void initControl();

	void loadStyleSheet(const QString& sheetName);

	Ui::ChatWindow ui;
	QString m_chatId;
	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	//所有分组联系人姓名

private slots:
	void onSendBtnClicked();
	void onItemDoubleClicked(QTreeWidgetItem* item, int colum);
};
