#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin() override;

private:
	Ui::UserLoginClass ui{};

	void initControl();

private slots:
	void onLoginButtonClicked();
};
