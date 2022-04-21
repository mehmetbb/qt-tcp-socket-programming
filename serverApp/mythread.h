#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QObject>
#include <QFile>
#include <QSet>
#include <QAbstractSocket>


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int ID, QObject *parent = 0);
    QTcpSocket *socket;
    QString customerList[7][5];
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void readyRead();
    void disconnected();
    void displayError(QAbstractSocket::SocketError socketError);
    void sendMessage(QTcpSocket* socket, QString message);
    void analyzeMessage(QString message);
    void checkAuthorization(QString user, QString pass);
    QString listToString(QString list[][5]);

private:
    QSet<QTcpSocket*> connection_set;
    int socketDescriptor;

};

#endif // MYTHREAD_H
