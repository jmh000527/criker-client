#include "CCMainWindow.h"
#include "CCMainWindow.h"

#include <QPainter>
#include <QTimer>

#include "CommonUtils.h"
#include "ContactItem.h"
#include "NotifyManager.h"
#include "RootContactItem.h"
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
	auto* timer{ new QTimer{ this } };
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

	const auto appUpLayout{ new QHBoxLayout{ this } };

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

	const auto appBottomLayout{ new QHBoxLayout{ this } };

	appBottomLayout->setContentsMargins(QMargins{ 0, 0, 0, 0 });
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	appBottomLayout->addWidget(createOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));

	ui.bottomLayout_up->addLayout(appBottomLayout);
	ui.bottomLayout_up->addStretch();

	//个性签名
	ui.lineEdit->installEventFilter(this);
	//搜索
	ui.searchLineEdit->installEventFilter(this);

	//单独更新搜索部件的颜色
	updateSearchStyle();

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowQuit(bool)));

	//实现在更换皮肤时，同时更新搜索部件底色
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, this, &CCMainWindow::updateSearchStyle);

	//系统托盘
	auto* sysTray{ new SysTray{ this } };

	//初始化联系人部件
	initContactTree();
}

void CCMainWindow::initContactTree() {
	//展开与收纳的信号
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
	        SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem * item)), this,
	        SLOT(onItemExpanded(QTreeWidgetItem * item)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem * item)), this,
			SLOT(onItemCollapsed(QTreeWidgetItem * item)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem * item, int column)), this,
			SLOT(onItemDoubleClicked(QTreeWidgetItem * item, int column)));

	//根节点
	QTreeWidgetItem* pRootGroupItem{ new QTreeWidgetItem };
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, QVariant{ 0 });

	RootContactItem* pItemName{ new RootContactItem{true, ui.treeWidget} };
	pItemName->setText(QString{ "西北大学" });

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);

	QStringList departments{ "信息科学与技术学院", "文学院", "外国语学院", "考古学院" };
	for(const auto& department: departments) {
		addDepartment(pRootGroupItem, department);
	}
}

void CCMainWindow::setUserName(const QString& username) const {
	//文本过长则省略过长部分
	const QString elidedUserName{
		ui.nameLabel->fontMetrics().elidedText(username, Qt::ElideRight, ui.nameLabel->width()) };

	ui.nameLabel->setText(elidedUserName);
}

void CCMainWindow::setUserLevelPixmap(const int level) const {
	QPixmap levePixmap{ ui.levelBtn->size() };
	levePixmap.fill(Qt::transparent);

	QPainter painter{ &levePixmap };
	painter.drawPixmap(0, 4, QPixmap{ ":/Resources/MainWindow/lv.png" });

	const auto unitNum{ level % 10 }; //个位数
	const auto tenNum{ level / 10 % 10 }; //十位数

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
	const auto button{ new QPushButton{ this } };
	button->setFixedSize(QSize{ 20, 20 });

	QPixmap pixmap{ button->size() };
	pixmap.fill(Qt::transparent);

	QPainter painter{ &pixmap };
	const QPixmap appPixmap{ appPath };

	painter.drawPixmap((button->width() - appPixmap.width()) / 2, (button->height() - appPixmap.height()) / 2,
	                   appPixmap);
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

bool CCMainWindow::eventFilter(QObject* watched, QEvent* event) {
	if (ui.searchLineEdit == watched) {
		//键盘焦点事件
		if (event->type() == QEvent::FocusIn) {
			//获取用户当前皮肤的RGB
			const QString r{ QString::number(m_colorBackground.red()) };
			const QString g{ QString::number(m_colorBackground.green()) };
			const QString b{ QString::number(m_colorBackground.blue()) };

			ui.searchWidget->setStyleSheet(QString{ "QWidget#searchWidget{\
													background-color: rgb(255, 255, 255);\
													border-bottom: 1px rgb(%1, %2, %3);\
													}\
													QWidget#searchLineEdit{\
													border-bottom: 1px rgb(%1, %2, %3);\
													}\
													QPushButton#searchBtn{\
													border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)\
													}\
													QPushButton#searchBtn:hover{\
													border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)\
													}\
													QPushButton#searchBtn:pressed{\
													border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)\
													}" }
			                               .arg(r)
			                               .arg(g)
			                               .arg(b));
		} else if (event->type() == QEvent::FocusOut) {
			//还原搜索部件样式
			updateSearchStyle();
		}
	}

	return BasicWindow::eventFilter(watched, event);
}

void CCMainWindow::updateSearchStyle() {
	//获取用户当前皮肤的RGB
	const QString r{ QString::number(m_colorBackground.red()) };
	const QString g{ QString::number(m_colorBackground.green()) };
	const QString b{ QString::number(m_colorBackground.blue()) };
	const auto increaseValue{ 230 };

	ui.searchWidget->setStyleSheet(QString{ "QWidget#searchWidget {\
											background-color:rgb(%1, %2, %3);\
											border-bottom:1px rgb(%1, %2, %3);\
											}\
											QPushButton#searchBtn {\
											border-image:url(:Resources/MainWindow/search/search_icon.png);\
											}" }
	                               .arg(qMin(r.toInt() / 10 + increaseValue, 255))
	                               .arg(qMin(g.toInt() / 10 + increaseValue, 255))
	                               .arg(qMin(b.toInt() / 10 + increaseValue, 255)));
}

void CCMainWindow::addDepartment(QTreeWidgetItem* pRootGroupItem, const QString& departmentName) {
	QTreeWidgetItem* pChildItem{ new QTreeWidgetItem };
	//添加子节点，子项数据为1
	pChildItem->setData(0, Qt::UserRole, 1);

	QPixmap pixmap{ ":/Resources/MainWindow/head_mask.png" };

	ContactItem* pContactItem{ new ContactItem{ui.treeWidget} };
	pContactItem->setHeadPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/girl.png" }, pixmap, pContactItem->getHeadLabelSize()));
	pContactItem->setUsername(departmentName);

	pRootGroupItem->addChild(pChildItem);
	ui.treeWidget->setItemWidget(pChildItem, 0, pContactItem);
}
void CCMainWindow::onAppIconClicked() const {
	if (sender()->objectName() == "app_skin") {
		SkinWindow* skinWindow{ new SkinWindow };
		skinWindow->show();
	}
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int column) {
	//数据为0表示根项，数据为1表示子项
	bool isChild{ item->data(0, Qt::UserRole).toBool() };
	if(!isChild) {
		item->setExpanded(!item->isExpanded());
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem* item) {}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item) {}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column) {}
