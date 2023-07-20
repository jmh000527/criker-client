#include "CommonUtils.h"

#include <QFile>
#include <QPainter>
#include <QWidget>
#include <QApplication>
#include <QDir>
#include <QSettings>

CommonUtils::CommonUtils() { }

QPixmap CommonUtils::getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize) {
	if (maskSize == QSize{ 0, 0 }) {
		maskSize = mask.size();
	} else {
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	QImage resultImage{ maskSize, QImage::Format_ARGB32_Premultiplied };
	QPainter painter{ &resultImage };

	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);

	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);

	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	painter.end();

	return QPixmap::fromImage(resultImage);
}

void CommonUtils::loadStyleSheet(QWidget* widget, const QString& sheetName) {
	QFile file{ ":/Resources/QSS/" + sheetName + ".css" };

	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		widget->setStyleSheet(qStyleSheet);
	}

	file.close();
}

void CommonUtils::setDefaultSkinColor(const QColor& color) {
	const QString&& path{ QApplication::applicationDirPath() + QDir::separator() + QString{ "tradeprintinfo.ini" } };
	QSettings settings{ path, QSettings::IniFormat };

	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

QColor CommonUtils::getDefaultSkinColor() {
	const QString&& path{ QApplication::applicationDirPath() + QDir::separator() + QString{ "tradeprintinfo.ini" } };
	QColor color;

	if (!QFile::exists(path)) {
		color = QColor{ 22, 154, 218 };
		setDefaultSkinColor(color);
	} else {
		QSettings settings{ path, QSettings::IniFormat };

		int r{ settings.value("DefaultSkin/red").toInt() };
		int g{ settings.value("DefaultSkin/green").toInt() };
		int b{ settings.value("DefaultSkin/blue").toInt() };

		color = QColor{ r, g, b };
	}

	return color;
}
