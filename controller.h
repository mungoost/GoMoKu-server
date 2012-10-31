#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "net.h"
#include "field.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    
private:
    Net *server;
    Field field;
};

#endif // CONTROLLER_H
