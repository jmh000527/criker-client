#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow {
	Q_OBJECT

public:
	UserLogin(QWidget* parent = nullptr);
	~UserLogin() override;

private:
	Ui::UserLoginClass ui{};

	void initControl();
	bool verifyAccountCode() const;

	void loadStyleSheet(const QString& sheetName) override;

private slots:
	void onLoginButtonClicked();
	void onRegisterButtonClicked();
};
