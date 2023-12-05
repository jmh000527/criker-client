#include <QMessageBox>
#include <thread>

#include "CCMainWindow.h"
#include <QtWidgets/QApplication>

#include "TcpClient.h"
#include "UserLogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // CCMainWindow w;
    // w.show();

    QApplication::setQuitOnLastWindowClosed(false);

    // std::thread readTask(TcpClient::readTaskHandler);
    // readTask.detach();

    UserLogin* userLogin{ new UserLogin };
    userLogin->show();

    return a.exec();
}
