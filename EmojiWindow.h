#pragma once

#include <QWidget>
#include "ui_EmojiWindow.h"

class EmojiWindow : public QWidget {
	Q_OBJECT

public:
	EmojiWindow(QWidget* parent = nullptr);
	~EmojiWindow();

private:
	Ui::EmojiWindowClass ui;

	void initControl();

	void paintEvent(QPaintEvent* event) override;

signals:
	void signalEmojiWindowHide();
	void siganlEmohiItemClicked(int emojiNum);


private slots:
	void addEmoji(int emojiNum);
};
