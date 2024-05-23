#pragma once

#include <mutex>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <semaphore>

#include "MsgType.h"

#include "json.hpp"

constexpr int MAX_PACKAGE_SIZE = 10 * 1024 * 1024;

// 定义包头结构
#pragma pack(push, 1)  // 关闭内存对齐
struct MessageHeader {
	uint16_t type;   // 消息类型
	uint32_t length; // 消息长度
};
#pragma pack(pop)  // 恢复默认的内存对齐方式

class TcpClient : public QObject {
	Q_OBJECT

public:
	TcpClient();

	static TcpClient* getInstance();

	TcpClient(const TcpClient&) = delete; // 禁用拷贝构造函数
	TcpClient& operator=(const TcpClient&) = delete; // 禁用赋值运算符

	bool sendMessage(const nlohmann::json& js, MsgType msgType) const;
	void closeConnection();

	void readTaskHandler();
	void messageTaskHandler();
	bool establishConnection(const QString& ipAddress, quint16 portNumber);

	static std::atomic_bool isLoginSuccess;
	static std::atomic_bool isRegisterSuccess;
	static std::counting_semaphore<1> rwsem;

private:
	//构造消息
	static std::vector<char> constructMessage(const nlohmann::json& js, MsgType msgtype);
	std::list<std::tuple<QByteArray, MsgType>> receiveMessage() const;
	void doRegResponse(nlohmann::json& responsejs);
	void doLoginResponse(nlohmann::json& responsejs);

	QTcpSocket* m_socket;
	mutable std::mutex m_mutex;

signals:
	void messageReceived(const QString msg, const QString time, const QString senderId);

private slots:
	void onReadyRead() const;
};
