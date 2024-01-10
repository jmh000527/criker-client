#include "UserLogin.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
// #include <thread>
#include <QThread>

#include "CCMainWindow.h"
#include "CommonInfo.h"
#include "MsgType.h"
#include "TcpClient.h"

UserLogin::UserLogin(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	if (!TcpClient::establishConnection("192.168.199.138", 6500)) {
		QMessageBox::information(NULL, QString{ "连接服务器失败" }, "强制退出");
		exit(1);
	}

	//开启接收服务器响应的线程
	std::thread readTask(&TcpClient::readTaskHandler, TcpClient::getInstance());
	readTask.detach();

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();

	//测试用
	ui.lineEditUserAccount->setText("1");
	ui.lineEditUserPassword->setText("123456");
}

UserLogin::~UserLogin() = default;

void UserLogin::initControl() {
	QLabel* headLabel{ new QLabel{ this } };
	const QSize headLabelSize{ 68, 68 };

	headLabel->setFixedSize(headLabelSize);

	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };

	headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/app/logo.ico" }, pix, headLabel->size()));
	headLabel->move(width() / 2 - headLabelSize.width() / 2, ui.titleWidget->height() - headLabelSize.height() / 2);

	connect(ui.buttonLogin, &QPushButton::clicked, this, &UserLogin::onLoginButtonClicked);

	// if (!connectMySQL()) {
	// 	close();
	// 	exit(-1); /*退出*/
	// }
}

bool UserLogin::connectMySQL() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("chatqq");
	db.setHostName("127.0.0.1");
	db.setUserName("root");
	db.setPassword("JMH20000527");
	db.setPort(3306);

	if (db.open()) {
		return true;
	} else {
		QSqlError error = db.lastError();
		QMessageBox::information(NULL, QString{ "连接数据库失败" }, db.lastError().databaseText());

		// QFile file(LOG_FILE);
		// file.open(QIODevice::WriteOnly | QIODevice::Append);
		// QTextStream text_stream(&file);
		// text_stream << "database connect error:" << errno << " " << error.driverText() << "\r\n";
		// QStringList drivers = db.drivers();
		// for (int i = 0; i < drivers.size(); i++) {
		// 	text_stream << "drives:" << drivers[i] << "\r\n";
		// }
		// text_stream << "bool:" << db.isOpenError() << "driverName:" << db.driverName() << "\r\n";
		// text_stream << error.nativeErrorCode();
		//
		// file.flush();
		// file.close();

		return false;
	}
}

bool UserLogin::verifyAccountCode() {
	QString account = ui.lineEditUserAccount->text();
	QString password = ui.lineEditUserPassword->text();

	//构造登录JSON请求
	nlohmann::json js;
	js["msgtype"] = static_cast<int>(MsgType::LOGIN_MSG);
	js["id"] = std::atoi(account.toStdString().c_str());
	js["password"] = password.toStdString();
	QString request = QString{ js.dump().c_str() };
	TcpClient::isLoginSuccess = false;

	//发送登录请求
	if (!TcpClient::sendMessage(request)) {
		QMessageBox::information(NULL, QString{ "请求发送失败" }, "请求发送失败");
		exit(1);
	} else {
		//信号量阻塞，等待服务器返回登录请求确认
		TcpClient::rwsem.acquire();

		if (TcpClient::isLoginSuccess) {
			// // Enter the chat main menu
			// TcpClient::isMainMenuRunning = true;
			// TcpClient::mainMenu(clientfd);
			return true;
		}

		return false;
	}

	//QQ号登陆
	QString strSqlCode = QString{ "SELECT code FROM tab_accounts WHERE employeeID=%1" }.arg(account);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();
	if (queryEmployeeID.first()) {
		QString strCode = queryEmployeeID.value(0).toString();
		if (strCode == password) {
			CommonInfo::loginEmployeeID = account;
			CommonInfo::loginPassword = account;
			CommonInfo::isAccount = false;

			return true;
		}
	}

	//账号登录
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account='%1'").arg(account);
	QSqlQuery queryAccount(strSqlCode);
	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();
		if (strCode == password) {
			CommonInfo::loginEmployeeID = queryAccount.value(1).toString();
			CommonInfo::loginPassword = account;
			CommonInfo::isAccount = true;

			return true;
		}
	}

	return false;
}

void UserLogin::onLoginButtonClicked() {
	if (!verifyAccountCode()) {
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("名输入的账号或密码有误,请从新输入!"));
		ui.lineEditUserAccount->clear();
		// ui.lineEditUserPassword->clear();
		return;
	}

	close();

	CCMainWindow* mainWindow{ new CCMainWindow{ CommonInfo::loginEmployeeID } };
	mainWindow->show();
}
