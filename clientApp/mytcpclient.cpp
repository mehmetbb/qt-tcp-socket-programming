#include "mytcpclient.h"


MyTcpClient::MyTcpClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(this, &MyTcpClient::newMessage, this, &MyTcpClient::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpClient::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpClient::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MyTcpClient::displayError);

    socket->connectToHost(QHostAddress::LocalHost,9999);

    if(socket->waitForConnected())
    {
        // If client connected to server this message appears
        qDebug() << "---------------------------\n"
                    "|| WELCOME TO THE PORTAL ||\n"
                    "---------------------------\n";
    }
    else
    {
        qDebug() << "Couldn't connect to Server: " << socket->errorString();
        exit(EXIT_FAILURE);
    }
}

MyTcpClient::~MyTcpClient()
{
    if(socket->isOpen())
        socket->close();
}

void MyTcpClient::readSocket()
{
    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_2);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = "Waiting for more data to come..";
        emit newMessage(message);
        return;
    }

    buffer = buffer.mid(128);

    QString message = QString::fromStdString(buffer.toStdString());
    emit newMessage(message);

    analyzeMessage(message);
}


void MyTcpClient::analyzeMessage(QString message)
{
    QStringList data = message.split(":");

    if(data[0]=="auth")
    {
        if(data[1]=="successful")
        {
            qDebug() << "Login successful!\n";
            //kullanıcı bilgileri gönderilecek
        }
        else if(data[1]=="failed")
        {
            qDebug() << "FAILED: username or password is wrong!\n";
            login();
        }
    }
    else
        qDebug() << "FAILED: message couldnt understand!\n";

}


void MyTcpClient::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;

    qDebug() << "Disconnected!";
}

void MyTcpClient::displayError(QAbstractSocket::SocketError socketError)
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
            qDebug() << "The following error occurred: " << socket->errorString();
        break;
    }
}


void MyTcpClient::sendMessage(const QString &str)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            //QString str = ui->lineEdit_message->text()

            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_2);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);

            socketStream << byteArray;

            //ui->lineEdit_message->clear();
        }
        else
            qDebug() << "Socket doesn't seem to be opened";
    }
    else
        qDebug() << "Not connected";
}

// Login to server to receive data
void MyTcpClient::login()
{
    if(socket->isOpen())
    {
        QTextStream qtin(stdin);       

        qDebug() << "Please enter your username:";
        QString username = qtin.readLine();

        qDebug() << "Please enter your password:";
        QString password = qtin.readLine();

        // Concatanation username and password
        QString userpass{"userpass:"};  // this is for analyzing message by server
        userpass.reserve(userpass.length() + username.length() + password.length());
        userpass.append(username);
        userpass.append(":");
        userpass.append(password);

        sendMessage(userpass);

    }
    else
    {
        qDebug() << "Socket doesn't seem to be opened";
    }
}



void MyTcpClient::displayMessage(const QString& str)
{
    //ui->textBrowser_receivedMessages->append(str);

    qDebug() << str;
}


/*

void MyTcpClient::setSocket(qintptr descriptor)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    qDebug() << "Client connected to x..";
}


void MyTcpClient::connected()
{
    qDebug() << "Client connected to event..";
}


void MyTcpClient::disconnected()
{
    qDebug() << "Client disconnected from y..";
}


void MyTcpClient::readyRead()
{
    qDebug() << socket->readAll();

    qDebug() << "Please enter the password:";

    QTextStream qtin(stdin);
    QString password = qtin.readLine();  // This is how you read the entire line

    qDebug() << password;

    socket->write("heyyy");
    socket->flush();
}




void MyTcpClient::connectToServer()
{
    qDebug() << "------------------------------";
    qDebug() << "Please enter your username:";

    QTextStream qtin(stdin);
    QString username = qtin.readLine();  // This is how you read the entire line

    qDebug() << "Username: " << username;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_2);

    socketStream << username;

    if(socket->isOpen())
    {
        QString str = "123dfdfdgdgd";

        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_6_2);

        socketStream << str;

        socket->write("username");
        socket->flush();

        qDebug() << "sdf???";

        QDataStream clientStream(socket);
        clientStream.setVersion(QDataStream::Qt_6_2);

        qDebug() << "adm";
        clientStream << str;
    }
    else
    {
        qDebug() << "Socket doesn't seem to be opened";
    }
}

void MyTcpClient::setSocket(qintptr descriptor)
{
    // make a new socket
    socket = new QTcpSocket(this);

    qDebug() << "A new socket created!";

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    //qDebug() << "Client connected at " << descriptor;

    socket->connectToHost(QHostAddress("127.0.0.1"),9999);

        if(socket->waitForConnected())
            qDebug() << "Connected to Server";
        else{
            qDebug() << "The following error occurred: " << socket->errorString();
        }
}

*/
