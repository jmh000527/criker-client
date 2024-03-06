#pragma once

#include <QDialog>

#include "BasicWindow.h"
#include "QClickLabel.h"
#include "ui_UserRegister.h"

class UserRegister : public BasicWindow {
	Q_OBJECT

public:
	void initControl();
	// void initTitleBar();
	void loadStyleSheet(const QString& sheetName) override;

	UserRegister(QWidget* parent = nullptr);
	~UserRegister() override;

private:
	Ui::UserRegisterClass ui;

	QClickLabel* m_headLabel{};
	QImage m_headImage{};

public slots:
	void onReceiveData(QImage& headImage);//主窗口接收子窗口发来的数据

private slots:
	void onHeadLabelClicked();
	void onRegister2ButtonClicked();
};
