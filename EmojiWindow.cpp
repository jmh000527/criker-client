#include "EmojiWindow.h"

#include <QPainter>
#include <QStyleOption>
#include <QQueue>

#include "CommonUtils.h"
#include "EmojiLabelItem.h"

enum class EmojiParam {
	Column = 14,
	Row = 30
};

EmojiWindow::EmojiWindow(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);

	initControl();
}

EmojiWindow::~EmojiWindow() {}

void EmojiWindow::initControl() {
	CommonUtils::loadStyleSheet(this, "EmotionWindow");
	// 定义Unicode Emoji范围
	char32_t startCodePoint = 0x1F600;  // 起始码点
	char32_t endCodePoint = 0x1F64F;    // 结束码点

	char32_t startCodePoint2 = 0x1F680;  // 起始码点
	char32_t endCodePoint2 = 0x1F6FF;    // 结束码点

	char32_t startCodePoint3 = 0x1F300;  // 起始码点
	char32_t endCodePoint3 = 0x1F5FF;    // 结束码点

	QQueue<QString> emojiQueue;

	// 循环生成emoji unicode
	for (char32_t codePoint = startCodePoint; codePoint <= endCodePoint; ++codePoint) {
		char32_t t[] = { codePoint, 0x0 };
		emojiQueue.enqueue(QString::fromUcs4(t));
	}
	for (char32_t codePoint = startCodePoint2; codePoint <= endCodePoint2; ++codePoint) {
		char32_t t[] = { codePoint, 0x0 };
		emojiQueue.enqueue(QString::fromUcs4(t));
	}
	for (char32_t codePoint = startCodePoint3; codePoint <= endCodePoint3; ++codePoint) {
		char32_t t[] = { codePoint, 0x0 };
		emojiQueue.enqueue(QString::fromUcs4(t));
	}


	for(int row{}; row < static_cast<int>(EmojiParam::Row); ++row) {
		for(int column{}; column < static_cast<int>(EmojiParam::Column); ++column) {
			if(emojiQueue.isEmpty()) {
				break;
			}

			EmojiLabelItem* emojiLabelItem{ new EmojiLabelItem{this} };
			// emojiLabelItem->setEmojiName(row * static_cast<int>(EmojiParam::Column) + column);
			emojiLabelItem->setEmojiCodePoint(emojiQueue.dequeue());

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

void EmojiWindow::addEmoji(QString unicodeEmoji) {
	hide();
	emit signalEmojiWindowHide();
	emit signalEmojiItemClicked(unicodeEmoji);
}
