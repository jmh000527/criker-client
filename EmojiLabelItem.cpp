#include "EmojiLabelItem.h"

#include <QMovie>

EmojiLabelItem::EmojiLabelItem(QWidget* parent)
	: QClickLabel{ parent } {
	initControl();

	connect(this, &QClickLabel::clicked,
	        [this]() {
		        emit emojiClicked(this->m_emojiName);
	        }
	);
}

EmojiLabelItem::~EmojiLabelItem() {}

void EmojiLabelItem::setEmojiName(int emojiName) {
	m_emojiName = emojiName;
	QString imageName = QString{ ":/Resources/MainWindow/emotion/%1.png" }.arg(emojiName);
	m_apngMovie = new QMovie{ imageName, "apng", this };
	m_apngMovie->start();
	m_apngMovie->stop();

	setMovie(m_apngMovie);
}

void EmojiLabelItem::initControl() {
	setAlignment(Qt::AlignCenter);
	setObjectName("emojiLabelItem");
	setFixedSize(QSize{ 32, 32 });

}
