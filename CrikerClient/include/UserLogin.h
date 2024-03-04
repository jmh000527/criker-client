#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "QClickLabel.h"
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

	QClickLabel* m_headLabel{};
	QImage m_headImage{};

public slots:
	void onReceiveData(QImage& headImage);//主窗口接收子窗口发来的数据

private slots:
	void onLoginButtonClicked();
	void onRegisterButtonClicked();
	void onHeadLabelClicked();
	void onRegister2ButtonClicked();
};
