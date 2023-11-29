#include "ChatWindowItem.h"

#include "CommonUtils.h"

ChatWindowItem::ChatWindowItem(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	CommonUtils::loadStyleSheet(this, "ChatWindowItem");

	initControl();
}

ChatWindowItem::~ChatWindowItem() {}

void ChatWindowItem::setHeadPixmap(const QString& pixmap) const {
	QPixmap mask{ ":/Resources/MainWindow/head_mask.png" };
	QPixmap head = QPixmap{ ":/Resources/MainWindow/girl.png" };
	const QPixmap& headPixmap{ CommonUtils::getRoundedImage(head, mask, ui.headLabel->size()) };

	ui.headLabel->setPixmap(headPixmap);
}

void ChatWindowItem::setMsgLabelContent(const QString& msg) const {
	ui.signatureLabel->setText(msg);
}

QString ChatWindowItem::getMsgLabelText() const {
	return ui.signatureLabel->text();
}

void ChatWindowItem::enterEvent(QEnterEvent* event) {
	ui.closeBtn->setVisible(true);

	return QWidget::enterEvent(event);
}

void ChatWindowItem::leaveEvent(QEvent* event) {
	ui.closeBtn->setVisible(false);

	return QWidget::leaveEvent(event);
}

void ChatWindowItem::resizeEvent(QResizeEvent* event) {
	return QWidget::resizeEvent(event);
}

void ChatWindowItem::initControl() const {
	ui.closeBtn->setVisible(false);

	connect(ui.closeBtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));
}
