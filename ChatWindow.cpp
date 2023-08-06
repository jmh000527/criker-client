#include "ChatWindow.h"

ChatWindow::ChatWindow(QWidget* parent, const QString& windowName)
	:QWidget{ parent } {
	ui.setupUi(this);
}

ChatWindow::~ChatWindow() {}

void ChatWindow::addEmojiImage(int emojiNum) {}

void ChatWindow::setWindowName(const QString& name) {}
