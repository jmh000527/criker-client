#include "CutRound.h"

#include <QPainter>

CutRound::CutRound(QWidget* parent /*= nullptr*/) : CutShape(parent) {
	this->m_Type = Round;
}

CutRound::~CutRound() = default;

bool CutRound::paintInit(QPaintEvent*, QPaintDevice* device) {
	if (m_MouseDown && m_IsMoving == false || m_IsFirst) {
		m_IsFirst = false;

		QPainter paint(device);
		paint.drawEllipse(border, border, width() - border * 2, width() - border * 2);

		return true;
	}

	return false;
}

QPainterPath CutRound::Region() {
	QPainterPath path;
	path.addEllipse(QRect(border, border, width() - border * 2, width() - border * 2));

	return path;
}
