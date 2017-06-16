#ifndef CTCLIENT_H
#define CTCLIENT_H

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include "mdl/database.h"
#include "mdl/client.h"
#include "util.h"

class ctClient
{
public:
    ctClient();
    int create(QStringList qstrl, Client *&c);
private:
};

#endif // CTCLIENT_H
