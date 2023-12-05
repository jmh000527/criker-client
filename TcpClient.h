#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <semaphore>

#include "thirdparty/include/nlohmann/json.hpp"


class TcpClient : public QObject {
    Q_OBJECT

public:
    static bool establishConnection(const QString& ipAddress, quint16 portNumber);
    static bool sendMessage(const QString& message);
    static QByteArray receiveMessage();
    static void closeConnection();

    static void readTaskHandler();

    static void doRegResponse(nlohmann::json& responsejs);
    static void doLoginResponse(nlohmann::json& responsejs);

    static std::atomic_bool isLoginSuccess;
    static std::counting_semaphore<1> rwsem;

private:
    static QTcpSocket* m_socket;
};
