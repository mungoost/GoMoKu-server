#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork/QTcpSocket>
#include "field.h"

enum Status
{
    ST_CONNECTED,
    ST_AUTHORIZED,
    ST_WAITING_STEP,
    ST_MAKING_STEP,
    ST_READY
};

enum Symbol
{
    SM_O,
    SM_X
};

class Client
{
public:
    Client();

public:
    Status state;
    QTcpSocket *socket;
    QMap<int, Client>::iterator playingWith;
    Symbol sym;
    void send(const QString& cmd);
    Field *myField;
};

#endif // CLIENT_H
