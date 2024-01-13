#pragma once

#include <QPainterPath>
#include <QWidget>
#include <QLabel>

enum ShapeType {
	Rect,
	//矩形
	Round,
	//圆形
};

//剪贴图基类 实现了基本的交互功能，并绘制了部分图案，主要的团绘制在子类实现，通过实现paintInit接口
class CutShape : public QWidget {
public:
	CutShape(QWidget* parent = nullptr);
	~CutShape() override = default;

public:
	QPainterPath CutRegion();

protected:
	//QWidget
	virtual void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
	virtual void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;
	virtual void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

	virtual bool paintInit(QPaintEvent*, QPaintDevice*) = 0;

	virtual QPainterPath Region();

	ShapeType m_Type;
	bool m_MouseDown = false;
	bool m_IsMoving = false;
	bool m_IsFirst = true;
	int border = 2;

private:
	QRect getResizeGem(QRect oldgeo, QPoint mousePoint, bool& ignore) const;

	bool m_Left = false;
	bool m_Right = false;
	bool m_Bottom = false;
	bool m_Top = false;
	QPoint m_startPoint;
	QPoint m_old_pos;
	QLabel* m_Label;
	QPixmap m_BufferPix;
};
