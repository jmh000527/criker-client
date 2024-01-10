#pragma once

#include <QTextEdit>

class QMsgTextEdit  : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit() override;

	void addEmojiUrl(int emojiNum);
	void deleteAllEmojiImage();

private:
	QList<QString> m_emojiUrlList;
	QMap<QMovie*, QString> m_emojiMap;

private slots:
	void onEmojiImageFrameChanged(int frame);
};
