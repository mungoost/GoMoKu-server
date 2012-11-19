#ifndef NET_H
#define NET_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QMap>
#include <QTimer>
#include "client.h"
#include "field.h"
#include <QThread>

typedef QMap<int, Client> Clients_;

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


class Net : public QObject
{
    Q_OBJECT
public:
    explicit Net();
    Clients_ clients;
private:
    QTcpServer *server;
    QTimer *timer;
    void parseData(const QByteArray &array, const int id);
    bool authorized(const QByteArray &array, const Clients_::iterator client);
    bool makeStep(const QByteArray &array, const Clients_::iterator client);
    bool replay(const QByteArray &array, const Clients_::iterator client);
    void connectTwoClients(Clients_::iterator client1, Clients_::iterator client2);
    void disconnectTwoClients(Clients_::iterator client1, Clients_::iterator client2);
    void changeState();
    void checkWin(const Clients_::iterator client_);

public slots:
    void onNewConnection();
    void onDisconnected();
    bool readData();
    void onTimer();
};

#endif // NET_H
