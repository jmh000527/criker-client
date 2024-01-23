#include "EmojiLabelItem.h"

#include <QMovie>

EmojiLabelItem::EmojiLabelItem(QWidget* parent)
	: QClickLabel{ parent } {
	initControl();

	connect(this, &QClickLabel::clicked,
	        [this]() {
		        emit emojiClicked(this->text());
	        }
	);
}

EmojiLabelItem::~EmojiLabelItem() {}

void EmojiLabelItem::setEmojiCodePoint(QString unicodeEmoji) {
	// 设置gif emoji
	// m_emojiName = unicodeEmoji;
	// QString imageName = QString{ ":/Resources/MainWindow/emotion/%1.png" }.arg(unicodeEmoji);
	// m_apngMovie = new QMovie{ imageName, "apng", this };
	// m_apngMovie->start();
	// m_apngMovie->stop();
	//
	// setMovie(m_apngMovie);

	// 设置文本为Unicode Emoji
	// 创建一个QFont对象并设置字体大小
	QFont font;
	font.setPointSize(14);  // 设置字体大小为16
	setFont(font);

	setText(unicodeEmoji);
	
}

void EmojiLabelItem::initControl() {
	setAlignment(Qt::AlignCenter);
	setObjectName("emojiLabelItem");
	setFixedSize(QSize{ 32, 32 });

}
