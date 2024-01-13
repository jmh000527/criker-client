#pragma once

#include <QWidget>

#include "basicwindow.h"
#include "ui_PictureEdit.h"

class PictureEdit : public BasicWindow {
	Q_OBJECT

public:
	PictureEdit(const QString& filePath, QWidget* parent = nullptr);
	~PictureEdit() override;

	QImage getHeadImage() const;

signals:
	void sendData(QImage&);

private slots:
	void on_pushButton_clicked();

private:
	void loadStyleSheet(const QString& sheetName) override;

	Ui::PictureEditClass ui;
	QImage m_headImage{};
};
