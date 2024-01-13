#pragma once

#include <CutShape.h>
#include <QMouseEvent>

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
