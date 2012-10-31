#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QMap>
#include "client.h"

class Clients : public QObject
{
    Q_OBJECT
public:
    explicit Clients(QObject *parent = 0);

private:
    QMap<int, Client> clientsMassive;
    
};

#endif // CLIENTS_H
