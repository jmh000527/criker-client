#include "CutShape.h"

#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>

CutShape::CutShape(QWidget* parent /*= nullptr*/) : QWidget(parent) {
	setMinimumSize(100, 100);

	setWindowFlags(Qt::FramelessWindowHint);

	setMouseTracking(true);
	setGeometry(0, 0, 539, 539);

	m_Label = new QLabel(this);

	m_Label->setMouseTracking(true);

	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->addWidget(m_Label);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
}

QPainterPath CutShape::CutRegion() {
	return Region();
}

void CutShape::mousePressEvent(QMouseEvent* event) {
	m_startPoint = event->pos();
	m_MouseDown = event->button() == Qt::LeftButton;
}

void CutShape::mouseMoveEvent(QMouseEvent* event) {
	QPoint dragPoint = event->pos();
	if (!parentWidget()->rect().contains(mapToParent(dragPoint))) {
		return;
	}
	int x = event->x();
	int y = event->y();
	// qDebug() << x << y << dragPoint;
	if (m_MouseDown) {
		if (m_Left == false && m_Right == false
			&& m_Bottom == false && m_Top == false) {
			QPoint p = QPoint((pos().x() + dragPoint.x() - m_startPoint.x()),
			                  (pos().y() + dragPoint.y() - m_startPoint.y()));
			QPoint dragEndge = p;
			dragEndge.setX(dragEndge.x() + rect().width());
			dragEndge.setY(dragEndge.y() + rect().height());
			p.setX(p.x() < 0 ? 0 : p.x());
			p.setX(dragEndge.x() > parentWidget()->width() ? parentWidget()->width() - rect().width() : p.x());
			p.setY(p.y() < 0 ? 0 : p.y());
			p.setY(dragEndge.y() > parentWidget()->height() ? parentWidget()->height() - rect().height() : p.y());
			move(p);
		} else {
			bool ignore = false;
			QRect g = getResizeGem(geometry(), dragPoint, ignore);
			if (parentWidget()->rect().contains(g))
				setGeometry(g);
			if (ignore == false) {
				m_startPoint = QPoint(!m_Right ? m_startPoint.x() : event->x(),
				                      !m_Bottom ? m_startPoint.y() : event->y());
			}
		}
	} else {
		QRect r = rect();
		m_Left = qAbs(x - r.left()) < 5;
		m_Right = qAbs(x - r.right()) < 5;
		m_Bottom = qAbs(y - r.bottom()) < 5;
		m_Top = qAbs(y - r.top()) < 5;
		bool lorr = m_Left | m_Right;
		bool torb = m_Top | m_Bottom;
		if (lorr && torb) {
			if ((m_Left && m_Top) || (m_Right && m_Bottom)) {
				setCursor(Qt::SizeFDiagCursor);
			} else
				setCursor(Qt::SizeBDiagCursor);
		} else if (lorr)
			setCursor(Qt::SizeHorCursor);
		else if (torb)
			setCursor(Qt::SizeVerCursor);
		else {
			setCursor(Qt::SizeAllCursor);
			m_Bottom = m_Left = m_Right = m_Top = false;
		}
	}
}

void CutShape::mouseReleaseEvent(QMouseEvent* event) {
	m_MouseDown = false;
}

void CutShape::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
}

void CutShape::paintEvent(QPaintEvent* event) {
	QPainter paint;
	if (paintInit(event, &m_BufferPix)) {
		m_BufferPix = QPixmap(size());
		m_BufferPix.fill(Qt::transparent);

		paint.begin(&m_BufferPix);
		QPen pen0;
		pen0.setColor(QColor(54, 158, 254, 120));
		pen0.setWidth(2);
		paint.setPen(pen0);
		paint.drawRect(border, border, width() - border * 2, width() - border * 2);

		QPen pen;
		QVector<qreal> dashes;
		qreal space = 3;
		dashes << 5 << space << 5 << space;
		pen.setDashPattern(dashes);
		pen.setColor(Qt::white);

		paint.setPen(pen);
		int x_pos = (int)width() / 3.0;
		int y_pos = (int)height() / 3.0;
		paint.drawLine(x_pos, border, x_pos, height() - 2 * border);
		paint.drawLine(2 * x_pos, border, 2 * x_pos, height() - 2 * border);
		paint.drawLine(border, y_pos, width() - 2 * border, y_pos);
		paint.drawLine(border, 2 * y_pos, width() - 2 * border, 2 * y_pos);
		paint.end();
	}
	paint.begin(this);
	paint.drawPixmap(rect(), m_BufferPix);
	paint.end();
}

QPainterPath CutShape::Region() {
	return {};
}

QRect CutShape::getResizeGem(QRect oldgeo, QPoint mousePoint, bool& ignore) const {
	QRect g = oldgeo;
	bool lorr = m_Left | m_Right;
	bool torb = m_Top | m_Bottom;
	int dx = mousePoint.x() - m_startPoint.x();
	int dy = mousePoint.y() - m_startPoint.y();
	ignore = false;
	if (lorr && torb) {
		int maxLen = qMax(qAbs(dx), qAbs(dy));
		if (m_Left && m_Top && dx * dy > 0) {
			g.setLeft(dx > 0 ? g.left() + maxLen : g.left() - maxLen);
			g.setTop(dy > 0 ? g.top() + maxLen : g.top() - maxLen);
		} else if (m_Right && m_Top && dx * dy < 0) {
			g.setRight(dx > 0 ? g.right() + maxLen : g.right() - maxLen);
			g.setTop(dy > 0 ? g.top() + maxLen : g.top() - maxLen);
		} else if (m_Right && m_Bottom) {
			if (dx * dy > 0) {
				g.setRight(dx > 0 ? g.right() + maxLen : g.right() - maxLen);
				g.setBottom(dy > 0 ? g.bottom() + maxLen : g.bottom() - maxLen);
			} else if (dx == 0 && dy != 0
				/*|| dx != 0 && dy == 0*/) {
				ignore = true;
			}
		} else if (m_Left && m_Bottom && dx * dy < 0) {
			g.setLeft(dx > 0 ? g.left() + maxLen : g.left() - maxLen);
			g.setBottom(dy > 0 ? g.bottom() + maxLen : g.bottom() - maxLen);
		}

		return g;
	} else if (lorr) {
		if (m_Left)
			g.setLeft(g.left() + dx);
		if (m_Right)
			g.setRight(g.right() + dx);
		int len = g.width() - oldgeo.width();
		int intHight = (int)len / 2.0;

		g.setTop(g.top() - intHight);
		g.setBottom(g.bottom() + len - intHight);
	} else if (torb) {
		if (m_Bottom)
			g.setBottom(g.bottom() + dy);
		if (m_Top)
			g.setTop(g.top() + dy);
		int dheigt = g.height() - oldgeo.height();
		int intWidth = (int)dheigt / 2.0;

		g.setLeft(g.left() - intWidth);
		g.setRight(g.right() + dheigt - intWidth);
	} else {
		ignore = true;
	}
	return g;
}
