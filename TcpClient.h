#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <semaphore>

#include "MsgType.h"

#include "thirdparty/include/nlohmann/json.hpp"

// 定义包头结构
struct MessageHeader {
	uint16_t type;    // 消息类型
	uint16_t length;  // 消息长度
};

class TcpClient : public QObject {
	Q_OBJECT

public:
	TcpClient() = default;

	static TcpClient* getInstance();

	TcpClient(const TcpClient&) = delete; // 禁用拷贝构造函数
	TcpClient& operator=(const TcpClient&) = delete; // 禁用赋值运算符

	static bool establishConnection(const QString& ipAddress, quint16 portNumber);
	static bool sendMessage(const nlohmann::json& js, MsgType msgType);
	static std::tuple<QByteArray, MsgType> receiveMessage();
	static void closeConnection();

	void readTaskHandler();

	void doRegResponse(nlohmann::json& responsejs);
	void doLoginResponse(nlohmann::json& responsejs);

	static std::atomic_bool isLoginSuccess;
	static std::counting_semaphore<1> rwsem;

private:
	//构造消息
	static std::vector<char> constructMessage(const nlohmann::json& js, MsgType msgtype);

	static QTcpSocket* m_socket;

signals:
	void messageReceived(const QString& msg, const QString& time);
};
