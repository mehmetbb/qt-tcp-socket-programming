#include "mytcpserver.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>

QString customerList[7][5];

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(m_server->listen(QHostAddress::Any, 9999))
    {
        connect(this, &MyTcpServer::newMessage, this, &MyTcpServer::displayMessage);
        connect(m_server, &QTcpServer::newConnection, this, &MyTcpServer::newConnection);
        qDebug() << "Server is started!";
    }
    else
    {
        qDebug() << "Server is not started. " << m_server->errorString();
        exit(EXIT_FAILURE);
    }
}


MyTcpServer::~MyTcpServer()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();
}


void MyTcpServer::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}


void MyTcpServer::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MyTcpServer::displayError);

    qDebug() << "Client connected!";
}


void MyTcpServer::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_2);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = "Waiting for data..";
        emit newMessage(message);
        return;
    }

    //QString header = buffer.mid(0,128);

    buffer = buffer.mid(128);

    //QString message = QString("Message :: %1").arg(QString::fromStdString(buffer.toStdString()));
    QString message = QString::fromStdString(buffer.toStdString());

    emit newMessage(message);
    //qDebug() << message;

    analyzeMessage(message);
}

// Analysis mechanism is used to understand
// messages between server and client.
void MyTcpServer::analyzeMessage(QString message)
{
    QStringList data = message.split(":");

    if(data[0]=="userpass")
    {
        QString user = data[1];
        QString pass = data[2];

        checkAuthorization(user, pass);
    }
    // Add money
    else if(data[0]=="operation" && data[1]=="1")
    {
        for(int i=0;i<7;i++)
        {
            if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double addMoney = data[3].toFloat();

                currentBalance += addMoney;

                customerList[i][4] = QString::number(currentBalance);

                writeFile();

                break;
            }
        }
    }
    // Withdraw money
    else if(data[0]=="operation" && data[1]=="2")
    {
        for(int i=0;i<7;i++)
        {
            if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double getMoney = data[3].toFloat();

                currentBalance -= getMoney;

                customerList[i][4] = QString::number(currentBalance);

                writeFile();

                break;
            }
        }
    }
    //"operation:3:poyrazkarayel:45:aliaydin:1001"
    // Transfer money
    else if(data[0]=="operation" && data[1]=="3")
    {
        // böyle bir müşteri var mı nosu doğru mu

        QString bankTo;
        QString bankFrom;
        double bankCut = 6;

        for(int i=0;i<7;i++)
        {
            if(data[4]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double addMoney = data[3].toFloat();

                currentBalance += addMoney;

                customerList[i][4] = QString::number(currentBalance);

                bankTo = customerList[i][1];

            }
            else if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double getMoney = data[3].toFloat();

                currentBalance -= getMoney;

                bankFrom = customerList[i][1];

                // When banks are different
                if(bankFrom!=bankTo)
                    currentBalance -= bankCut;

                customerList[i][4] = QString::number(currentBalance);

                writeFile();
            }
        }
    }

}


void MyTcpServer::readFile()
{
    QString filename = "C:\\Users\\poseidon\\Desktop\\customerinfo.txt";

    QFile file(filename);
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << "Could not open the file for reading";
        return;
    }

    QTextStream in(&file);
    QString myText = in.readAll(); // myText contains all customer info
    file.close();

    qDebug() << myText;

    QStringList dataLine = myText.split("\n");

    for(int i = 0; i < dataLine.size(); i++)
    {
         QStringList data = dataLine[i].split(":");

         for(int j = 0; j < data.size(); j++)
         {
             customerList[i][j] = data[j];
         }
    }
}


void MyTcpServer::writeFile()
{

    qDebug() << "write file";


    QString temp = "";

    for(int i=0;i<7;i++)
    {
        for(int j=0;j<5;j++)
        {
            temp.append(customerList[i][j]);

            if(j!=4)
            {
                temp.append(":");
            }
        }
        if(i!=6)
        {
            temp.append("\n");
        }
    }

    qDebug() << "Temp: " << temp;


/*


    QString filename = "C:\\Users\\poseidon\\Desktop\\customerinfo.txt";

    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    // To write text, we use operator<<(),
    // which is overloaded to take
    // a QTextStream on the left
    // and data types (including QString) on the right

    QTextStream out(&file);
    out << "QFile\nTuto\nrial";
    file.flush();
    file.close(); */
}


void MyTcpServer::checkAuthorization(QString user, QString pass)
{ 
    bool exist = false;
    for(int k=0;k<7;k++)
    {
        if(user==customerList[k][0] && pass==customerList[k][3])
        {
            foreach (QTcpSocket* socket,connection_set)
            {
                QString authMessage{"auth:successful"};
                authMessage.append(":").append(customerList[k][0])
                           .append(":").append(customerList[k][1])
                           .append(":").append(customerList[k][2])
                           .append(":").append(customerList[k][4]);

                sendMessage(socket, authMessage);
            }
            exist = true;
            break;
        }
    }

        if(!exist)
        {
            foreach (QTcpSocket* socket,connection_set)
            {
                sendMessage(socket, "auth:failed");
            }
        }  
}


void MyTcpServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        qDebug() << "Client has left!";
        connection_set.remove(*it);
    }

    socket->deleteLater();
}


void MyTcpServer::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "The host was not found.";
        break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "The connection was refused.";
        break;
        default:
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            qDebug() << "The following error occurred: " << socket->errorString();
        break;
    }
}


void MyTcpServer::sendMessage(QTcpSocket* socket, QString message)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_2);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(message.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = message.toUtf8();
            byteArray.prepend(header);

            socketStream.setVersion(QDataStream::Qt_6_2);
            socketStream << byteArray;
        }
        else
            qDebug() << "Socket doesn't seem to be opened";
    }
    else
        qDebug() << "Not connected";
}


void MyTcpServer::displayMessage(QString str)
{
    qDebug() << str;
}


