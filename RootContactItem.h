#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContactItem : public QLabel {
	Q_OBJECT
	//箭头角度
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	RootContactItem(bool showIndicator = true, QWidget* parent = nullptr);
	~RootContactItem();

	void setText(const QString& title);
	void setExpanded(bool expand);

private:
	int rotation();
	void setRotation(int rotation);

	QPropertyAnimation* m_animation;
	QString m_titleText{}; //显示文本
	int m_rotation{}; //箭头角度
	bool m_showIndicator{}; //是否显示箭头

protected:
	void paintEvent(QPaintEvent* event) override;
};
