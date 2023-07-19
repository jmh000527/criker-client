#pragma once

#include <QDialog>

#include "titlebar.h"

class BasicWindow : public QDialog {
	Q_OBJECT

public:
	BasicWindow(QWidget* parent = nullptr);
	virtual ~BasicWindow();

	void loadStyleSheet(const QString& sheetName);
	QPixmap getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize{ 0, 0 });

private:
	void initBackgroundColor();

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void initTitleBar(ButtonType buttonType = ButtonType::MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = "");

public slots:
	void onShow(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNoemal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	TitleBar* m_titlebar{};

	QPoint mousePos;
	bool m_isMousePressed;
	QColor m_colorBackground;
	QString m_styleSheetName;
};
