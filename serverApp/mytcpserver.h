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

//protected:
    //void incomingConnection(qintptr socketDescriptor);

public slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(const QString& str);
    void sendMessage(QTcpSocket* socket, QString message);
    void on_pushButton_sendMessage_clicked();
    void analyzeMessage(QString message);
    //void refreshComboBox();

private:
    QTcpServer *m_server;
    QSet<QTcpSocket*> connection_set;
};

#endif // MYTCPSERVER_H
