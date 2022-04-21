#include "mytcpserver.h"
#include "mythread.h"


QString filename;


MyTcpServer::MyTcpServer(QObject *parent) :
    QTcpServer(parent)
{

}

// to start server
void MyTcpServer::startServer()
{
    if(this->listen(QHostAddress::Any,9999))
    {
        qDebug() << ">> Server started!\n";
    }
    else
    {
        qDebug() << ">> Could not start server!";
    }
}

// thread is created on every incoming connection (multi-threading)
void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << "-> connecting..";
    MyThread *thread = new MyThread(socketDescriptor,this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}


// reading the txt file which containing the data
void MyTcpServer::readFile()
{
    QDir dir;
    QTextStream qtin(stdin);

    filename = dir.currentPath();

    QStringList data = filename.split("/");

    filename = "";

    for(int i=0; i<(data.size()-2); i++)
    {
        filename.append(data[i]).append("/");
    }

    filename.append("customerinfo.txt");

    qDebug() << "Default directory:\n" << filename;

    check:
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "\nFile 'customerinfo.txt' can not found in the directory!"
                 << "\nPlease paste the directory where the 'customerinfo.txt' file is located:\n";

        filename = qtin.readLine();

        goto check;
    }
    else
    {
        QTextStream in(&file);
        myText = in.readAll(); // myText contains all customer info

        file.close();


        qDebug() << "\nOn the client side, you can use information below.\n"
                    "This template contains all customer info:\n"
                    "-------------------------------------";

        QStringList dataLine = myText.split("\n");

        for(int i = 0; i < dataLine.size(); i++)
        {
             QStringList data = dataLine[i].split(":");

             qDebug() << "Username:    " << data[0] << "\n"
                         "Password:    " << data[3] << "\n"
                         "Customer no: " << data[2] << "\n"
                         "Bank:        " << data[1] << "\n"
                         "Balance:     " << data[4] << "\n"
                         "-------------------------------------";
        }

        qDebug() << "\nOn the client side, you can use information above.\n"
                    "This template is for ease of use. Changes can be checked on the client side.\n\n"
                    ">> Data received. Client can be started!";
    }

}


// applying changes to the file / db
void MyTcpServer::writeFile(QString message)
{
    qDebug() << "\n>> Writing to db..";

    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << ">> Could not open file for writing!";
        return;
    }

    QTextStream out(&file);
    out << message;
    file.flush();
    file.close();

    qDebug() << ">> Completed!\n";

    myText = message;
}

