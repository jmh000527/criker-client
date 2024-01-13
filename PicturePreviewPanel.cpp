#include "PicturePreviewPanel.h"

#include <QVBoxLayout>
#include <QEvent>

PicturePreviewPanel::PicturePreviewPanel(QWidget* parent)
	: QWidget(parent) {
	InitializeUI();
}

PicturePreviewPanel::~PicturePreviewPanel() = default;

BackgroundWidget* PicturePreviewPanel::getBackgroundWidget() const {
	return m_BgWidget;
}

ImageView* PicturePreviewPanel::getImageView() const {
	return m_PictureContainer;
}

void PicturePreviewPanel::LoadPicture(const QString& filepath) {
	m_PicturePath = filepath;

	LoadPicture_p();
}

bool PicturePreviewPanel::eventFilter(QObject* watched, QEvent* event) {
	if (watched == m_PictureContainer) {
		if (event->type() == QEvent::Resize) {
			LoadPicture_p();
			if (m_BgWidget) {
				m_BgWidget->resize(m_PictureContainer->size());
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}

void PicturePreviewPanel::InitializeUI() {
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	m_PictureContainer = new ImageView;

	m_PictureContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	m_PictureContainer->installEventFilter(this);

	mainLayout->addWidget(m_PictureContainer);

	setLayout(mainLayout);

	m_BgWidget = new BackgroundWidget(m_PictureContainer, ShapeType::Rect);
	m_BgWidget->show();
}

void PicturePreviewPanel::LoadPicture_p() const {
	QPixmap picture;
	picture.load(m_PicturePath);
	if (!picture.isNull()) {
		// picture = picture.scaled(m_PictureContainer->width(), m_PictureContainer->height());
		picture = picture.scaledToWidth(m_PictureContainer->width(), Qt::SmoothTransformation);

		m_PictureContainer->loadImage(m_PicturePath);
		m_BgWidget->PictureLoadFinished();
	}
}
