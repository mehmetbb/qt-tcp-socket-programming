#include "mytcpserver.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>

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
    //ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
    qDebug() << "INFO :: Client connected!";
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
        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
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


void MyTcpServer::analyzeMessage(QString message)
{
    QStringList data = message.split(":");

    if(data[0]=="userpass")
    {
        QString user = data[1];
        QString pass = data[2];

        checkAuthorization(user, pass);

        //foreach (QTcpSocket* socket,connection_set)
        //{sendMessage(socket, "server >> i got it!");}
    }

}


void MyTcpServer::checkAuthorization(QString user, QString pass)
{
    // Get user info from txt file
    QFile file("C:\\Users\\poseidon\\Desktop\\customerinfo.txt");

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return;
    }

    QTextStream in(&file);
    QString myText = in.readAll();  // myText contains all user info

    file.close();

    //QString myText = "mehmet:Ziraat Bank:3001:pass123:6900,82\nfatihyenilmez:Ziraat Bank:3002:pass123:5600,17";

    if(!myText.contains(user))
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendMessage(socket, "auth:failed");
        }
    }

    QStringList dataLine = myText.split("\n");
    QStringList dataSplitted;

    for(int i=0; i<dataLine.size();i++)
    {
        if(dataLine[i].contains(user))
        {
            dataSplitted = dataLine[i].split(":");

            if(user==dataSplitted[0] && pass==dataSplitted[3])
            {
                foreach (QTcpSocket* socket,connection_set)
                {
                    QString authMessage{"auth:successful"};
                    authMessage.append(":").append(dataSplitted[0])
                               .append(":").append(dataSplitted[1])
                               .append(":").append(dataSplitted[2])
                               .append(":").append(dataSplitted[4]);

                    sendMessage(socket, authMessage);
                }
            }
            else
            {
                foreach (QTcpSocket* socket,connection_set)
                {
                    sendMessage(socket, "auth:failed");
                }
            }
            break;
        }
    }
}


void MyTcpServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        qDebug() << "INFO :: A client has left!";
        connection_set.remove(*it);
    }
    //refreshComboBox();

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

void MyTcpServer::on_pushButton_sendMessage_clicked()
{
    //QString receiver = ui->comboBox_receiver->currentText();

    QString receiver = "deneme123";

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendMessage(socket, receiver);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendMessage(socket, receiver);
                break;
            }
        }
    }
    //ui->lineEdit_message->clear();
}


void MyTcpServer::sendMessage(QTcpSocket* socket, QString message)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            //QString str = ui->lineEdit_message->text();

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


void MyTcpServer::displayMessage(const QString& str)
{
    //ui->textBrowser_receivedMessages->append(str);

    qDebug() << str;
}
