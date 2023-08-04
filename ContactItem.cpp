#include "ContactItem.h"

ContactItem::ContactItem(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);

	initControl();
}

ContactItem::~ContactItem() {}

void ContactItem::setUsername(const QString& username) {
	ui.username->setText(username);
}

void ContactItem::setSignature(const QString& signature) {
	ui.signature->setText(signature);
}

void ContactItem::setHeadPixmap(const QPixmap& pixmap) {
	ui.label->setPixmap(QPixmap{ pixmap });
}

QString ContactItem::getUsername() const {
	return ui.username->text();
}

QSize ContactItem::getHeadLabelSize() const {
	return ui.label->size();
}

void ContactItem::initControl() {
	
}
