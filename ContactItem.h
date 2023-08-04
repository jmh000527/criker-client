#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget {
	Q_OBJECT

public:
	ContactItem(QWidget* parent = nullptr);
	~ContactItem();

	void setUsername(const QString& username);
	void setSignature(const QString& signature);
	void setHeadPixmap(const QPixmap& pixmap);

	QString getUsername() const;
	QSize getHeadLabelSize() const;

private:
	Ui::ContactItemClass ui;

	void initControl();
};
