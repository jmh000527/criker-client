#include "UserLogin.h"

#include "CCMainWindow.h"

UserLogin::UserLogin(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin() {}

void UserLogin::initControl() {
	QLabel* headLabel{ new QLabel{ this } };
	headLabel->setFixedSize(QSize{ 68, 68 });
	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };
	headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/girl.png" }, pix, headLabel->size()));
	headLabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);

	connect(ui.buttonLogin, &QPushButton::clicked, this, &UserLogin::onLoginButtonClicked);
}

void UserLogin::onLoginButtonClicked() {
	close();

	CCMainWindow* mainWindow{ new CCMainWindow };
	mainWindow->show();
}
