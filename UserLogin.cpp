#include "UserLogin.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>

#include <thread>

#include "CCMainWindow.h"
#include "CommonInfo.h"
#include "MsgType.h"
#include "TcpClient.h"
#include "UserRegister.h"

UserLogin::UserLogin(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	TcpClient::getInstance();

	if (!TcpClient::getInstance()->establishConnection("192.168.199.138", 6500)) {
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
	auto y = 106 - headLabelSize.height() / 2;
	headLabel->move(width() / 2 - headLabelSize.width() / 2, y);

	connect(ui.buttonLogin, &QPushButton::clicked, this, &UserLogin::onLoginButtonClicked);
	connect(ui.buttonRegister, &QPushButton::clicked, this, &UserLogin::onRegisterButtonClicked);
}

// bool UserLogin::connectMySQL() {
// 	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
// 	db.setDatabaseName("chatqq");
// 	db.setHostName("127.0.0.1");
// 	db.setUserName("root");
// 	db.setPassword("JMH20000527");
// 	db.setPort(3306);
//
// 	if (db.open()) {
// 		return true;
// 	} else {
// 		QSqlError error = db.lastError();
// 		QMessageBox::information(NULL, QString{ "连接数据库失败" }, db.lastError().databaseText());
//
// 		// QFile file(LOG_FILE);
// 		// file.open(QIODevice::WriteOnly | QIODevice::Append);
// 		// QTextStream text_stream(&file);
// 		// text_stream << "database connect error:" << errno << " " << error.driverText() << "\r\n";
// 		// QStringList drivers = db.drivers();
// 		// for (int i = 0; i < drivers.size(); i++) {
// 		// 	text_stream << "drives:" << drivers[i] << "\r\n";
// 		// }
// 		// text_stream << "bool:" << db.isOpenError() << "driverName:" << db.driverName() << "\r\n";
// 		// text_stream << error.nativeErrorCode();
// 		//
// 		// file.flush();
// 		// file.close();
//
// 		return false;
// 	}
// }

bool UserLogin::verifyAccountCode() const {
	QString account = ui.lineEditUserAccount->text();
	QString password = ui.lineEditUserPassword->text();

	//构造登录JSON请求
	nlohmann::json js;
	js["msgtype"] = static_cast<int>(MsgType::LOGIN_MSG);
	js["id"] = std::atoi(account.toStdString().c_str());
	js["password"] = password.toStdString();

	TcpClient::isLoginSuccess = false;

	//发送登录请求
	if (!TcpClient::getInstance()->sendMessage(js, MsgType::LOGIN_MSG)) {
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

void UserLogin::loadStyleSheet(const QString& sheetName) {
	m_styleSheetName = sheetName;
	QFile file{ ":/Resources/QSS/" + sheetName + ".css" };
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		//获取用户当前皮肤的RGB
		const QString r{ QString::number(m_colorBackground.red()) };
		const QString g{ QString::number(m_colorBackground.green()) };
		const QString b{ QString::number(m_colorBackground.blue()) };

		const auto increaseValue{ 230 };
		qStyleSheet += QString("QWidget#UserLoginClass {\
								background-color: transparent;\
								border-radius: 4px;\
								}\
								QWidget#titleWidget, #titleWidget_2 {\
								background-color: rgb(%1, %2, %3);\
								border-top-right-radius: 4px;\
								border-top-left-radius: 4px;\
								}\
								QWidget#bodyWidget, #bodyWidget_2 {\
								background-color: rgb(%4, %5, %6);\
								border-bottom-right-radius: 4px;\
								border-bottom-left-radius: 4px;\
								border: 1px solid rgb(%1, %2, %3);\
								}\
								QPushButton#buttonLogin {\
								font-size: 14px;\
								background: rgb(%1, %2, %3);\
								color: #ffffff;\
								border: none;\
								border-radius: 4px;\
								}\
								QPushButton#buttonLogin:hover,\
								QPushButton#buttonLogin:pressed{\
								background: rgba(%1, %2, %3, 150);\
								}").arg(r).arg(g).arg(b)
								   .arg(qMin(r.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(g.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(b.toInt() / 10 + increaseValue, 255));

		setStyleSheet(qStyleSheet);
	}

	file.close();
}

void UserLogin::onLoginButtonClicked() {
	if (!verifyAccountCode()) {
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("名输入的账号或密码有误,请从新输入!"));
		ui.lineEditUserAccount->clear();
		// ui.lineEditUserPassword->clear();
		return;
	}

	close();

	CCMainWindow* mainWindow{ new CCMainWindow{} };
	mainWindow->show();
}

void UserLogin::onRegisterButtonClicked() {
	// UserRegister* userRegister{ new UserRegister{} };
	//
	// // // 生成随机偏移量
	// // int xOffset = QRandomGenerator::global()->bounded(50);  // 范围为[0, 200)
	// // int yOffset = QRandomGenerator::global()->bounded(50);  // 范围为[0, 200)
	// //
	// // // 设置第二个窗口的初始位置，随机偏移
	// // userRegister->move(this->pos() + QPoint(xOffset, yOffset));
	//
	// userRegister->show();

	ui.stackedWidget->setCurrentIndex(1);
}
