#include "CCMainWindow.h"
#include "CCMainWindow.h"

#include <QPainter>
#include <QTimer>

#include "CommonUtils.h"
#include "SkinWindow.h"
#include "SysTray.h"

CCMainWindow::CCMainWindow(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");

	initColtrol();
	initTimer();
}

CCMainWindow::~CCMainWindow() = default;

void CCMainWindow::initTimer() {
	auto* timer{ new QTimer{this} };
	timer->setInterval(std::chrono::seconds(1));
	connect(timer, &QTimer::timeout,
			[this]() {
				static int level{};
				if (level == 99) {
					level = 0;
				}
				level++;

				setUserLevelPixmap(level);
			}
	);

	timer->start();
}

void CCMainWindow::initColtrol() {
	ui.treeWidget->setStyle(new CustomProxyStyle{ this });

	setUserLevelPixmap(0);
	setUserHeadPixmap(":/Resources/MainWindow/girl.png");
	setUserStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	auto appUpLayout{ new QHBoxLayout{this} };

	appUpLayout->setContentsMargins(QMargins{ 0, 0, 0, 0 });
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appUpLayout->addStretch();
	appUpLayout->setSpacing(2);
	appUpLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));

	ui.appWidget->setLayout(appUpLayout);

	auto appBottomLayout{ new QHBoxLayout{this} };

	appBottomLayout->setContentsMargins(QMargins{ 0, 0, 0, 0 });
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));

	ui.bottomLayout_up->addLayout(appBottomLayout);
	ui.bottomLayout_up->addStretch();

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowQuit(bool)));

	//系统托盘
	auto* sysTray{ new SysTray{this} };
}

void CCMainWindow::setUserName(const QString& username) {
	//文本过长则省略过长部分
	QString elidedUserName{ ui.nameLabel->fontMetrics().elidedText(username, Qt::ElideRight, ui.nameLabel->width()) };
	
	ui.nameLabel->setText(elidedUserName);
}

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

void CCMainWindow::setUserHeadPixmap(const QString& headPath) const {
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundedImage(QPixmap{ headPath }, pix, ui.headLabel->size()));
}

void CCMainWindow::setUserStatusMenuIcon(const QString& statusPath) const {
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
	button->setObjectName(appName);
	button->setProperty("hasBorder", true);

	connect(button, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);

	return button;
}

void CCMainWindow::resizeEvent(QResizeEvent* event) {
	setUserName(QString{ "姬小明zzzzzzzzzzzzzzzzzzzzzzzzzzzzz" });

	return BasicWindow::resizeEvent(event);
}

void CCMainWindow::onAppIconClicked() const {
	if(sender()->objectName() == "app_skin") {
		SkinWindow* skinWindow{ new SkinWindow };
		skinWindow->show();
	}
}
