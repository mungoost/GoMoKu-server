#include "net.h"
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QByteArray>
#include <QRegExp>

Net::Net()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress("192.168.0.100"), 123);
    qDebug() << "Server started on 192.168.0.100:123\n\n";
    timer = new QTimer(this);
    timer->setInterval(3000);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer->start();
}

void Net::onNewConnection()
{
    qDebug() << "New connection!!!\n";
    Client client;
    client.state = ST_CONNECTED;
    client.socket = server->nextPendingConnection();
    client.playingWith = clients.end();
    int clientId = client.socket->socketDescriptor();
    clients.insert(clientId, client);
    connect(client.socket, SIGNAL(readyRead()),this, SLOT(readData()));
    connect(client.socket, SIGNAL(disconnected()),client.socket, SLOT(deleteLater()));
}

bool Net::readData()
{
    QTcpSocket *clientSocket=(QTcpSocket*)sender();
    QByteArray data=clientSocket->readAll();
    parseData(data, clientSocket->socketDescriptor());
    return true;
}

void Net::onDisconnected()
{
    qDebug() << clients.keys();
    QTcpSocket *clientSocket=(QTcpSocket*)sender();
    Clients_::iterator it = clients.find(clientSocket->socketDescriptor());
    int f = clients.remove(clientSocket->socketDescriptor());
    qDebug() << "Client disconnected";
    qDebug() << it.key() << clients.end().key() << f;
    //qDebug() << "Playing with " << clientSocket->;
}

void Net::parseData(const QByteArray &array, const int id)
{
    Clients_::iterator it = clients.find(id);
    if (authorized(array, it)) return;
    if (makeStep(array, it)) return;
    qDebug() << "wrongcmd";
    it->send("wrongcmd");
}

bool Net::authorized(const QByteArray &array, const Clients_::iterator client)
{
    QString cmd = array;
    QRegExp rx("authorized:(\\w+):(.+)");
    if (client->state == ST_CONNECTED)
    {
        if (rx.indexIn(cmd)!=-1)
        {
            if ((rx.cap(1) == "admin") &&
                 (rx.cap(2) == "123"))
                {
                    qDebug() << "client authorized";
                    client->state = ST_READY;
                    return true;
                }
                else
                {
                    qDebug() << "wrong login or password";
                     client->send("wrong login or password");
                    return true;
                }
        }}
    return false;
}

bool Net::makeStep(const QByteArray &array, const Clients_::iterator client)
{
    QString cmd = array;
    QRegExp rx("step:(\\d{1,2}):(\\d{1,2})");
    if (client->state == ST_MAKING_STEP)
        if (rx.indexIn(cmd)!=-1)
        {
            int x = rx.cap(1).toInt();
            int y = rx.cap(2).toInt();
            if (( x <= 15) && (y <= 15) && ( x >= 0) && ( y >= 0))
            {
                QByteArray stepOut = "step:"+rx.cap(1).toLocal8Bit()+":"+rx.cap(2).toLocal8Bit();
                if (client->sym == SM_O)
                {
                    client->myField->setField(x, y, CL_O);
                    client->state = ST_WAITING_STEP;
                    client->playingWith->state = ST_MAKING_STEP;
                    client->socket->write("state:ST_WAITING_STEP");
                    client->playingWith->socket->write(stepOut);
                    qDebug() << "makestep" << x << y;
                }
                else
                {
                    client->myField->setField(x, y, CL_X);
                    client->state = ST_WAITING_STEP;
                    client->playingWith->state = ST_MAKING_STEP;
                    client->socket->write("state:ST_WAITING_STEP");
                    client->playingWith->socket->write(stepOut);
                    qDebug() << "makestep" << x << y;
                }
                checkWin(client);
            }
            else
            {
                qDebug() << "wrong coords";
                 client->send("wrong coords");
                return true;
            }
        }
    return true;
}

void Net::onTimer()
{
    Clients_::iterator freeClient=clients.end();
    for (Clients_::iterator i=clients.begin();i!=clients.end();i++)
        if (i->state == ST_READY)
        {
            if (freeClient==clients.end())
                freeClient=i;
            else
            {
                connectTwoClients(freeClient,i);
                freeClient=clients.end();
            }
        }
}

void Net::connectTwoClients(Clients_::iterator client1, Clients_::iterator client2)
{
        client1->state=ST_MAKING_STEP;
       client2->state=ST_WAITING_STEP;
       client1->sym = SM_X;
       client1->sym = SM_O;
       client1->playingWith=client2;
       client2->playingWith=client1;

       client1->myField = new Field;
       client2->myField = client1->myField;

       client1->socket->write("found:x");
       client2->socket->write("found:o");

       //client1->socket->flush();

      // SleeperThread sleeper;
       //sleeper.msleep(100);

       //client1->socket->write("go:\n");
}

void Net::checkWin(const Clients_::iterator client_)
{
    Clients_::iterator client = client_;
    Cell winner = client->myField->isWin();
    if (winner != CL_CLEAR)
    {
        client->socket->flush();
        client->playingWith->socket->flush();
        SleeperThread sleeper;
        sleeper.msleep(200);
        if (client->sym == SM_O)
        {
            if (winner == CL_O)
            {
                client->socket->write("win");
                client->playingWith->socket->write("lose");
            }
            else
            {
                client->socket->write("lose");
                client->playingWith->socket->write("win");
            }
            client->socket->disconnect();
            client->playingWith->socket->disconnect();
        }
        else
        {
            if (winner == CL_X)
            {
                client->socket->write("win");
                client->playingWith->socket->write("lose");
            }
            else
            {
                client->socket->write("lose");
                client->playingWith->socket->write("win");
            }
            disconnectTwoClients(client, client->playingWith);
        }
    }
}

void Net::disconnectTwoClients(Clients_::iterator client1, Clients_::iterator client2)
{
    client1->socket->disconnect();
    client2->socket->disconnect();
    clients.erase(client1);
    clients.erase(client2);
    delete client1->myField;
    client2->myField = 0;
    return;
}
