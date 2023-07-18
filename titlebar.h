#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum class ButtonType {
	MIN_BUTTON = 0,
	//最小化及关闭按钮

	MIN_MAX_BUTTON,
	//最小化、最大化及关闭按钮

	ONLY_CLOSE_BUTTON,
	//仅关闭按钮
};

//自定义标题栏
class TitleBar : public QWidget {
	Q_OBJECT

public:
	TitleBar(QWidget* parent = nullptr);
	~TitleBar();

	void setTitleIcon(const QString& filePath);
	void setTitleContent(const QString& titleContent);
	void setTitleWidth(int width);
	void setButtonType(ButtonType buttonType);

	//保存&获取最大化前窗口位置及大小
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	void getRestoreInfo(QPoint& point, QSize& size);

private:
	void paintEvent(QPaintEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	//初始化控件
	void initControl();
	//初始化signal-slot
	void initConnections();
	//加载样式表
	void loadStyleSheet(const QString& sheetName);

signals:
	//最小化按钮
	void signalButtonMinClicked();
	//最大化还原按钮
	void signalButtonRestoreClicked();
	//最大化按钮
	void signalButtonMaxClicked();
	//关闭按钮
	void signalButtonCloseClicked();

private slots:
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

private:
	//标题栏图标
	QLabel* m_pIcon{};
	//标题栏内容
	QLabel* m_ptitleContent{};
	QPushButton* m_pButtonMin{};
	QPushButton* m_pButtonRestore{};
	QPushButton* m_pButtonMax{};
	QPushButton* m_pButtonClose{};

	//最大化还原按钮
	QPoint m_restorePos;
	QSize m_restoreSize;

	//是否移动
	bool m_isPressed;
	QPoint m_startMovePos;

	//标题栏内容
	QString m_titleContent;

	//标题栏按钮类型
	ButtonType m_buttonType;
};
