#include "client.h"

Client::Client()
{
}

void Client::send(const QString& cmd)
{
    QTextStream clientStream(socket);
    clientStream << cmd << "\n";
    clientStream << flush;
}
