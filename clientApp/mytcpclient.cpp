#include "mytcpclient.h"


MyTcpClient::MyTcpClient(QObject *parent) :
    QObject(parent)
{
    // new socket created
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
        qDebug() << ">> Couldn't connect to Server: " << socket->errorString();
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
        QString message = ">> Waiting for data..";
        emit newMessage(message);
        return;
    }

    buffer = buffer.mid(128);

    QString message = QString::fromStdString(buffer.toStdString());

    //emit newMessage(message);  //to suppress the message

    // to make the incoming message understandable
    analyzeMessage(message);
}


// Analysis mechanism is used to understand
// messages between server and client.
void MyTcpClient::analyzeMessage(QString message)
{
    QStringList data = message.split(":");
    QTextStream qtin(stdin);

    int dataSize = data.size();

    // authorization check
    if(data[0]=="auth")
    {
        if(data[1]=="successful")
        {
            qDebug() << "\n---------------------------------------------------\n"
                     << "Welcome to" << data[3] << "online operations.\n"
                     << "------------------------------------------------\n"
                     << "Username:   " << data[2] << "\n"
                     << "Customer no:" << data[4] << "\n"
                     << "Balance:    " << data[5] << "\n"
                     << "-----------------------------\n";

            QString forOperation = data[2].append(":").append(data[5])
                                          .append(":").append(data[3])
                                          .append(":").append(data[4]);


            operations(forOperation);

        }
        else if(data[1]=="failed")
        {
            qDebug() << "\n>> FAILED: username or password is wrong!\n";
            login();
        }
    }
    // if error occurs when transfer money
    else if(data[0]=="transfererror")
    {
        if(data[1]=="notexist")
        {
            qDebug() << "\n>> User" << data[6] << "does not exist!\n";

            notexist:
            qDebug() << "Press 0 to go back:";

            QString choice = qtin.readLine();

            if(choice=="0")
            {
                QString back = "auth:successful:";

                back.append(data[2]).append(":")  // username
                    .append(data[3]).append(":")  // bank
                    .append(data[4]).append(":")  // customer no
                    .append(data[5]);             // balance

                analyzeMessage(back);

                return;
            }
            else
            {
                qDebug() << "\nPlease enter a valid entry.";
                goto notexist;
            }

        }
        else if(data[1]=="wrongnumber")
        {
            qDebug() << "\n>> There is a mismatch in the customer name and number!\n";
            wrongnum:
            qDebug() << "Press 0 to go back:";

            QString choice = qtin.readLine();

            if(choice=="0")
            {
                QString back = "auth:successful:";

                back.append(data[2]).append(":")  // username
                    .append(data[3]).append(":")  // bank
                    .append(data[4]).append(":")  // customer no
                    .append(data[5]);             // balance

                analyzeMessage(back);

                return;
            }
            else
            {
                qDebug() << "\nPlease enter a valid entry.";
                goto wrongnum;
            }
        }
    }
    // operation successful message
    else if(data[0]=="completed")
    {
        qDebug() << "\n>> Operation successful!";

        if(dataSize==6)
            qDebug() << "\n>> 6 TL deduction fee is charged between different banks.";

        goback:
        qDebug() << "\nPress 0 to go back:";

        QString choice = qtin.readLine();

        if(choice=="0")
        {
            QString back = "auth:successful:";

            back.append(data[1]).append(":")
                .append(data[2]).append(":")
                .append(data[3]).append(":")
                .append(data[4]);

            analyzeMessage(back);

            return;
        }
        else
        {
            qDebug() << "\nPlease enter a valid entry.";
            goto goback;
        }
    }
    else
        qDebug() << ">> FAILED: message error! Please restart the application.\n";

}

