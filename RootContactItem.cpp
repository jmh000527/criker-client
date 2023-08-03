#include "RootContactItem.h"

#include <QPainter>

RootContactItem::RootContactItem(bool showIndicator, QWidget* parent)
	: QLabel(parent), m_rotation{ 0 }, m_showIndicator{ showIndicator } {
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//初始化属性动画
	m_animation = new QPropertyAnimation{ this, "rotation" };
	m_animation->setDuration(50);
	m_animation->setEasingCurve(QEasingCurve::InQuad);
}

RootContactItem::~RootContactItem() {}

void RootContactItem::setText(const QString& title) {
	m_titleText = title;

	//触发界面重绘
	update();
}

void RootContactItem::setExpanded(bool expand) {
	if (expand) {
		m_animation->setEndValue(90);
	} else {
		m_animation->setEndValue(0);
	}

	m_animation->start();
}

int RootContactItem::rotation() {
	return m_rotation;
}

void RootContactItem::setRotation(int rotation) {
	m_rotation = rotation;
}

void RootContactItem::paintEvent(QPaintEvent* event) {
	QPainter painter{ this };
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	QFont font;
	font.setPointSize(10);

	painter.setFont(font);
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.save();

	if (m_showIndicator) {
		QPixmap pixmap{ ":/Resources/MainWindow/arrow.png" };

		QPixmap tmpPixmap{ pixmap.size() };
		tmpPixmap.fill(Qt::transparent);

		QPainter painter2{ &tmpPixmap };
		painter2.setRenderHint(QPainter::SmoothPixmapTransform, true);
		painter2.translate(pixmap.width() / static_cast<qreal>(2), pixmap.height() / static_cast<qreal>(2));
		painter2.rotate(m_rotation);
		painter2.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);

		painter.restore();
	}

	return QLabel::paintEvent(event);
}
