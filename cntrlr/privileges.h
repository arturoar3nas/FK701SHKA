#ifndef PRIVILEGES_H
#define PRIVILEGES_H

#include "mdl/cashier.h"
#include "util.h"

class Privileges
{
public:
    Privileges();
    bool privileges_Null(Cashier *c);
    bool privileges_Low(Cashier *c);
    bool privileges_Midle(Cashier *c);
    bool privileges_High(Cashier *c);
    bool privileges_SU(Cashier *c);
};

#endif // PRIVILEGES_H
