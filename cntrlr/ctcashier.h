#ifndef CTCASHIER_H
#define CTCASHIER_H

#include "dev/magneticstripreader.h"
#include "mdl/database.h"
#include "mdl/cashier.h"
#include "util.h"

class ctCashier
{
public:
    ctCashier();
    int handle();
    int Start(QStringList qstrlCashier);
    int Close(QStringList &qstrl);
private:
    DataBase *db;
    MagneticStripReader *msr;
    bool bCashier;
public:
    Cashier *csh;

};

#endif // CTCASHIER_H
