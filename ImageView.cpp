#include "ImageView.h"

#include <QFile>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>

ImageView::ImageView(QWidget* parent) : QWidget{ parent } { }

ImageView::~ImageView() = default;

void ImageView::loadImage(const QString& filePath) {
	QFile file(filePath);
	if (!file.exists())
		return;

	m_Image.load(filePath);
}

void ImageView::loadImage(const QImage& image) {
	m_Image = image;
	repaint();
}

QImage ImageView::getCurrentScaledImage() const {
	QImage scaledImage = m_Image.scaled(m_picRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	return scaledImage;
}

int ImageView::getXBlank() const {
	return (width() - m_picRect.width()) / 2;
}

int ImageView::getYBlank() const {
	return (height() - m_picRect.height()) / 2;
}

// void ImageView::resizeImage(QSize size) {
//     m_Image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
// }

void ImageView::contextMenuEvent(QContextMenuEvent* event) {
	QPoint pos = event->pos();
	pos = this->mapToGlobal(pos);
	QScopedPointer<QMenu> menu{ new QMenu(this) };

	QScopedPointer<QAction> loadImage{ new QAction(tr("替换其他图像")) };
	QObject::connect(loadImage.data(), &QAction::triggered, this, &ImageView::onLoadImage);
	menu->addAction(loadImage.data());
	menu->addSeparator();

	QAction* zoomInAction = new QAction(tr("放大"));
	QObject::connect(zoomInAction, &QAction::triggered, this, &ImageView::onZoomInImage);
	menu->addAction(zoomInAction);

	QAction* zoomOutAction = new QAction(tr("缩小"));
	QObject::connect(zoomOutAction, &QAction::triggered, this, &ImageView::onZoomOutImage);
	menu->addAction(zoomOutAction);

	QScopedPointer<QAction> presetAction{ new QAction(tr("重置该图像")) };
	QObject::connect(presetAction.data(), &QAction::triggered, this, &ImageView::onPresetImage);
	menu->addAction(presetAction.data());

	menu->exec(pos);
}

void ImageView::paintEvent(QPaintEvent* event) {
	// 绘制样式
	QStyleOption opt;
	opt.initFrom(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	if (m_Image.isNull())
		return QWidget::paintEvent(event);

	// 根据窗口计算应该显示的图片的大小
	int width;
	if (m_Image.width() > this->width()) {
		width = qMin(m_Image.width(), this->width());
	} else {
		width = qMax(m_Image.width(), this->width());
	}
	int height = width * 1.0 / (m_Image.width() * 1.0 / m_Image.height());

	height = qMin(height, this->height());
	width = height * 1.0 * (m_Image.width() * 1.0 / m_Image.height());

	// 平移
	painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);

	// 缩放
	painter.scale(m_ZoomValue, m_ZoomValue);

	// 绘制图像
	m_picRect = QRect{ -width / 2, -height / 2, width, height };
	painter.drawImage(m_picRect, m_Image);
}

void ImageView::wheelEvent(QWheelEvent* event) {
	int value = event->angleDelta().y();
	if (value > 0)
		onZoomInImage();
	else
		onZoomOutImage();

	this->update();
}

void ImageView::mousePressEvent(QMouseEvent* event) {
	// m_OldPos = event->pos();
	// m_Pressed = true;
	QWidget::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent* event) {
	if (!m_Pressed)
		return QWidget::mouseMoveEvent(event);

	//    this->setCursor(Qt::SizeAllCursor);
	//    QPoint pos = event->pos();
	//    int xPtInterval = pos.x() - m_OldPos.x();
	//    int yPtInterval = pos.y() - m_OldPos.y();

	//    m_XPtInterval += xPtInterval;
	//    m_YPtInterval += yPtInterval;

	//    m_OldPos = pos;
	//    this->update();
}

void ImageView::mouseReleaseEvent(QMouseEvent* event) {
	Q_UNUSED(event);
	//    m_Pressed = false;
	//    this->setCursor(Qt::ArrowCursor);
}

void ImageView::onLoadImage(void) {
	QString imageFile = QFileDialog::getOpenFileName(this, "Open Image", "./", tr("Images (*.png *.xpm *.jpg)"));

	QFile file(imageFile);
	if (!file.exists())
		return;

	m_Image.load(imageFile);
}

void ImageView::onZoomInImage(void) {
	m_ZoomValue += 0.1;
	this->update();
}

void ImageView::onZoomOutImage(void) {
	if (m_ZoomValue > 1) {
		m_ZoomValue -= 0.1;
	}
	//    if (m_ZoomValue <= 0)
	//    {
	//        m_ZoomValue += 0.1;
	//        return;
	//    }
	if (m_ZoomValue == 1) {
		emit onPresetImage(); // 图片偏移中心时，再次滑动滚轮重新定位该图像
	}

	this->update();
}

void ImageView::onPresetImage(void) {
	m_ZoomValue = 1.0;
	m_XPtInterval = 0;
	m_YPtInterval = 0;
	this->update();
}
