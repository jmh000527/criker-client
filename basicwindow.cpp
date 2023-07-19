#include "basicwindow.h"

#include <QFile>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

#include "CCMainWindow.h"

BasicWindow::BasicWindow(QWidget* parent)
	: QDialog{ parent } {
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
}

BasicWindow::~BasicWindow() {}

void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleSheetName = sheetName;
	QFile file{ ":/Resource/QSS/" + sheetName + ".css" };
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		//获取用户当前皮肤的RGB
		const QString r{ QString::number(m_colorBackground.red()) };
		const QString g{ QString::number(m_colorBackground.green()) };
		const QString b{ QString::number(m_colorBackground.blue()) };

		qStyleSheet += QString{
			"QWidget[titleskin=true]\
				{background-color: rgb(%1, %2, %3);\
				border-top-left-radius: 4px;}\
				QWidget[bottomskin=true]\
				{boeder-top: 1px solid rgba(%1, %2, %3, 100);\
				background-color: rgba(%1, %2, %3, 50)；\
				border-bottom-left-radius: 4px;\
				border-bottom-right-radius: 4px;}"
		}.arg(r).arg(g).arg(b);

		setStyleSheet(qStyleSheet);
	}

	file.close();
}

QPixmap BasicWindow::getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize) {
	if(maskSize == QSize{0, 0}) {
		maskSize - mask.size();
	} else {
		mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	QImage resultImage{ maskSize, QImage::Format_ARGB32_Premultiplied };
	QPainter painter{ &resultImage };

	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::initBackgroundColor() {
	QStyleOption opt;
	opt.init(this);
	QPainter p;

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event) {
	initBackgroundColor();

	return QDialog::paintEvent(event);
}

void BasicWindow::initTitleBar(ButtonType buttonType) {
	m_titlebar = new TitleBar{ this };
	m_titlebar->setButtonType(buttonType);
	m_titlebar->move(QPoint{ 0, 0 });

	connect(m_titlebar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titlebar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titlebar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titlebar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon) {
	m_titlebar->setTitleIcon(icon);
	m_titlebar->setTitleContent(title);
}

void BasicWindow::onSignalSkinChanged(const QColor& color) {
	m_colorBackground = color;
	loadStyleSheet(m_styleSheetName);
}
