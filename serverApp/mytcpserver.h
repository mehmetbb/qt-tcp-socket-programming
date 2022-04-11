#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QSet>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();

signals:
    void newMessage(QString);

public slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(QString str);
    void sendMessage(QTcpSocket* socket, QString message);    
    void analyzeMessage(QString message);
    void checkAuthorization(QString user, QString pass);
    void readFile();
    void writeFile(QString message);

private:
    QTcpServer *m_server;
    QSet<QTcpSocket*> connection_set;
};

#endif // MYTCPSERVER_H
