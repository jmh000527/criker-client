#pragma once

#include <QtWidgets/QWidget>

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class CCMainWindow : public BasicWindow {
	Q_OBJECT

public:
	CCMainWindow(QWidget* parent = nullptr);
	~CCMainWindow() override;

private:
	Ui::CCMainWindowClass ui{};

	void initTimer();
	void initColtrol();
	void setUserName(const QString& username) const;
	void setUserLevelPixmap(int level) const;
	void setUserHeadPixmap(const QString& headPath) const;
	void setUserStatusMenuIcon(const QString& statusPath) const;

	QWidget* createOtherAppExtension(const QString& appPath, const QString& appName);

	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
	void updateSearchStyle();

private slots:
	void onAppIconClicked() const;

};
