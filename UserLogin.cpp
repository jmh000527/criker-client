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

UserLogin::~UserLogin() = default;

void UserLogin::initControl() {
	QLabel* headLabel{ new QLabel{ this } };
	QSize headLabelSize{ 68, 68 };

	headLabel->setFixedSize(headLabelSize);

	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };

	headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/girl.png" }, pix, headLabel->size()));
	headLabel->move(width() / 2 - headLabelSize.width() / 2, ui.titleWidget->height() - headLabelSize.height() / 2);

	connect(ui.buttonLogin, &QPushButton::clicked, this, &UserLogin::onLoginButtonClicked);
}

void UserLogin::onLoginButtonClicked() {
	close();

	CCMainWindow* mainWindow{ new CCMainWindow };
	mainWindow->show();
}
