#pragma once

#include <QWidget>

#include "CutShape.h"

class BackgroundWidget : public QWidget {
public:
	BackgroundWidget(QWidget* parent = nullptr, ShapeType type = Round);
	~BackgroundWidget();

	void PictureLoadFinished();
	QRectF getRectToCut() const;

protected:
	virtual void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

private:
	ShapeType m_Type;
	CutShape* m_CutShape = nullptr;
};
