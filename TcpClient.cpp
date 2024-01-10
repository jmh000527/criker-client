#include "TcpClient.h"

#include <iostream>
#include <QByteArray>

#include "Group.h"
#include "MsgType.h"
#include "User.h"
#include "UserManager.h"
#include "WindowManager.h"

using namespace nlohmann;

QTcpSocket* TcpClient::m_socket{};
std::atomic_bool TcpClient::isLoginSuccess{ false };
std::counting_semaphore<1> TcpClient::rwsem(0);

TcpClient* TcpClient::getInstance() {
	static TcpClient instance; // 在首次调用时创建唯一实例
	return &instance;
}

bool TcpClient::establishConnection(const QString& ipAddress, quint16 portNumber) {
	m_socket = new QTcpSocket();
	m_socket->connectToHost(ipAddress, portNumber);
	if (!m_socket->waitForConnected()) {
		qDebug() << "Error: " << m_socket->errorString();
		delete m_socket;
		m_socket = nullptr;
		return false;
	}

	return true;
}

bool TcpClient::sendMessage(const QString& message) {
	if (!m_socket) {
		qDebug() << "Socket not initialized.";
		return false;
	}

	QByteArray data = message.toUtf8();
	qint64 len = m_socket->write(data);

	if (len == -1) {
		qDebug() << "Error sending message: " << m_socket->errorString();
		return false;
	}

	if (!m_socket->waitForBytesWritten()) {
		qDebug() << "Error waiting for bytes to be written: " << m_socket->errorString();
		return false;
	}

	return true;
}

QByteArray TcpClient::receiveMessage() {
	if (!m_socket) {
		qDebug() << "Socket not initialized.";
		return QByteArray();
	}

	QByteArray buffer;
	if (m_socket->waitForReadyRead(-1)) {
		buffer = m_socket->readAll();
	}

	return buffer;
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
		QByteArray data = TcpClient::receiveMessage();
		if (data.isEmpty()) {
			qDebug() << "Error receiving message.";
			continue;
		}

		// Convert QByteArray to std::string
		std::string buffer = data.toStdString();
		// std::cout << buffer << std::endl;

		// Deserialize the received data
		json js = json::parse(buffer);
		MsgType msgtype{ static_cast<MsgType>(getValueFromJson<int>(js, "msgtype")) };

		if (MsgType::ONE_CHAT_MSG == msgtype) {
			auto* targetWidget = WindowManager::getInstance()->findWindowByName(QString::number(getValueFromJson<int>(js, "id")));
			auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);

			if (targetChatWindow) {
				// 连接信号到槽函数
				QObject::connect(this, &TcpClient::messageReceived,
								 targetChatWindow, &ChatWindow::onRecieveMessage);

				auto msg = getValueFromJson<std::string>(js, "msg");
				auto sentTime = getValueFromJson<std::string>(js, "time");

				// 发射信号
				emit messageReceived(msg.c_str(), sentTime.c_str());

				// 断开连接，如果需要
				QObject::disconnect(this, &TcpClient::messageReceived,
									targetChatWindow, &ChatWindow::onRecieveMessage);
			}

			continue;
		}

		if (MsgType::GROUP_CHAT_MSG == msgtype) {
			auto* targetWidget = WindowManager::getInstance()->findWindowByName(QString::number(getValueFromJson<int>(js, "groupid")));
			auto* targetChatWindow = dynamic_cast<ChatWindow*>(targetWidget);

			if (targetChatWindow) {
				// 连接信号到槽函数
				QObject::connect(this, &TcpClient::messageReceived,
								 targetChatWindow, &ChatWindow::onRecieveMessage);

				auto msg = getValueFromJson<std::string>(js, "msg");
				auto sentTime = getValueFromJson<std::string>(js, "time");

				// 发射信号
				emit messageReceived(msg.c_str(), sentTime.c_str());

				// 断开连接，如果需要
				QObject::disconnect(this, &TcpClient::messageReceived,
									targetChatWindow, &ChatWindow::onRecieveMessage);
			}

			continue;
		}

		if (MsgType::LOGIN_MSG_ACK == msgtype) {
			doLoginResponse(js);
			rwsem.release();

			continue;
		}

		if (MsgType::REG_MSG_ACK == msgtype) {
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
					GroupUser user{ js["id"].get<int>(), js["name"], "", js["state"], js["role"] };
					users.push_back(user);
				}

				group.setUsers(users);
				groups.push_back(group);
			}

			UserManager::setCurrentUserGroupList(groups);
		}

		// 显示登录用户的基本信息
		// showCurrentUserData();

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
