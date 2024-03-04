#include "PictureEdit.h"

#include <QFileDialog>

#include "CommonUtils.h"

PictureEdit::PictureEdit(const QString& filePath, QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	ui.widget->LoadPicture(filePath);

	PictureEdit::loadStyleSheet("PictureEdit");

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
}

PictureEdit::~PictureEdit() = default;

QImage PictureEdit::getHeadImage() const {
	return m_headImage;
}

void PictureEdit::loadStyleSheet(const QString& sheetName) {
	m_styleSheetName = sheetName;
	QFile file{ ":/Resources/QSS/" + sheetName + ".css" };
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		//获取用户当前皮肤的RGB
		const QString r{ QString::number(m_colorBackground.red()) };
		const QString g{ QString::number(m_colorBackground.green()) };
		const QString b{ QString::number(m_colorBackground.blue()) };

		const auto increaseValue{ 230 };
		qStyleSheet += QString("QWidget[titleSkin = true] {\
									background-color: rgb(%1, %2, %3);\
									border-top-left-radius: 4px;\
									border-top-right-radius: 4px;\
									border: 1px solid rgb(%1, %2, %3);\
									border-bottom: none;\
								}\
								QWidget[bottomSkin = true] {\
									background-color: rgb(%4, %5, %6);\
									border: 1px solid rgb(%1, %2, %3);\
									border-top: none;\
								}\
								QWidget#PictureEditClass {\
									border: 1px solid rgb(%1, %2, %3);\
									border-top-left-radius: 4px;\
									border-top-right-radius: 4px;\
									background-color: rgb(%4, %5, %6);\
								}").arg(r).arg(g).arg(b)
											.arg(qMin(r.toInt() / 10 + increaseValue, 255))
											.arg(qMin(g.toInt() / 10 + increaseValue, 255))
											.arg(qMin(b.toInt() / 10 + increaseValue, 255));

		setStyleSheet(qStyleSheet);
	}

	file.close();
}

void PictureEdit::on_pushButton_clicked() {
	auto rectToCut = ui.widget->getBackgroundWidget()->getRectToCut().toRect();
	const auto image = ui.widget->getImageView()->getCurrentScaledImage();
	const auto adjestValueX = ui.widget->getImageView()->getXBlank();
	const auto adjestValueY = ui.widget->getImageView()->getYBlank();

	rectToCut.moveTo(qMax(0, rectToCut.x()), qMax(0, rectToCut.y()));
	const auto finalRect = QRect{ QPoint{rectToCut.topLeft().x() - adjestValueX, rectToCut.topLeft().y() - adjestValueY}, rectToCut.size() };
	m_headImage = image.copy(finalRect);

	emit sendData(m_headImage);

	close();
	// croppedImage.save("C:/Users/jmh00/Desktop/3.jpg", "JPEG", 80);

}
