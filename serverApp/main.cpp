#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;

/*
    QTextStream s(stdin);
    QString value = s.readLine();
    server.writeData(value);
*/
    return a.exec();
}
