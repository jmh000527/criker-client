#include "TcpClient.h"

#include <iostream>
#include <QByteArray>
#include <QMessageBox>
#include <QtEndian>
#include <QTimer>

#include "Group.h"
#include "MsgType.h"
#include "User.h"
#include "UserManager.h"
#include "WindowManager.h"

using namespace nlohmann;

std::atomic_bool TcpClient::isLoginSuccess{ false };
std::atomic_bool TcpClient::isRegisterSuccess{ false };
std::counting_semaphore<1> TcpClient::rwsem(0);

TcpClient::TcpClient()
	: m_socket{ new QTcpSocket{ this } } {
	// connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

TcpClient* TcpClient::getInstance() {
	static TcpClient instance;
	return &instance;
}

bool TcpClient::establishConnection(const QString& ipAddress, quint16 portNumber) {
	if (!m_socket) {
		return false;
	}

	m_socket->connectToHost(ipAddress, portNumber);
	if (!m_socket->waitForConnected()) {
		qDebug() << "Error: " << m_socket->errorString();
		delete m_socket;
		m_socket = nullptr;
		return false;
	}

	return true;
}

bool TcpClient::sendMessage(const json& js, MsgType msgType) const {
	if (!m_socket) {
		qDebug() << "Socket not initialized.";
		return false;
	}

	const std::vector<char> message = constructMessage(js, msgType);

	const QByteArray data{ message.data(), static_cast<qsizetype>(message.size()) };

	if (const qint64 len = m_socket->write(data); len == -1) {
		qDebug() << "Error sending message: " << m_socket->errorString();
		return false;
	}

	if (!m_socket->waitForBytesWritten()) {
		qDebug() << "Error waiting for bytes to be written: " << m_socket->errorString();
		return false;
	}

	return true;
}

std::tuple<QByteArray, MsgType> TcpClient::receiveMessage() {
	if (!m_socket) {
		qDebug() << "Socket not initialized.";
		return {};
	}

	// QByteArray buffer;
	//
	// // 阻塞等待数据直到接收到完整的包头
	// if (!m_socket->waitForReadyRead(-1)) {
	// 	qDebug() << "waitForReadyRead failed.";
	// 	return {};
	// }
	//
	// // 读取包头
	// MessageHeader header;
	// m_socket->read(reinterpret_cast<char*>(&header), sizeof(MessageHeader));
	//
	// // 转换字节序，获取包头中各项的值
	// auto messageType = static_cast<MsgType>(qFromBigEndian(header.type));
	// const auto messageLength = qFromBigEndian(header.length);
	//
	// // 阻塞等待数据直到接收到完整的消息体
	// while (m_socket->bytesAvailable() < messageLength) {
	// 	if (!m_socket->waitForReadyRead(-1)) {
	// 		qDebug() << "waitForReadyRead failed.";
	// 		return {};
	// 	}
	// }
	//
	// // 读取消息体
	// buffer = m_socket->read(messageLength);
	//
	// return std::make_tuple(buffer, messageType);


	// if (!m_socket->waitForReadyRead(-1)) {
	// 	qDebug() << "waitForReadyRead failed.";
	// 	return {};
	// }
	//
	// QByteArray buffer;
	// MsgType messageType;
	// uint32_t messageLength;
	//
	// while (m_socket->bytesAvailable() >= sizeof(MessageHeader)) {
	// 	MessageHeader header;
	// 	m_socket->peek(reinterpret_cast<char*>(&header), sizeof(MessageHeader));
	//
	// 	// 转换字节序，获取包头中各项的值
	// 	messageType = static_cast<MsgType>(qFromBigEndian(header.type));
	// 	messageLength = qFromBigEndian(header.length);
	//
	// 	// 处理非法包
	// 	if (messageLength <= 0 || messageLength > MAX_PACKAGE_SIZE) {
	// 		return {};
	// 	}
	//
	// 	// 阻塞等待数据直到接收到完整的消息体
	// 	while (m_socket->bytesAvailable() < sizeof(MessageHeader) + messageLength) {
	// 		if (!m_socket->waitForReadyRead(-1)) {
	// 			qDebug() << "waitForReadyRead failed.";
	// 			return {};
	// 		}
	// 	}
	//
	// 	if (m_socket->bytesAvailable() >= sizeof(MessageHeader) + messageLength) {
	// 		// 移动读取位置
	// 		m_socket->read(sizeof(MessageHeader));
	// 		buffer = m_socket->read(messageLength);
	//
	// 		return std::make_tuple(buffer, messageType);
	// 	} else {
	// 		// 如果没有足够的数据可用，等待下一个readyRead信号
	// 		if (!m_socket->waitForReadyRead(-1)) {
	// 			qDebug() << "waitForReadyRead failed.";
	// 			return {};
	// 		}
	// 	}
	// }
	//
	// return {}; // 如果没有足够的数据可用，等待下一个readyRead信号


	if (!m_socket->waitForReadyRead(-1)) {
		qDebug() << "waitForReadyRead failed.";
		return {};
	}

	QByteArray buffer;
	MsgType messageType;
	uint32_t messageLength;

	while (m_socket->bytesAvailable() >= sizeof(MessageHeader)) {
		// 读取消息头
		MessageHeader header;
		m_socket->read(reinterpret_cast<char*>(&header), sizeof(MessageHeader));

		// 转换字节序，获取包头中各项的值
		messageType = static_cast<MsgType>(qFromBigEndian(header.type));
		messageLength = qFromBigEndian(header.length);

		// 处理非法包
		if (messageLength <= 0 || messageLength > MAX_PACKAGE_SIZE) {
			return {};
		}

		// 阻塞等待数据直到接收到完整的消息体
		while (m_socket->bytesAvailable() < messageLength) {
			if (!m_socket->waitForReadyRead(-1)) {
				qDebug() << "waitForReadyRead failed.";
				return {};
			}
		}

		// 读取消息体
		buffer = m_socket->read(messageLength);

		return std::make_tuple(buffer, messageType);
	}

	return {};
}

void TcpClient::closeConnection() {
	if (m_socket) {
		m_socket->close();
		delete m_socket;
		m_socket = nullptr;
	}
}

void TcpClient::readTaskHandler() {
	for (;;) {
		const auto data = TcpClient::receiveMessage();

		QByteArray message;
		MsgType messageType;
		std::tie(message, messageType) = data;

		if (message.isEmpty()) {
			qDebug() << "Error receiving message.";
			continue;
		}

		// Convert QByteArray to std::string
		std::string buffer = message.toStdString();
		// std::cout << buffer << std::endl;

		// Deserialize the received data
		json js = json::parse(buffer);
		// MsgType msgtype{ static_cast<MsgType>(getValueFromJson<int>(js, "msgtype")) };

		if (MsgType::ONE_CHAT_MSG == messageType) {
			auto* targetWidget = WindowManager::getInstance()->findWindowByName(
				QString::number(getValueFromJson<int>(js, "id")));
			const auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);

			if (targetChatWindow) {
				// 连接信号到槽函数
				QObject::connect(this, &TcpClient::messageReceived,
				                 targetChatWindow, &ChatWindow::onRecieveMessage);

				auto msg = getValueFromJson<std::string>(js, "msg");
				auto sentTime = getValueFromJson<std::string>(js, "time");
				auto senderId = getValueFromJson<int>(js, "id");

				// 发射信号
				emit messageReceived(msg.c_str(), sentTime.c_str(), QString::number(senderId));

				// 断开连接，如果需要
				QObject::disconnect(this, &TcpClient::messageReceived,
				                    targetChatWindow, &ChatWindow::onRecieveMessage);
			}

			continue;
		}

		if (MsgType::GROUP_CHAT_MSG == messageType) {
			auto* targetWidget = WindowManager::getInstance()->findWindowByName(
				QString::number(getValueFromJson<int>(js, "groupid")));
			const auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);

			if (targetChatWindow) {
				// 连接信号到槽函数
				QObject::connect(this, &TcpClient::messageReceived,
				                 targetChatWindow, &ChatWindow::onRecieveMessage);

				auto msg = getValueFromJson<std::string>(js, "msg");
				auto sentTime = getValueFromJson<std::string>(js, "time");
				auto senderId = getValueFromJson<int>(js, "id");

				// 发射信号
				emit messageReceived(msg.c_str(), sentTime.c_str(), QString::number(senderId));

				// 断开连接，如果需要
				QObject::disconnect(this, &TcpClient::messageReceived,
				                    targetChatWindow, &ChatWindow::onRecieveMessage);
			}

			continue;
		}

		if (MsgType::LOGIN_MSG_ACK == messageType) {
			doLoginResponse(js);
			rwsem.release();

			continue;
		}

		if (MsgType::REG_MSG_ACK == messageType) {
			doRegResponse(js);
			rwsem.release();

			continue;
		}
	}
}

