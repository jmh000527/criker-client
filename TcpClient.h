#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <semaphore>

#include "thirdparty/include/nlohmann/json.hpp"


class TcpClient : public QObject {
	Q_OBJECT

public:
	TcpClient() = default;

	static TcpClient* getInstance();

	TcpClient(const TcpClient&) = delete; // 禁用拷贝构造函数
	TcpClient& operator=(const TcpClient&) = delete; // 禁用赋值运算符

	static bool establishConnection(const QString& ipAddress, quint16 portNumber);
	static bool sendMessage(const QString& message);
	static QByteArray receiveMessage();
	static void closeConnection();

	void readTaskHandler();

	void doRegResponse(nlohmann::json& responsejs);
	void doLoginResponse(nlohmann::json& responsejs);

	static std::atomic_bool isLoginSuccess;
	static std::counting_semaphore<1> rwsem;

private:
	static QTcpSocket* m_socket;

signals:
	void messageReceived(const QString& msg, const QString& time);
};
