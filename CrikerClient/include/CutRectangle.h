#pragma once

#include <QMouseEvent>

#include "CutShape.h"

class CutRectangle : public CutShape {
public:
	CutRectangle(QWidget* parent = nullptr);
	~CutRectangle() override;

protected:
	//CutShape
	virtual bool paintInit(QPaintEvent*, QPaintDevice*) override;
	virtual QPainterPath Region() override;

private:
};
