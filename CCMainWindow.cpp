#include "CCMainWindow.h"

#include <QPainter>

#include "CommonUtils.h"

CCMainWindow::CCMainWindow(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");

	initColtrol();
}

CCMainWindow::~CCMainWindow() = default;

void CCMainWindow::initColtrol() {
	ui.treeWidget->setStyle(new CustomProxyStyle{ this });

	setUserLevelPixmap(0);
	setUserHeadPixmap(":/Resources/MainWindow/girl.png");
	setUserStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	auto appUpLayout{ new QHBoxLayout{this} };

	appUpLayout->setContentsMargins(QMargins{ 0, 0, 0, 0 });
	// appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/"))
	
}

void CCMainWindow::setUserName(const QString& username) {}

void CCMainWindow::setUserLevelPixmap(const int level) const {
	QPixmap levePixmap{ ui.levelBtn->size() };
	levePixmap.fill(Qt::transparent);

	QPainter painter{ &levePixmap };
	painter.drawPixmap(0, 4, QPixmap{ ":/Resources/MainWindow/lv.png" });

	const auto unitNum{ level % 10 };	//个位数
	const auto tenNum{ level / 10 % 10 };	//十位数

	//十位，截取图片中的部分进行绘制
	painter.drawPixmap(10, 4, QPixmap{ ":/Resources/MainWindow/levelvalue.png" }, tenNum * 6, 0, 6, 7);

	//个位，同上
	painter.drawPixmap(16, 4, QPixmap{ ":/Resources/MainWindow/levelvalue.png" }, unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levePixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setUserHeadPixmap(const QString& headPath) {
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundedImage(QPixmap{ headPath }, pix, ui.headLabel->size()));
}

void CCMainWindow::setUserStatusMenuIcon(const QString& statusPath) {
	QPixmap statusBtnPixmap{ ui.statusBtn->size() };
	statusBtnPixmap.fill(Qt::transparent);
	QPainter painter{ &statusBtnPixmap };
	painter.drawPixmap(4, 4, QPixmap{ statusPath });

	ui.statusBtn->setIcon(statusBtnPixmap);
	ui.statusBtn->setIconSize(ui.statusBtn->size());
}

QWidget* CCMainWindow::createOtherAppExtension(const QString& appPath, const QString& appName) {
	const auto button{ new QPushButton{this} };
	button->setFixedSize(QSize{ 20, 20 });

	QPixmap pixmap{ button->size() };
	pixmap.fill(Qt::transparent);

	QPainter painter{ &pixmap };
	const QPixmap appPixmap{ appPath };

	painter.drawPixmap((button->width() - appPixmap.width()) / 2, (button->height() - appPixmap.height()) / 2, appPixmap);
	button->setIcon(pixmap);
	button->setIconSize(button->size());

	return button;
}
