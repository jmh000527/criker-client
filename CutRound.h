#pragma once

#include <CutShape.h>

class CutRound : public CutShape {
public:
	CutRound(QWidget* parent = nullptr);
	~CutRound();

protected:
	//CutShape
	virtual bool paintInit(QPaintEvent*, QPaintDevice*) Q_DECL_OVERRIDE;
	virtual QPainterPath Region() Q_DECL_OVERRIDE;
};
