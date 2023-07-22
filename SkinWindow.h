#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "ui_SkinWindow.h"

class SkinWindow : public BasicWindow {
	Q_OBJECT

public:
	explicit SkinWindow(QWidget* parent = nullptr);
	~SkinWindow() override;

	void initControl();

public slots:
	void onShowClose();
	void onSHowMin();

private:
	Ui::SkinWindowClass ui{};
};