#include <QCoreApplication>
#include <mytcpclient.h>
#include <iostream>

// git control

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpClient client;

    client.login();

    return a.exec();

}
