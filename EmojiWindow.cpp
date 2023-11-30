#include "EmojiWindow.h"

#include <QPainter>
#include <QStyleOption>

#include "CommonUtils.h"
#include "EmojiLabelItem.h"

enum class EmojiParam {
	Column = 14,
	Row = 12
};

EmojiWindow::EmojiWindow(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);

	// // 设置QWidget的样式
	// this->setStyleSheet("QWidget {"
	// 					 "    border-radius: 4px;"       // 圆角为4px
	// 					 "}");

	initControl();
}

EmojiWindow::~EmojiWindow() {}

void EmojiWindow::initControl() {
	CommonUtils::loadStyleSheet(this, "EmotionWindow");

	for(int row{}; row < static_cast<int>(EmojiParam::Row); ++row) {
		for(int column{}; column < static_cast<int>(EmojiParam::Column); ++column) {
			EmojiLabelItem* emojiLabelItem{ new EmojiLabelItem{this} };
			emojiLabelItem->setEmojiName(row * static_cast<int>(EmojiParam::Column) + column);

			connect(emojiLabelItem, &EmojiLabelItem::emojiClicked, this, &EmojiWindow::addEmoji);
			ui.gridLayout->addWidget(emojiLabelItem, row, column);
		}
	}
}

void EmojiWindow::paintEvent(QPaintEvent* event) {
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p{ this };

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	return QWidget::paintEvent(event);
}

void EmojiWindow::addEmoji(int emojiNum) {
	hide();
	emit signalEmojiWindowHide();
	emit signalEmojiItemClicked(emojiNum);
}
