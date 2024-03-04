#include "BasicWindow.h"
#include "CommonUtils.h"
#include "NotifyManager.h"

#include <QFile>
#include <QMouseEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QGuiApplication>
#include <QPainterPath>

#include "CCMainWindow.h"

BasicWindow::BasicWindow(QWidget* parent)
	: QDialog{ parent }, m_isMousePressed{ false }, m_colorBackground{ CommonUtils::getDefaultSkinColor() } {
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
	        this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow() = default;

void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleSheetName = sheetName;
	QFile file{ ":/Resources/QSS/" + sheetName + ".css" };
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		//获取用户当前皮肤的RGB
		const QString r{ QString::number(m_colorBackground.red()) };
		const QString g{ QString::number(m_colorBackground.green()) };
		const QString b{ QString::number(m_colorBackground.blue()) };

		const auto increaseValue{ 230 };
		qStyleSheet += QString("QWidget[titleSkin = true] {\
								background-color: rgb(%1, %2, %3);\
								border-top-left-radius: 4px;\
								border-top-right-radius: 4px;\
								border: 1px solid rgb(%1, %2, %3);\
								}\
								QWidget[bottomSkin = true] {\
								background-color: rgb(%4, %5, %6);\
								border-bottom-left-radius: 4px;\
								border-bottom-right-radius: 4px;\
								border: 1px solid rgb(%1, %2, %3);\
								border-top: none;\
								}").arg(r).arg(g).arg(b)
								   .arg(qMin(r.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(g.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(b.toInt() / 10 + increaseValue, 255));

		setStyleSheet(qStyleSheet);
	}

	file.close();
}

QPixmap BasicWindow::getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize) const {
	if (maskSize == QSize{ 0, 0 }) {
		maskSize = mask.size();
	} else {
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	QImage resultImage{ maskSize, QImage::Format_ARGB32_Premultiplied };
	QPainter painter{ &resultImage };

	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);

	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);

	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::initBackgroundColor() {
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p{ this };

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event) {
	initBackgroundColor();

	return QDialog::paintEvent(event);
}

void BasicWindow::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		m_isMousePressed = true;
		m_mousePos = event->globalPos() - pos();
		event->accept();
	}

	return QDialog::mousePressEvent(event);
}

void BasicWindow::mouseMoveEvent(QMouseEvent* event) {
	if (m_isMousePressed && (event->buttons() && Qt::LeftButton)) {
		move(event->globalPos() - m_mousePos);
		event->accept();
	}

	return QDialog::mouseMoveEvent(event);
}

void BasicWindow::mouseReleaseEvent(QMouseEvent* event) {
	m_isMousePressed = false;

	return QDialog::mouseReleaseEvent(event);
}

void BasicWindow::initTitleBar(const ButtonType buttonType) {
	m_titlebar = new TitleBar{ this };
	m_titlebar->setButtonType(buttonType);
	m_titlebar->move(QPoint{ 0, 0 });

	connect(m_titlebar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titlebar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titlebar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titlebar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon) const {
	m_titlebar->setTitleIcon(icon);
	m_titlebar->setTitleContent(title);
}

void BasicWindow::onShowMin(bool) {
	showMinimized();
}

void BasicWindow::onShowHide(bool) {
	hide();
}

void BasicWindow::onShowNormal(bool) {
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool) {
	QApplication::quit();
}

void BasicWindow::onShowClose(bool) {
	close();
}

void BasicWindow::onSignalSkinChanged(const QColor& color) {
	m_colorBackground = color;
	loadStyleSheet(m_styleSheetName);
}

void BasicWindow::onButtonMinClicked() {
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		hide();
	} else {
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked() {
	QPoint windowPos;
	QSize windowSize;

	m_titlebar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect{ windowPos, windowSize });
}

void BasicWindow::onButtonMaxClicked() {
	m_titlebar->saveRestoreInfo(pos(), size());

	const QRect desktopRect{ QApplication::primaryScreen()->availableGeometry() };
	const QRect factRect{
		QRect{ desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6 } };

	setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked() {
	close();
}
