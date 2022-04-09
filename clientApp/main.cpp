#include <QCoreApplication>
#include <mytcpclient.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpClient client;

    client.login();

    return a.exec();

}
