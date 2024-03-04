#include "CutRectangle.h"

CutRectangle::CutRectangle(QWidget* parent /*= nullptr*/) : CutShape(parent) {
	this->m_Type = Rect;
}

CutRectangle::~CutRectangle() {}

bool CutRectangle::paintInit(QPaintEvent* event, QPaintDevice* device) {
	return true;
}

QPainterPath CutRectangle::Region() {
	QPainterPath path;
	path.addRect(QRect(border, border, width() - border * 2, width() - border * 2));
	return path;
}
