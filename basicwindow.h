#pragma once

#include <QDialog>

class BasicWindow  : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	void loadStyleSheet(const QString& sheetName);

	QPixmap getRoundedImage(const QPixmap& src, QPixmap);
};
