#include "mytcpserver.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>



// when read file, customer info is writing to 2d array
// preferred to use static array since there is no user addition
QString customerList[7][5];
QString filename;

// Creating object triggers it to Server started
MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(m_server->listen(QHostAddress::Any, 9999))
    {
        connect(this, &MyTcpServer::newMessage, this, &MyTcpServer::displayMessage);
        connect(m_server, &QTcpServer::newConnection, this, &MyTcpServer::newConnection);
        qDebug() << "Server is started!\n";
    }
    else
    {
        qDebug() << "Server is not started!" << m_server->errorString();
        exit(EXIT_FAILURE);
    }
}

// destructor
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

// check for new connection
void MyTcpServer::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

// display whether client connected
void MyTcpServer::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MyTcpServer::displayError);

    qDebug() << "Client connected!";
}

// The server listens to the channel
// and catches when new messages arrive
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

    buffer = buffer.mid(128);

    // message: new message
    QString message = QString::fromStdString(buffer.toStdString());

    //emit newMessage(message);  *for trial*

    // to make the incoming message understandable
    analyzeMessage(message);
}

// Analysis mechanism is used to understand
// messages between server and client.
void MyTcpServer::analyzeMessage(QString message)
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
        for(int i=0;i<7;i++)
        {
            if(data[2]==customerList[i][0])
            {
                double currentBalance = customerList[i][4].toFloat();
                double addMoney = data[3].toFloat();

                currentBalance += addMoney;

                customerList[i][4] = QString::number(currentBalance);

                // To display new balance in client side
                QString successfulOp = "completed:";
                successfulOp.append(customerList[i][0]);  //username
                successfulOp.append(":");
                successfulOp.append(customerList[i][4]);  //current balance

                // applying changes to the file
                writeFile(successfulOp);

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

                // To display new balance in client side
                QString successfulOp = "completed:";
                successfulOp.append(customerList[i][0]);  //username
                successfulOp.append(":");
                successfulOp.append(customerList[i][4]);  //current balance

                // applying changes to the file
                writeFile(successfulOp);

                break;
            }
        }
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

                if(data[5]!=customerList[k][2])
                {
                    foreach (QTcpSocket* socket,connection_set)
                    {
                        QString error{"transfererror:wrongnumber:"};
                        error.append(data[2]);
                        error.append(":");
                        error.append(data[3]);
                        sendMessage(socket, error);
                    }
                }
                else
                    falseNo = true;

                break;
            }
        }

        if(!exist)
        {
            foreach (QTcpSocket* socket,connection_set)
            {
                QString error{"transfererror:notexist:"};
                error.append(data[4]);
                error.append(":");
                error.append(data[2]);
                error.append(":");
                error.append(data[3]);
                sendMessage(socket, error);
            }
        }

        if(exist && falseNo)
        {
            QString bankTo;
            QString bankFrom;
            QString myBalance;
            double currentBalance;
            double addMoney;
            double getMoney;
            double bankCut = 6;  // if banks are different

            for(int i=0;i<7;i++)
            {
                if(data[4]==customerList[i][0])
                {
                    currentBalance = customerList[i][4].toFloat();
                    addMoney = data[3].toFloat();

                    currentBalance += addMoney;

                    customerList[i][4] = QString::number(currentBalance);

                    bankTo = customerList[i][1];

                }
                else if(data[2]==customerList[i][0])
                {
                    currentBalance = customerList[i][4].toFloat();
                    getMoney = data[3].toFloat();

                    currentBalance -= getMoney;

                    bankFrom = customerList[i][1];

                    // When banks are different
                    if(bankFrom!=bankTo)
                        currentBalance -= bankCut;

                    customerList[i][4] = QString::number(currentBalance);

                    myBalance = customerList[i][4];
                }
            }

            // To display new balance in client side
            QString successfulOp = "completed:";
            successfulOp.append(data[2]);  //username
            successfulOp.append(":");
            successfulOp.append(myBalance);  //current balance

            // applying changes to the file
            writeFile(successfulOp);
        }



    }
    else
        qDebug() << "FAILED: message couldnt understand!\n";
}


// reading the txt file which containing the data
void MyTcpServer::readFile()
{
    qDebug() << "Data is kept in <customerinfo.txt> file\n"
                "Please paste the directory where the <customerinfo.txt> file is located:\n"
                "(Example -> C:\\some_folder\\customerinfo.txt)\n";

    start:

    QTextStream qtin(stdin);
    filename = qtin.readLine();

    QFile file(filename);
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << "\nFile can not found!\n"
                    "Please enter the valid directory:";
        goto start;
    }

    QTextStream in(&file);
    QString myText = in.readAll(); // myText contains all customer info
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

         for(int j=0; j<data.size(); j++)
         {
             customerList[i][j] = data[j];            
         }
    }

    qDebug() << "\nOn the client side, you can use information above.\n"
                "Client can be started!";

}

// applying changes to the file
void MyTcpServer::writeFile(QString message)
{
    qDebug() << "Writing to db..";

    QString temp = "";

    for(int i=0;i<7;i++)
    {
        for(int j=0;j<5;j++)
        {
            temp.append(customerList[i][j]);

            if(j!=4)            
                temp.append(":");

        }
        if(i!=6)        
            temp.append("\n");        
    }

    foreach (QTcpSocket* socket,connection_set)
    {
        sendMessage(socket, message);
    }


    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing!";
        return;
    }

    QTextStream out(&file);
    out << temp;
    file.flush();
    file.close();

    qDebug() << "Completed!";
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


