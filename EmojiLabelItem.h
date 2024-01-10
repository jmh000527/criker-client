#pragma once

#include "QClickLabel.h"

class EmojiLabelItem : public QClickLabel {
	Q_OBJECT

public:
	EmojiLabelItem(QWidget* parent);
	~EmojiLabelItem() override;

	void setEmojiName(int emojiName);

signals:
	void emojiClicked(int emojiNum);

private:
	void initControl();

	int m_emojiName;
	QMovie* m_apngMovie;
};
