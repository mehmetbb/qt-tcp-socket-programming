#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "mytcpserver.h"
#include "customer.h"




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Server started
    MyTcpServer server;


    return a.exec();
}
