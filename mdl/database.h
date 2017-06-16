#ifndef DATABASE_H
#define DATABASE_H

extern "C"
{
#include <mdl/unqlite.h>
}
#include <QString>
#include <QStringList>
#include <stdlib.h>
#include "util.h"

#define MAX_BUF 256 /* Static buffer size */

class DataBase
{
public:
    DataBase();
    QStringList getPlu(QString numero_plu);
    QStringList getDept(QString numero_dept);
    int read_db(QString qstrkey, QStringList &qstrlarray);
    int read_db(QString qstrkey, QString &qstr);
    int setDataBase(char *key, char *data);
    int iterateOverDb(char *ptrCmp, char **&ptrBuf);
};

#endif // DATABASE_H
