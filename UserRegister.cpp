#include "UserRegister.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QTimer>

#include "MsgType.h"
#include "PictureEdit.h"
#include "QClickLabel.h"
#include "TcpClient.h"

UserRegister::UserRegister(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	loadStyleSheet("UserLogin");

	initControl();

	// initTitleBar();
}

UserRegister::~UserRegister() = default;

void UserRegister::initControl() {
	m_headLabel = new QClickLabel{ this };
	const QSize headLabelSize{ 68, 68 };

	m_headLabel->setFixedSize(headLabelSize);

	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };

	auto i = ui.titleWidget->height();
	auto j = ui.titleWidget_2->size().height();

	m_headLabel->setPixmap(getRoundedImage(QPixmap{ ":/Resources/MainWindow/head_with_cross.png" }, pix,
	                                       m_headLabel->size()));
	m_headLabel->move(width() / 2 - headLabelSize.width() / 2,
	                  ui.titleWidget->height() + ui.titleWidget_2->height() - headLabelSize.height() / 2);

	connect(m_headLabel, &QClickLabel::clicked, this, &UserRegister::onHeadLabelClicked);
	connect(ui.buttonRegister2, &QPushButton::clicked, this, &UserRegister::onRegister2ButtonClicked);
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
}

void UserRegister::loadStyleSheet(const QString& sheetName) {
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
		qStyleSheet += QString("QWidget#UserRegisterClass {\
								border: 1px solid rgb(%1, %2, %3);\
								background-color: rgb(%1, %2, %3);\
								border-radius: 4px;\
								}\
								QWidget#titleWidget {\
								background-color: rgb(%1, %2, %3);\
								border-top-right-radius: 4px;\
								border-top-left-radius: 4px;\
								}\
								QWidget#bodyWidget {\
								background-color: rgb(%4, %5, %6);\
								border-bottom-right-radius: 4px;\
								border-bottom-left-radius: 4px;\
								}\
								QPushButton#buttonRegister2 {\
								font-size: 14px;\
								background: rgb(%1, %2, %3);\
								color: #ffffff;\
								border: none;\
								border-radius: 4px;\
								}\
								QPushButton#buttonRegister2:hover,\
								QPushButton#buttonRegister2:pressed{\
								background: rgba(%1, %2, %3, 150);\
								}").arg(r).arg(g).arg(b)
								   .arg(qMin(r.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(g.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(b.toInt() / 10 + increaseValue, 255));

		setStyleSheet(qStyleSheet);
	}

	file.close();
}

void UserRegister::onReceiveData(QImage& headImage) {
	QPixmap pix{ ":/Resources/MainWindow/head_mask.png" };
	m_headLabel->setPixmap(getRoundedImage(QPixmap::fromImage(headImage), pix, m_headLabel->size()));
	m_headImage = headImage;
}

void UserRegister::onHeadLabelClicked() {
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

void UserRegister::onRegister2ButtonClicked() {
	QString name = ui.lineEditUserAccount->text();
	QString password = ui.lineEditUserPassword->text();
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
	}
}