// to select operation
void MyTcpClient::operations(QString userData)
{
    QStringList strList = userData.split(":");

    QTextStream qtin(stdin);

    qDebug() << "Which operation you want to perform?\n"
             << "Press 1 to deposit money:\n"
             << "Press 2 to withdraw money:\n"
             << "Press 3 to transfer money:\n"
             << "Press 4 to sign out:\n"
             << "Press 5 to disconnect:";

    QString choice = qtin.readLine();

    // add money
    if(choice=="1")
    {
        posit:
        qDebug() << "\nPlease enter amount to deposit:";

        QString amount = qtin.readLine();

        double amountFl = amount.toFloat();

        if(amountFl<=0)
        {
            qDebug() << "\n>> Input should be positive.";
            goto posit;
        }

        QString operation{"operation:"};  // this is for analyzing message by server
        operation.append(choice);
        operation.append(":");
        operation.append(strList[0]);  // username
        operation.append(":");
        operation.append(amount);

        sendMessage(operation);
    }
    // withdraw money
    else if(choice=="2")
    {
        positive:

        qDebug() << "\nPlease enter amount to withdraw:";

        QString amount = qtin.readLine();

        double amountF = amount.toFloat();
        double balanceF = strList[1].toFloat();


        if(amountF<=0)
        {
            qDebug() << "\n>> Input should be positive.";
            goto positive;
        }


        if(amountF>balanceF)
        {
            qDebug() << "\n>> There is no enough balance."
                     << "\nPress 0 to go back:";

            wrongchoice:
            QString choiceBack = qtin.readLine();

            if(choiceBack=="0")
            {
                QString back = "auth:successful:";

                back.append(strList[0]).append(":")
                    .append(strList[2]).append(":")
                    .append(strList[3]).append(":")
                    .append(strList[1]);

                analyzeMessage(back);

                return;
            }
            else
            {
                qDebug() << "\nPlease enter a valid entry.";
                goto wrongchoice;
            }
        }
        else
        {
            QString operation{"operation:"};  // this is for analyzing message by server
            operation.append(choice);
            operation.append(":");
            operation.append(strList[0]);
            operation.append(":");
            operation.append(amount);

            sendMessage(operation);
        }


    }
    // transfer money
    else if(choice=="3")
    {
        qDebug() << "\nPlease enter the 'CUSTOMER NO' of the person to transfer:";
        QString customto = qtin.readLine();

        qDebug() << "Please enter the 'USERNAME' of the person to transfer:";
        QString userto = qtin.readLine();

        if(userto==strList[0])
        {
            qDebug() << "\n>> Not allowed transfer money to your own account!\n"
                     << "\nPress 0 to go back:";

            start:

            QString choiceT = qtin.readLine();

            if(choiceT=="0")
            {
                QString back = "auth:successful:";

                back.append(strList[0]).append(":")
                    .append(strList[2]).append(":")
                    .append(strList[3]).append(":")
                    .append(strList[1]);

                analyzeMessage(back);

                return;
            }
            else
            {
                qDebug() << "\nPlease enter a valid entry.";
                goto start;
            }
        }
        else
        {
            positiv:
            qDebug() << "Please enter amount to transfer:";
            QString amount = qtin.readLine();

            // to compare variables we need to convert to float
            double amountF = amount.toFloat();
            double balanceF = strList[1].toFloat();

            if(amountF<=0)
            {
                qDebug() << "\n>> Input should be positive.\n";
                goto positiv;
            }


            if(amountF>balanceF)
            {
                qDebug() << "\n>> There is no enough balance.\n"
                         << "\nPress 0 to go back:";

                transfer:

                QString choiceB = qtin.readLine();

                if(choiceB=="0")
                {
                    QString back = "auth:successful:";

                    back.append(strList[0]).append(":")
                        .append(strList[2]).append(":")
                        .append(strList[3]).append(":")
                        .append(strList[1]);

                    analyzeMessage(back);

                    return;
                }
                else
                {
                    qDebug() << "\nPlease enter a valid entry.";
                    goto transfer;
                }
            }
            else
            {
                QString operation{"operation:"};  // this is for analyzing message by server
                operation.append(choice);
                operation.append(":");
                operation.append(strList[0]);
                operation.append(":");
                operation.append(amount);
                operation.append(":");
                operation.append(userto);
                operation.append(":");
                operation.append(customto);
                operation.append(":");
                operation.append(strList[1]);

                sendMessage(operation);
            }
        }

    }
    // sign out
    else if(choice=="4")
    {
        qDebug() << "\nClient signed out!";

        qDebug() << "\nPress 1 to login again:"
                 << "\nPress 2 to exit:";

        exited:

        QString choiceL = qtin.readLine();

        if(choiceL=="1")
        {
            login();
        }
        else if(choiceL=="2")
        {
            socket -> close();
        }
        else
        {
            qDebug() << "\nPlease enter a valid entry.";
            goto exited;
        }

    }
    // disconnect
    else if(choice=="5")
    {
        socket -> close();
    }
    else
    {
        qDebug() << "\n>> Please enter a valid entry:\n";
        operations(userData);
    }

}

// when the connection lost
void MyTcpClient::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;

    qDebug() << "\n>> Disconnected from server..";
}


// error control
void MyTcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << ">> The host was not found.";
        break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << ">> The connection was refused.";
        break;
        default:
            qDebug() << ">> The following error occurred: " << socket->errorString();
        break;
    }
}


// send message to server
void MyTcpClient::sendMessage(const QString &str)
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
            qDebug() << ">> Socket could not be opened!";

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
        qDebug() << ">> Socket could not be opened!";
    }
}


void MyTcpClient::displayMessage(const QString& str)
{
    qDebug() << str;
}

