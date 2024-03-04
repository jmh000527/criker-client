#include "QClickLabel.h"

#include <QMouseEvent>

QClickLabel::QClickLabel(QWidget* parent)
	: QLabel{ parent } { }

QClickLabel::~QClickLabel() = default;

void QClickLabel::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}

	return QLabel::mousePressEvent(event);
}
