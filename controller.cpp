#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{
    server = new Net;
}
