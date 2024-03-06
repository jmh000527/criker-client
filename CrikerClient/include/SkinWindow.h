#pragma once

#include <QWidget>

#include "BasicWindow.h"
#include "ui_SkinWindow.h"

class SkinWindow : public BasicWindow {
	Q_OBJECT

public:
	explicit SkinWindow(QWidget* parent = nullptr);
	~SkinWindow() override;

	void initControl();

private:
	Ui::SkinWindowClass ui{};
};
