#include "QMsgTextEdit.h"
#include <QMovie>

QMsgTextEdit::QMsgTextEdit(QWidget* parent)
	: QTextEdit(parent) {}

QMsgTextEdit::~QMsgTextEdit() {
	deleteAllEmojiImage();
}

void QMsgTextEdit::addEmojiUrl(int emojiNum) {
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emojiNum);
	const QString& flagName = QString("%1").arg(imageName);
	insertHtml(QString("<img src='%1'/>").arg(flagName));

	if (m_emojiUrlList.contains(imageName)) {
		return;
	}

	m_emojiUrlList.append(imageName);
	auto* apngMovie = new QMovie(imageName, "apng", this);
	m_emojiMap.insert(apngMovie, flagName);

	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmojiImageFrameChanged(int)));
	apngMovie->start();
	updateGeometry();
}

void QMsgTextEdit::deleteAllEmojiImage() {
	for (auto iter = m_emojiMap.constBegin(); iter != m_emojiMap.constEnd(); iter++) {
		delete iter.key();
	}
	m_emojiMap.clear();
}

void QMsgTextEdit::onEmojiImageFrameChanged(int frame) {
	auto* movie = qobject_cast<QMovie*>(sender());
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emojiMap.value(movie)), movie->currentPixmap());
}
