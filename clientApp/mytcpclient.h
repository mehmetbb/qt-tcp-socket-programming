#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QMetaType>
#include <QString>
#include <QStandardPaths>
#include <QTcpSocket>

//QObject??
class MyTcpClient : public QObject
{
    Q_OBJECT

public:
    explicit MyTcpClient(QObject *parent = 0);
    ~MyTcpClient();

signals:
    void newMessage(QString);

public slots:    
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(const QString& str);
    void sendMessage(const QString& str);
    void login();
/*
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(qintptr number);
    void sendMessage(const QString &text);
    void connectToServer();
*/

private:
    QTcpSocket *socket;
};

#endif // MYTCPCLIENT_H
