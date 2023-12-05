#pragma once

#include <QWidget>
#include "ui_ChatWindowItem.h"

class ChatWindowItem : public QWidget {
	Q_OBJECT

public:
	ChatWindowItem(QWidget* parent = nullptr);
	~ChatWindowItem();

	void setHeadPixmap(const QPixmap& pixmap) const;
	void setMsgLabelContent(const QString& msg) const;
	QString getMsgLabelText() const;

private:
	Ui::ChatWindowItemClass ui;

	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	void initControl() const;

signals:
	void signalCloseClicked();
};
