#include "ChatWindowShell.h"

#include "CommonUtils.h"

ChatWindowShell::ChatWindowShell(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

ChatWindowShell::~ChatWindowShell() {}

void ChatWindowShell::addTalkWindow(ChatWindow* chatWindow, ChatWindowItem* chatWindowItem, const QString& windowName) {
	ui.rightStackedWidget->addWidget(chatWindow);

	connect(m_emojiWindow, SIGNAL(signalEmojiWindowHide()), chatWindow, SLOT(onSetEmojiBtnFocusStatus()));

	QListWidgetItem* item{ new QListWidgetItem{ui.listWidget} };
	m_chatWindowItemMap.insert(item, chatWindow);

	item->setSelected(true);
}

void ChatWindowShell::setCurrentWidget(QWidget* widget) const {
	ui.rightStackedWidget->setCurrentWidget(widget);
}

void ChatWindowShell::onEmojiBtnClicked() const {
	m_emojiWindow->setVisible(!m_emojiWindow->isVisible());
	QPoint emojiPoint = this->mapToGlobal(QPoint{ 0, 0 });	//将当前控件的相对位置转换为屏幕的绝对位置

	emojiPoint.setX(emojiPoint.x() + 170);
	emojiPoint.setY(emojiPoint.y() + 220);

	m_emojiWindow->move(emojiPoint);
}

void ChatWindowShell::onChatWindowItemClicked(QListWidgetItem* item) {
	QWidget* chatWindowWidget{ m_chatWindowItemMap.find(item).value() };
	ui.rightStackedWidget->setCurrentWidget(chatWindowWidget);
}

void ChatWindowShell::onEmojiItemClicked(int emojiNum) {
	ChatWindow* currentChatWindow{ dynamic_cast<ChatWindow*>(ui.rightStackedWidget->currentWidget()) };
	if(currentChatWindow != nullptr) {
		currentChatWindow->addEmojiImage(emojiNum);
	}
}

void ChatWindowShell::initControl() {
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString{ "聊天窗口" });

	m_emojiWindow = new EmojiWindow;
	m_emojiWindow->hide();

	QList<int> leftWidgetSize;
	leftWidgetSize.append(154);
	leftWidgetSize.append(width() - 154);
	ui.splitter->setSizes(leftWidgetSize);

	ui.listWidget->setStyle(new CustomProxyStyle{ this });

	connect(ui.listWidget, &QListWidget::itemClicked, this, &ChatWindowShell::onChatWindowItemClicked);
	connect(m_emojiWindow, SIGNAL(signalEmojiItemClicked(int)), this, SLOT(onEmojiItemClicked(int)));
}
