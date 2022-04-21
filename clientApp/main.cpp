#include <QCoreApplication>
#include <mytcpclient.h>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // create client object
    MyTcpClient client;

    // display login screen
    client.login();

    return a.exec();

}