void TcpClient::doRegResponse(json& responsejs) {
	// Process registration response
	if (0 != responsejs["errno"].get<int>()) {
		std::cerr << "name is already exist, register error!" << std::endl;
	} else {
		std::cout << "name register success, userid is " << responsejs["id"]
			<< ", do not forget it!" << std::endl;
		TcpClient::isRegisterSuccess = true;
	}
}

void TcpClient::doLoginResponse(json& responsejs) {
	if (0 != responsejs["errno"].get<int>()) {
		// 登录失败
		std::cerr << responsejs["errmsg"] << std::endl;
		isLoginSuccess = false;
	} else {
		// 登录成功
		User currentUser;
		currentUser.setId(responsejs["id"].get<int>());
		currentUser.setName(responsejs["name"]);
		currentUser.setHeadImage(responsejs["headimage"]);
		UserManager::setCurrentUser(currentUser);

		// 记录当前用户的好友列表信息
		if (responsejs.contains("friends")) {
			std::vector<User> friends;

			for (auto& str : responsejs["friends"]) {
				std::string receieved = str;
				json js = json::parse(receieved);

				User user;
				user.setId(js["id"].get<int>());
				user.setName(js["name"]);
				user.setState(js["state"]);
				user.setHeadImage(js["headimage"]);
				friends.push_back(std::move(user));
			}

			UserManager::setCurrentUserFriendList(friends);
		}

		// 记录当前用户的群组列表信息
		if (responsejs.contains("groups")) {
			std::vector<Group> groups;

			for (const auto& groupstr : responsejs["groups"]) {
				std::string receieved = groupstr;
				json grpjs = json::parse(receieved);

				Group group;
				group.setId(grpjs["id"].get<int>());
				group.setName(grpjs["groupname"]);
				group.setDesc(grpjs["groupdesc"]);

				std::vector<GroupUser> users;
				for (const auto& userstr : grpjs["users"]) {
					std::string receieved = userstr;
					json js = json::parse(receieved);
					GroupUser user{ js["headimage"], js["id"].get<int>(), js["name"], "", js["state"], js["role"] };
					users.push_back(user);
				}

				group.setUsers(users);
				groups.push_back(group);
			}

			UserManager::setCurrentUserGroupList(groups);
		}

		// 显示当前用户的离线消息  个人聊天信息或者群组消息
		if (responsejs.contains("offlinemsg")) {
			for (const std::vector<std::string>& offlineMessages = responsejs["offlinemsg"]; const auto& str :
			     offlineMessages) {
				if (json js = json::parse(str); MsgType::ONE_CHAT_MSG == static_cast<MsgType>(
					getValueFromJson<int>(js, "msgtype"))) {
					std::cout << getValueFromJson<std::string>(js, "time") << " [" << getValueFromJson<int>(js, "id")
						<< "]" << getValueFromJson<std::string>(js, "name") << " said: "
						<< getValueFromJson<std::string>(js, "msg") << std::endl;
				} else {
					std::cout << "群消息[" << getValueFromJson<int>(js, "groupid") << "]:"
						<< getValueFromJson<std::string>(js, "time") << " [" << getValueFromJson<int>(js, "id")
						<< "]" << getValueFromJson<std::string>(js, "name") << " said: "
						<< getValueFromJson<std::string>(js, "msg") << std::endl;
				}
			}
		}

		isLoginSuccess = true;
	}
}

