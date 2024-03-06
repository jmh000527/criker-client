#include "UserLogin.h"

#include <QBuffer>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QTimer>

#include <thread>

#include "CCMainWindow.h"
#include "MsgType.h"
#include "PictureEdit.h"
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
	// setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();

	//测试用
	ui.lineEditUserAccount->setText("1");
	ui.lineEditUserPassword->setText("123456");
}

UserLogin::~UserLogin() = default;

void UserLogin::initControl() {
	m_headLabel = new QClickLabel{ this };
	const QSize headLabelSize{ 68, 68 };

	m_headLabel->setFixedSize(headLabelSize);

	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };

	m_headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/app/qq_detective.ico" }, pix, m_headLabel->size()));
	auto y = 106 - headLabelSize.height() / 2;
	m_headLabel->move(width() / 2 - headLabelSize.width() / 2, y);

	//登录页
	connect(ui.buttonLogin, &QPushButton::clicked, this, &UserLogin::onLoginButtonClicked);
	connect(ui.buttonRegister, &QPushButton::clicked, this, &UserLogin::onRegisterButtonClicked);

	//注册页
	connect(m_headLabel, &QClickLabel::clicked, this, &UserLogin::onHeadLabelClicked);
	connect(ui.buttonRegister2, &QPushButton::clicked, this, &UserLogin::onRegister2ButtonClicked);
}

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
			return true;
		}

		return false;
	}
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

void UserLogin::onReceiveData(QImage& headImage) {
	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };
	m_headLabel->setPixmap(getRoundedImage(QPixmap::fromImage(headImage), pix, m_headLabel->size()));
	m_headImage = headImage;
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
	ui.stackedWidget->setCurrentIndex(1);

	//更换新建头像标识
	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };
	m_headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/head_with_cross.png" }, pix,
	                                       m_headLabel->size()));


}

void UserLogin::onHeadLabelClicked() {
	QString filepath = QFileDialog::getOpenFileName(nullptr, QStringLiteral("选择图片"), ".", "*.jpg");

	if (!filepath.trimmed().isEmpty()) {
		auto* w = new PictureEdit(filepath);

		connect(w, &PictureEdit::destroyed, [=]() {
			delete w;
				});
		connect(w, SIGNAL(sendData(QImage&)), this, SLOT(onReceiveData(QImage&))); //子窗口发信号，主窗口接收

		w->show();
	}
}

void UserLogin::onRegister2ButtonClicked() {
	QString name = ui.lineEditUserAccount_2->text();
	QString password = ui.lineEditUserPassword_2->text();
	QString passwordConfirm = ui.lineEditUserPasswordConfirm->text();

	if (password != passwordConfirm) {
		return;
	}

	// QPixmap pixmap{ m_headLabel->pixmap() };
	QPixmap pixmap = QPixmap::fromImage(m_headImage);

	// 将 QPixmap 转换为 QByteArray
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	pixmap.save(&buffer, "JPEG");
	buffer.close();

	// 将 QByteArray 转换为 Base64 编码
	QString base64Image = byteArray.toBase64();

	//构造注册JSON请求
	nlohmann::json js;
	js["msgtype"] = static_cast<int>(MsgType::REG_MSG);
	js["name"] = name.toStdString();
	js["password"] = password.toStdString();
	js["headimage"] = base64Image.toStdString();

	TcpClient::isRegisterSuccess = false;

	//发送注册请求
	if (!TcpClient::getInstance()->sendMessage(js, MsgType::REG_MSG)) {
		QMessageBox::information(NULL, QString{ "请求发送失败" }, "请求发送失败");
		exit(1);
	}

	// 使用定时器等待服务器返回，超过五秒弹出对话框
	QTimer timer;
	timer.setSingleShot(true);

	QObject::connect(&timer, &QTimer::timeout, [&]() {
		// 释放信号量
		TcpClient::rwsem.release();
		QMessageBox::information(NULL, QString{ "注册超时" }, "注册超时，请重试");
					 });

	// 启动定时器，等待五秒
	timer.start(5000);

	// 信号量阻塞，等待服务器返回注册请求确认
	TcpClient::rwsem.acquire();

	if (TcpClient::isRegisterSuccess) {
		QMessageBox::information(NULL, QString{ "注册成功" }, "注册成功");

		disconnect(ui.buttonRegister2, &QPushButton::clicked, this, &UserLogin::onRegister2ButtonClicked);
		ui.buttonRegister2->setText("返回登陆");

		ui.lineEditUserAccount->setText(name);
		ui.buttonForgetPassword->setText(password);

		connect(ui.buttonRegister2, &QPushButton::clicked, [this]() {
			ui.stackedWidget->setCurrentIndex(0);
		});
	}
}
