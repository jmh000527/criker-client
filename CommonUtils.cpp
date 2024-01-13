#include "CommonUtils.h"

#include <QFile>
#include <QPainter>
#include <QWidget>
#include <QApplication>
#include <QDir>
#include <QSettings>

#include "UserManager.h"

CommonUtils::CommonUtils() = default;

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
		const QString qStyleSheet{ QLatin1String{ file.readAll() } };

		widget->setStyleSheet(qStyleSheet);
	}

	file.close();
}

void CommonUtils::setDefaultSkinColor(const QColor& color) {
	const QString path{ QApplication::applicationDirPath() + QDir::separator() + QString{ "tradeprintinfo.ini" } };
	QSettings settings{ path, QSettings::IniFormat };

	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

QColor CommonUtils::getDefaultSkinColor() {
	const QString path{ QApplication::applicationDirPath() + QDir::separator() + QString{ "tradeprintinfo.ini" } };
	QColor color;

	if (!QFile::exists(path)) {
		color = QColor{ 22, 154, 218 };
		setDefaultSkinColor(color);
	} else {
		const QSettings settings{ path, QSettings::IniFormat };

		const int r{ settings.value("DefaultSkin/red").toInt() };
		const int g{ settings.value("DefaultSkin/green").toInt() };
		const int b{ settings.value("DefaultSkin/blue").toInt() };

		color = QColor{ r, g, b };
	}

	return color;
}

QPixmap CommonUtils::base64ToQPixmap(const std::string& base64String) {
	// 假设从服务器接收到的 Base64 图像数据
	QString base64ImageData = QString{ base64String.c_str() };

	// 将 Base64 字符串转换为 QByteArray
	QByteArray imageData = base64ToByteArray(base64ImageData);

	// 将 QByteArray 转换为 QPixmap
	QPixmap headImage = byteArrayToPixmap(imageData);

	return headImage;
}

QByteArray CommonUtils::base64ToByteArray(const QString& base64String) {
	return QByteArray::fromBase64(base64String.toUtf8());
}

QPixmap CommonUtils::byteArrayToPixmap(const QByteArray& imageData) {
	QImage image = QImage::fromData(imageData);
	return QPixmap::fromImage(image);
}
