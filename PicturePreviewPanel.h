#pragma once

#include <QWidget>

#include "BackgroundWidget.h"
#include "ImageView.h"

class PicturePreviewPanel : public QWidget {
	Q_OBJECT

public:
	PicturePreviewPanel(QWidget* parent = nullptr);
	~PicturePreviewPanel() override;

	BackgroundWidget* getBackgroundWidget() const;
	ImageView* getImageView() const;

	void LoadPicture(const QString& filepath); //加载图片

protected:
	virtual bool eventFilter(QObject*, QEvent*) Q_DECL_OVERRIDE;

private:
	void InitializeUI();
	void LoadPicture_p() const;

	QString m_PicturePath;
	ImageView* m_PictureContainer{};
	BackgroundWidget* m_BgWidget{};
};
