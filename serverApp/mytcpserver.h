#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QDir>
#include "mythread.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    static QString myText;
    void startServer();

signals:
    void newMessage(QString);

public slots:
    void readFile();
    void writeFile(QString message);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QTcpServer *m_server;

};

#endif // MYTCPSERVER_H
