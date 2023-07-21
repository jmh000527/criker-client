#pragma once

#include <QtWidgets/QWidget>

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class CCMainWindow : public BasicWindow {
	Q_OBJECT

public:
	CCMainWindow(QWidget* parent = nullptr);
	~CCMainWindow();

private:
	Ui::CCMainWindowClass ui;

	void initColtrol();
	void setUserName(const QString& username);
	void setUserLevel(int level);
	void setHeadPixmap(const QString& headPath);
};
