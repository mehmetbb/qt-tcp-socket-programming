#include <QCoreApplication>
#include "mytcpserver.h"

QString MyTcpServer::myText;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;

    server.startServer();

    server.readFile();

    return a.exec();
}
