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
                    "---------------------------";
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
            QString choice = "";

            qDebug() << "\n>> User" << data[6] << "does not exist!";

            while(choice!="0")
            {
                qDebug() << "\nPress 0 to go back:";

                choice = qtin.readLine();

                if(choice=="0")
                {
                    QString back = "auth:successful:";

                    back.append(data[2]).append(":")  // username
                        .append(data[3]).append(":")  // bank
                        .append(data[4]).append(":")  // customer no
                        .append(data[5]);             // balance

                    analyzeMessage(back);

                    break;
                }
                else
                {
                    qDebug() << "\nPlease enter a valid entry.";
                }
            }

        }
        else if(data[1]=="wrongnumber")
        {
            QString choice = "";

            qDebug() << "\n>> There is a mismatch in the customer name and number!";

            while(choice!="0")
            {
                qDebug() << "\nPress 0 to go back:";

                choice = qtin.readLine();

                if(choice=="0")
                {
                    QString back = "auth:successful:";

                    back.append(data[2]).append(":")  // username
                        .append(data[3]).append(":")  // bank
                        .append(data[4]).append(":")  // customer no
                        .append(data[5]);             // balance

                    analyzeMessage(back);

                    break;
                }
                else
                {
                    qDebug() << "\nPlease enter a valid entry.";
                }
            }

        }
    }
    // operation successful message
    else if(data[0]=="completed")
    {
        QString choice = "";

        qDebug() << "\n>> Operation successful!";

        if(dataSize==6)
            qDebug() << "\n>> 6 TL deduction fee is charged between different banks.";


        while(choice!="0")
        {
            qDebug() << "\nPress 0 to go back:";

            choice = qtin.readLine();

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
            }
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
        QString amount;
        double amountFl = 0;

        // negativity check
        do{
            qDebug() << "\nPlease enter amount to deposit:";

            amount = qtin.readLine();
            amountFl = amount.toFloat();

            if(amountFl<=0)
                qDebug() << "\n>> Input should be positive.";

        }while(amountFl<=0);


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
        QString amount;
        double amountF = 0;
        double balanceF = strList[1].toFloat();

        // negativity check
        do{
            qDebug() << "\nPlease enter amount to withdraw:";

            amount = qtin.readLine();
            amountF = amount.toFloat();

            if(amountF<=0)
                qDebug() << "\n>> Input should be positive.";

        }while(amountF<=0);

        // over balance check
        if(amountF>balanceF)
        {
            QString choiceBack = "";

            qDebug() << "\n>> There is no enough balance.";

            while(choiceBack!="0")
            {
                qDebug() << "\nPress 0 to go back:";

                choiceBack = qtin.readLine();

                if(choiceBack=="0")
                {
                    QString back = "auth:successful:";

                    back.append(strList[0]).append(":")
                        .append(strList[2]).append(":")
                        .append(strList[3]).append(":")
                        .append(strList[1]);

                    analyzeMessage(back);

                    break;
                }
                else
                {
                    qDebug() << "\nPlease enter a valid entry.";
                }
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

        // own account check
        if(userto==strList[0])
        {
            QString choiceT = "";

            qDebug() << "\n>> Not allowed transfer to your own account!";

            while(choiceT!="0")
            {
                qDebug() << "\nPress 0 to go back:";

                choiceT = qtin.readLine();

                if(choiceT=="0")
                {
                    QString back = "auth:successful:";

                    back.append(strList[0]).append(":")
                        .append(strList[2]).append(":")
                        .append(strList[3]).append(":")
                        .append(strList[1]);

                    analyzeMessage(back);

                    break;
                }
                else
                {
                    qDebug() << "\nPlease enter a valid entry.";
                }
            }

        }
        else
        {
            QString amount;
            double amountF = 0;
            double balanceF = strList[1].toFloat();

            // negativity check
            do{
                qDebug() << "Please enter amount to transfer:";

                amount = qtin.readLine();
                amountF = amount.toFloat();

                if(amountF<=0)
                    qDebug() << "\n>> Input should be positive.\n";

            }while(amountF<=0);


            // over balance check
            if(amountF>balanceF)
            {
                QString choiceB = "";

                qDebug() << "\n>> There is no enough balance.";

                while(choiceB!="0")
                {
                    qDebug() << "\nPress 0 to go back:";

                    choiceB = qtin.readLine();

                    if(choiceB=="0")
                    {
                        QString back = "auth:successful:";

                        back.append(strList[0]).append(":")
                            .append(strList[2]).append(":")
                            .append(strList[3]).append(":")
                            .append(strList[1]);

                        analyzeMessage(back);

                        break;
                    }
                    else
                    {
                        qDebug() << "\nPlease enter a valid entry.";
                    }
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
        QString choiceL = "";

        qDebug() << "\nClient signed out!";

        qDebug() << "\nPress 1 to login again:"
                 << "\nPress 2 to exit:";


        while(choiceL!="1" || choiceL!="2")
        {
            choiceL = qtin.readLine();

            if(choiceL=="1")
            {
                login();
                break;
            }
            else if(choiceL=="2")
            {
                socket -> close();
                break;
            }
            else
            {
                qDebug() << "\nPlease enter a valid entry.";
            }
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

        qDebug() << "\nPlease enter your username:";
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

