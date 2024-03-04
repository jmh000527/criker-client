#pragma once

#include <QDialog>

#include "TitleBar.h"

class BasicWindow : public QDialog {
	Q_OBJECT

public:
	BasicWindow(QWidget* parent = nullptr);
	~BasicWindow() override;

	virtual void loadStyleSheet(const QString& sheetName);
	QPixmap getRoundedImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize{ 0, 0 }) const;

private:
	void initBackgroundColor();

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void initTitleBar(ButtonType buttonType = ButtonType::MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = "") const;

public slots:
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onShowClose(bool);
	void onSignalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	TitleBar* m_titlebar{};

	QPoint m_mousePos;
	bool m_isMousePressed;
	QColor m_colorBackground;
	QString m_styleSheetName;
};
