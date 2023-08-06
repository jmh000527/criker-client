#pragma once

#include <QWidget>

#include "ui_ChatWindow.h"

class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	ChatWindow(QWidget* parent, const QString& windowName);
	~ChatWindow();

	void addEmojiImage(int emojiNum);
	void setWindowName(const QString& name);

private:
	Ui::ChatWindowClass ui;
};
