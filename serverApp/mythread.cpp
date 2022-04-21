#include "mythread.h"
#include "mytcpserver.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>


// to access variables and methods
// mytcpserver object created
MyTcpServer t_server;

MyThread::MyThread(int ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;

    // Get customer data from mytcpserver
    QString customerText = t_server.myText;

    QStringList dataLine = customerText.split("\n");

    for(int i = 0; i < dataLine.size(); i++)
    {
         QStringList data = dataLine[i].split(":");

         for(int j=0; j<data.size(); j++)
         {
             customerList[i][j] = data[j];
         }
    }

}

void MyThread::run()
{
    // thread start here
    // each socket within the network has a unique number -> socket descriptor
    qDebug() << socketDescriptor << "-> thread started.";

    socket = new QTcpSocket;

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    qDebug() << socketDescriptor << "-> client connected.";

    exec();

}


// The server listens to the channel
// and catches when new messages arrive
void MyThread::readyRead()
{
    QString buffer = socket->readAll();

    QString fileSize = buffer.split(";")[0].split(",")[2].split(":")[1];

    QString message = buffer.right(fileSize.toInt());

    //qDebug() << message;  // to suppress the message

    analyzeMessage(message);

}


void MyThread::disconnected()
{
    qDebug() << socketDescriptor << "-> disconnected";
    socket->deleteLater();
    exit(0);
}


// Analysis mechanism is used to understand
// messages between server and client.
void MyThread::analyzeMessage(QString message)
{
    // some string operations
    QStringList data = message.split(":");

    // for login operations
    if(data[0]=="userpass")
    {
        QString user = data[1];
        QString pass = data[2];

        checkAuthorization(user, pass);
    }
    // Add money
    else if(data[0]=="operation" && data[1]=="1")
    {
        QString successfulOp = "completed:";

        for(int i=0;i<7;i++)
        {
            if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double addMoney = data[3].toFloat();

                currentBalance += addMoney;

                customerList[i][4] = QString::number(currentBalance);

                // To display new balance in client side
                successfulOp.append(customerList[i][0]).append(":")  //username
                            .append(customerList[i][1]).append(":")  //bank
                            .append(customerList[i][2]).append(":")  //customer no
                            .append(customerList[i][4]);             //current balance

                break;
            }
        }

        // applying changes to the file
        t_server.writeFile(listToString(customerList));

        sendMessage(socket, successfulOp);
    }
    // Withdraw money
    else if(data[0]=="operation" && data[1]=="2")
    {
        QString successfulOp = "completed:";

        for(int i=0;i<7;i++)
        {
            if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double getMoney = data[3].toFloat();

                currentBalance -= getMoney;

                customerList[i][4] = QString::number(currentBalance);

                // To display new balance in client side
                successfulOp.append(customerList[i][0]).append(":")  //username
                            .append(customerList[i][1]).append(":")  //bank
                            .append(customerList[i][2]).append(":")  //customer no
                            .append(customerList[i][4]);             //current balance

                break;
            }
        }

        // applying changes to the file
        t_server.writeFile(listToString(customerList));

        sendMessage(socket, successfulOp);
    }
    // Transfer money
    else if(data[0]=="operation" && data[1]=="3")
    {
        bool exist = false;
        bool falseNo = false;

        // when transfer money we need to control
        // whether user exist or customer no is true
        for(int k=0;k<7;k++)
        {
            if(data[4]==customerList[k][0])
            {
                exist = true;

                if(data[5]==customerList[k][2])
                {
                    falseNo = true;
                }

                break;
            }
        }

        if(!exist)
        {
            for(int k=0;k<7;k++)
            {
                if(data[2]==customerList[k][0])
                {
                    QString error{"transfererror:notexist:"};
                    error.append(customerList[k][0]).append(":")  // username
                         .append(customerList[k][1]).append(":")  // bank
                         .append(customerList[k][2]).append(":")  // customer no
                         .append(customerList[k][4]).append(":")  // balance
                         .append(data[4]);                        // userto


                    sendMessage(socket, error);

                    break;
                }
            }

        }
        else if(!falseNo)
        {
            for(int k=0;k<7;k++)
            {
                if(data[2]==customerList[k][0])
                {
                    QString error{"transfererror:wrongnumber:"};
                    error.append(customerList[k][0]).append(":")  // username
                         .append(customerList[k][1]).append(":")  // bank
                         .append(customerList[k][2]).append(":")  // customer no
                         .append(customerList[k][4]).append(":")  // balance
                         .append(data[4]);                        // userto

                    sendMessage(socket, error);

                    break;
                }
            }

        }

        // if user exist and customer no is true
        // perform the transfer
        if(exist && falseNo)
        {
            QString bankTo;
            QString bankFrom;
            bool cutOrNot = false;
            double currentBalance;
            double addMoney;
            double getMoney;
            double bankCut = 6;  // when banks are different
            int temp = 0;

            for(int i=0;i<7;i++)
            {
                if(customerList[i][0]==data[4])
                {
                    currentBalance = customerList[i][4].toFloat();
                    addMoney = data[3].toFloat();

                    currentBalance += addMoney;

                    customerList[i][4] = QString::number(currentBalance);

                    bankTo = customerList[i][1];

                }
                else if(customerList[i][0]==data[2])
                {
                    currentBalance = customerList[i][4].toFloat();
                    getMoney = data[3].toFloat();

                    currentBalance -= getMoney;

                    customerList[i][4] = QString::number(currentBalance);

                    bankFrom = customerList[i][1];

                    temp = i;

                }
            }

            // When banks are different
            if(bankFrom!=bankTo)
            {
                double cBalance = customerList[temp][4].toFloat();
                cBalance -= bankCut;
                customerList[temp][4] = QString::number(cBalance);
                cutOrNot = true;
            }

            // To display new balance in client side
            QString successfulOp = "completed:";
            successfulOp.append(customerList[temp][0]).append(":")  //username
                        .append(customerList[temp][1]).append(":")  //bank
                        .append(customerList[temp][2]).append(":")  //customer no
                        .append(customerList[temp][4]);             //current balance

            if(cutOrNot)
                successfulOp.append(":true");

            // applying changes to the file
            t_server.writeFile(listToString(customerList));

            sendMessage(socket, successfulOp);
        }



    }
    else
        qDebug() << "\nFAILED: message error! Please restart the application.\n";
}




