#include "BackgroundWidget.h"

#include <QPainter>

#include "CutRectangle.h"
#include "CutRound.h"

BackgroundWidget::BackgroundWidget(QWidget* parent /*= nullptr*/, ShapeType type) : QWidget(parent) {
	m_Type = type;
	if (parentWidget()) {
		setGeometry(0, 0, parentWidget()->width() - 350, parentWidget()->height() - 10);
	}

	PictureLoadFinished();
}

BackgroundWidget::~BackgroundWidget() {}

void BackgroundWidget::PictureLoadFinished() {
	if (m_CutShape == nullptr) {
		if (m_Type == Round) {
			m_CutShape = new CutRound(this);
		} else {
			m_CutShape = new CutRectangle(this);
		}
		// m_CutShape->move(rect().center());
		m_CutShape->move(QPoint{ 130, 0 });
		m_CutShape->show();
	}
}

QRectF BackgroundWidget::getRectToCut() const {
	return m_CutShape->CutRegion().translated(m_CutShape->pos()).boundingRect();
}

void BackgroundWidget::paintEvent(QPaintEvent*) {
	QPainterPath painterPath;
	QPainterPath p;
	p.addRect(x(), y(), rect().width(), rect().height());
	if (m_CutShape) {
		painterPath.addPath(m_CutShape->CutRegion().translated(m_CutShape->pos()));
	}

	// QRectF boundingRect = painterPath.boundingRect();
	// qDebug() << "Bounding Rect:" << boundingRect;

	QPainterPath drawPath = p.subtracted(painterPath);

	QPainter paint(this);
	paint.setOpacity(0.8);
	paint.fillPath(drawPath, QBrush(Qt::black));
}
