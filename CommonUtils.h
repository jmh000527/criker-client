#pragma once
#include <QPixmap>
#include <QSize>
#include <QProxyStyle>

//改变默认部件风格
class CustomProxyStyle: public QProxyStyle{
public:
	CustomProxyStyle(QObject* parent) {
		setParent(parent);
	}

	virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override {
		if(PE_FrameFocusRect == pe) {
			//去掉Windows中默认部件的边框或虚线，部件获取焦点时直接返回，不进行绘制
			return;
		} else {
			QProxyStyle::drawPrimitive(pe, opt, p, w);
		}
	}
};

class CommonUtils {
public:
	CommonUtils();
	~CommonUtils() = default;

public:
	static QPixmap getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize{ 0, 0 });
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);
	static void setDefaultSkinColor(const QColor& color);
	static QColor getDefaultSkinColor();
};