// authorization check
void MyThread::checkAuthorization(QString user, QString pass)
{
    bool exist = false;
    for(int k=0;k<7;k++)
    {
        if(user==customerList[k][0] && pass==customerList[k][3])
        {
            qDebug() << ">>" << customerList[k][0] << "logged in!";

            QString authMessage{"auth:successful"};

            authMessage.append(":").append(customerList[k][0])
                       .append(":").append(customerList[k][1])
                       .append(":").append(customerList[k][2])
                       .append(":").append(customerList[k][4]);

            //socket->write(authMessage.toUtf8());


            sendMessage(socket, authMessage);

            exist = true;
            break;
        }
    }

        if(!exist)
        {
            qDebug() << ">> Login failed!";

            sendMessage(socket, "auth:failed");
        }
}



// error messages
void MyThread::displayError(QAbstractSocket::SocketError socketError)
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
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            qDebug() << ">> The following error occurred: " << socket->errorString();
        break;
    }
}


// send message to client
void MyThread::sendMessage(QTcpSocket* socket, QString message)
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
        qDebug() << ">> Socket could not be opened";


}


QString MyThread::listToString(QString list[][5])
{
    QString myTemp = "";

    for(int i=0;i<7;i++)
    {
        for(int j=0;j<5;j++)
        {
            myTemp.append(list[i][j]);

            if(j!=4)
                myTemp.append(":");

        }
        if(i!=6)
            myTemp.append("\n");
    }

    return myTemp;
}








