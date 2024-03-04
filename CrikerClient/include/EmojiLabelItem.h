#pragma once

#include "QClickLabel.h"

class EmojiLabelItem : public QClickLabel {
	Q_OBJECT

public:
	EmojiLabelItem(QWidget* parent);
	~EmojiLabelItem() override;

	void setEmojiCodePoint(QString unicodeEmoji);

signals:
	void emojiClicked(QString unicodeEmoji);

private:
	void initControl();

	int m_emojiName;
	QMovie* m_apngMovie;
};
