#include <QMessageBox>
#include <thread>

#include "CCMainWindow.h"
#include <QtWidgets/QApplication>

#include "TcpClient.h"
#include "UserLogin.h"

inline void EnableMemLeakCheck()
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

//可以定位到发生内存泄露 所在的文件和具体那一行，用于检测 malloc 分配的内存
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>

//把分配内存的信息保存下来，可以定位到那一行发生了内存泄露。用于检测 new 分配的内存
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int main(int argc, char *argv[])
{
    

    QApplication a(argc, argv);
    // CCMainWindow w;
    // w.show();

    QApplication::setQuitOnLastWindowClosed(false);

    // std::thread readTask(TcpClient::readTaskHandler);
    // readTask.detach();

    // EnableMemLeakCheck();
    // _CrtSetBreakAlloc(282682);

    UserLogin* userLogin{ new UserLogin };
    userLogin->show();
    // _CrtDumpMemoryLeaks();

    a.exec();

    return 0;
}
