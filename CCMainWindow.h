#pragma once

#include <QtWidgets/QWidget>

#include "basicwindow.h"
#include "Group.h"
#include "SearchResult.h"
#include "ui_CCMainWindow.h"
#include "User.h"

class CCMainWindow : public BasicWindow {
	Q_OBJECT

public:
	CCMainWindow(QWidget* parent = nullptr);
	~CCMainWindow() override;

	void setLineEditText(QString text) const; //设置搜索文本框内容

private:
	Ui::CCMainWindowClass ui{};

	void loadStyleSheet(const QString& sheetName) override;

	void initTimer();
	void initColtrol();
	void initContactTree();
	void initShowResult();

	static void pushToSystem(const QString msg, const QString time, const QString senderId);

	void setUserName(const QString& username) const;
	// void setUserLevelPixmap(int level) const;
	void setUserHeadPixmap() const;
	void setUserStatusMenuIcon(const QString& statusPath) const;

	QWidget* createOtherAppExtension(const QString& appPath, const QString& appName);

	void resizeEvent(QResizeEvent* event) override;
	void showEvent(QShowEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

	void updateSearchStyle() const;
	void addDepartment(QTreeWidgetItem* pRootGroupItem, const int DepID) const;
	void addFriends(QTreeWidgetItem* pRootGroupItem, const User& user);
	void addGroups(QTreeWidgetItem* pRootGroupItem, const Group& group);
	void addUserMessage(const User& user, const QString& msg, const QString& time);
	void addGroupMessage(const Group& group, const QString& msg, const QString& time);

	SearchResult* m_pSearchResult{};
	QStringList strs;                   //初始化的信息   本程序为构造函数内 静态信息 后续可以在数据库动态获取

private slots:
	void onAppIconClicked() const;

	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

	void onSearchLineEditTextChanged(const QString& arg1) const;      //编辑时触发

public slots:
	void onAddMessage(const QString msg, const QString time, const QString senderId);
};
