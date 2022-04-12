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
        qDebug() << "*** Couldn't connect to Server: " << socket->errorString();
        exit(EXIT_FAILURE);
    }
}

// destructor
MyTcpClient::~MyTcpClient()
{
    if(socket->isOpen())
        socket->close();
}

// The client listens to the channel
// and catches when new messages arrive
void MyTcpClient::readSocket()
{
    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_2);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        QString message = "*** Waiting for data..";
        emit newMessage(message);
        return;
    }

    buffer = buffer.mid(128);

    QString message = QString::fromStdString(buffer.toStdString());
    //emit newMessage(message);  *for trial*

    // to make the incoming message understandable
    analyzeMessage(message);
}

// Analysis mechanism is used to understand
// messages between server and client.
void MyTcpClient::analyzeMessage(QString message)
{
    QStringList data = message.split(":");

    // authorization check
    if(data[0]=="auth")
    {
        if(data[1]=="successful")
        {
            qDebug() << "\n*** Login successful!\n\n"
                     << "---------------------------------------------------\n"
                     << "Welcome to" << data[3] << "online operations.\n"
                     << "------------------------------------------------\n"
                     << "Username:   " << data[2] << "\n"
                     << "Customer no:" << data[4] << "\n"
                     << "Balance:    " << data[5] << "\n"
                     << "-----------------------------\n";

            operations(data[2],data[5]);

        }
        else if(data[1]=="failed")
        {
            qDebug() << "\n*** FAILED: username or password is wrong!\n";
            login();
        }
    }
    // if error occurs when transfer money
    else if(data[0]=="transfererror")
    {
        if(data[1]=="notexist")
        {
            qDebug() << "\n*** User" << data[2] << "does not exist!\n";

            operations(data[3],data[4]);

        }
        else if(data[1]=="wrongnumber")
        {
            qDebug() << "\n*** There is a mismatch in the customer name and number!\n";

            operations(data[2],data[3]);
        }
    }
    // operation successful message
    else if(data[0]=="completed")
    {
        qDebug() << "\n*** Operation successful!\n"
                 << "-----------------------------\n"
                 << "Username:   " << data[1] << "\n"
                 << "Balance:    " << data[2] << "\n"
                 << "-----------------------------\n";

        operations(data[1],data[2]);
    }
    else
        qDebug() << "*** FAILED: message could not understand!\n";

}

// selected operations are evaluated
void MyTcpClient::operations(QString username, QString balance)
{
    operation:

    QTextStream qtin(stdin);

    qDebug() << "Which operation you want to perform?\n"
             << "Enter 1 to deposit money:\n"
             << "Enter 2 to withdraw money:\n"
             << "Enter 3 to transfer money:\n"
             << "Enter 4 to sign out:\n"
             << "Enter 5 to disconnect:";

    QString choice = qtin.readLine();

    // add money
    if(choice=="1")
    {
        qDebug() << "\nPlease enter amount to deposit:";

        QString amount = qtin.readLine();

        QString operation{"operation:"};  // this is for analyzing message by server
        operation.append(choice);
        operation.append(":");
        operation.append(username);
        operation.append(":");
        operation.append(amount);

        sendMessage(operation);
    }
    // withdraw money
    else if(choice=="2")
    {
        qDebug() << "\nPlease enter amount to withdraw:";

        QString amount = qtin.readLine();

        double amountF = amount.toFloat();
        double balanceF = balance.toFloat();

        while(amountF>balanceF)
        {
            qDebug() << "\n*** There is no enough balance.\n"
                        "Current balance: " << balance << "\n";
            goto operation;
        }

        QString operation{"operation:"};  // this is for analyzing message by server
        operation.append(choice);
        operation.append(":");
        operation.append(username);
        operation.append(":");
        operation.append(amount);

        sendMessage(operation);
    }
    // transfer money
    else if(choice=="3")
    {
        start:

        qDebug() << "\nPlease enter the 'CUSTOMER NO' of the person to transfer:";
        QString customto = qtin.readLine();

        qDebug() << "Please enter the 'USERNAME' of the person to transfer:";
        QString userto = qtin.readLine();

        if(userto==username)
        {
            qDebug() << "\n*** Not allowed transfer money to your own account!\n";
            goto start;
        }

        transfer:

        qDebug() << "Please enter amount to transfer:";
        QString amount = qtin.readLine();

        // to compare variables we need to convert to float
        double amountF = amount.toFloat();
        double balanceF = balance.toFloat();

        if(amountF>balanceF)
        {
            qDebug() << "\n*** There is no enough balance.";
            qDebug() << "*** Current balance: " << balanceF << "\n";
            goto transfer;
        }

        QString operation{"operation:"};  // this is for analyzing message by server
        operation.append(choice);
        operation.append(":");
        operation.append(username);
        operation.append(":");
        operation.append(amount);
        operation.append(":");
        operation.append(userto);
        operation.append(":");
        operation.append(customto);
        operation.append(":");
        operation.append(balance);

        sendMessage(operation);
    }
    // sign out
    else if(choice=="4")
    {
        qDebug() << "\nClient signed out!\n"
                    "Please login again!\n";
        login();
    }
    // disconnect
    else if(choice=="5")
    {
        socket -> close();
    }
    else
    {
        qDebug() << "\n*** Please enter a valid entry:\n";
        goto operation;
    }

}

// when the connection lost
void MyTcpClient::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;

    qDebug() << "\n*** Disconnected!";
}


// error control
void MyTcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "*** The host was not found.";
        break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "*** The connection was refused.";
        break;
        default:
            qDebug() << "*** The following error occurred: " << socket->errorString();
        break;
    }
}


// send message to server
void MyTcpClient::sendMessage(const QString &str)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_2);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);

            socketStream << byteArray;
        }
        else
            qDebug() << "*** Socket could not be opened!";
    }
    else
        qDebug() << "*** Not connected!";
}

// login to server
void MyTcpClient::login()
{
    if(socket->isOpen())
    {
        QTextStream qtin(stdin);       

        qDebug() << "Please enter your username:";
        QString username = qtin.readLine();

        qDebug() << "Please enter your password:";
        QString password = qtin.readLine();

        // concatanation username and password
        // this is for analyzing message by server
        QString userpass{"userpass:"};
        userpass.reserve(userpass.length() + username.length() + password.length());
        userpass.append(username);
        userpass.append(":");
        userpass.append(password);

        sendMessage(userpass);

    }
    else
    {
        qDebug() << "*** Socket could not be opened!";
    }
}


void MyTcpClient::displayMessage(const QString& str)
{
    qDebug() << str;
}