std::vector<char> TcpClient::constructMessage(const nlohmann::json& js, MsgType msgtype) {
	// 构造包头
	MessageHeader header;
	header.type = qToBigEndian(static_cast<uint16_t>(msgtype));
	header.length = qToBigEndian(static_cast<uint32_t>(js.dump().size()));

	// 合并包头和消息为一个连续的字节数组
	std::vector<char> messageData(sizeof(header) + js.dump().size());

	std::memcpy(messageData.data(), &header, sizeof(header));
	std::memcpy(messageData.data() + sizeof(header), js.dump().c_str(), js.dump().size());

	return messageData;
}

void TcpClient::onReadyRead() {
	// const auto data = TcpClient::receiveMessage();
	//
	// QByteArray message;
	// MsgType messageType;
	// std::tie(message, messageType) = data;
	//
	// if (message.isEmpty()) {
	// 	qDebug() << "Error receiving message.";
	// }
	//
	// // Convert QByteArray to std::string
	// std::string buffer = message.toStdString();
	// // std::cout << buffer << std::endl;
	//
	// // Deserialize the received data
	// json js = json::parse(buffer);
	// // MsgType msgtype{ static_cast<MsgType>(getValueFromJson<int>(js, "msgtype")) };
	//
	// if (MsgType::ONE_CHAT_MSG == messageType) {
	// 	auto* targetWidget = WindowManager::getInstance()->findWindowByName(
	// 		QString::number(getValueFromJson<int>(js, "id")));
	// 	const auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);
	//
	// 	if (targetChatWindow) {
	// 		// 连接信号到槽函数
	// 		QObject::connect(this, &TcpClient::messageReceived,
	// 		                 targetChatWindow, &ChatWindow::onRecieveMessage);
	//
	// 		auto msg = getValueFromJson<std::string>(js, "msg");
	// 		auto sentTime = getValueFromJson<std::string>(js, "time");
	//
	// 		// 发射信号
	// 		emit messageReceived(msg.c_str(), sentTime.c_str());
	//
	// 		// 断开连接，如果需要
	// 		QObject::disconnect(this, &TcpClient::messageReceived,
	// 		                    targetChatWindow, &ChatWindow::onRecieveMessage);
	// 	}
	// }
	//
	// if (MsgType::GROUP_CHAT_MSG == messageType) {
	// 	auto* targetWidget = WindowManager::getInstance()->findWindowByName(
	// 		QString::number(getValueFromJson<int>(js, "groupid")));
	// 	const auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);
	//
	// 	if (targetChatWindow) {
	// 		// 连接信号到槽函数
	// 		QObject::connect(this, &TcpClient::messageReceived,
	// 		                 targetChatWindow, &ChatWindow::onRecieveMessage);
	//
	// 		auto msg = getValueFromJson<std::string>(js, "msg");
	// 		auto sentTime = getValueFromJson<std::string>(js, "time");
	//
	// 		// 发射信号
	// 		emit messageReceived(msg.c_str(), sentTime.c_str());
	//
	// 		// 断开连接，如果需要
	// 		QObject::disconnect(this, &TcpClient::messageReceived,
	// 		                    targetChatWindow, &ChatWindow::onRecieveMessage);
	// 	}
	// }
	//
	// if (MsgType::LOGIN_MSG_ACK == messageType) {
	// 	doLoginResponse(js);
	// 	rwsem.release();
	// }
	//
	// if (MsgType::REG_MSG_ACK == messageType) {
	// 	doRegResponse(js);
	// 	rwsem.release();
	// }
}
