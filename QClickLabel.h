#pragma once

#include <QLabel>

class QClickLabel : public QLabel {
	Q_OBJECT

public:
	QClickLabel(QWidget* parent);
	~QClickLabel() override;

protected:
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void clicked();
};
