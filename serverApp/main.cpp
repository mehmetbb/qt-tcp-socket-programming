#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "mytcpserver.h"




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    // Create server object
    MyTcpServer server;

    // check and read file
    server.readFile();

    return a.exec();
}
