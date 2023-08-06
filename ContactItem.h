#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget {
	Q_OBJECT

public:
	ContactItem(QWidget* parent = nullptr);
	~ContactItem();

	void setUsername(const QString& username) const;
	void setSignature(const QString& signature) const;
	void setHeadPixmap(const QPixmap& pixmap) const;

	QString getUsername() const;
	QSize getHeadLabelSize() const;

private:
	Ui::ContactItemClass ui;

	void initControl();
};
