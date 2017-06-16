#ifndef CTDRAWER_H
#define CTDRAWER_H

#include <QString>
#include "mdl/database.h"
#include <stdio.h>
#include "util.h"

class ctDrawer
{
public:
    ctDrawer();
    int handle();
    int updateDrawer(double dInput, int nOption,QString &qstrRet);
    int getDrawer(QString &qstr);
};

#endif // CTDRAWER_H
