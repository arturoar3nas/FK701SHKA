#ifndef REPORTS_H
#define REPORTS_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include "mdl/database.h"
#include "json.h"

extern "C"
{
    #include "mdl/unqlite.h"
}

using QtJson::JsonObject;
using QtJson::JsonArray;
class DataBase;

class Reports
{
public:
    Reports();
    int updateTrade(DataBase *&oDb);
    int getReportDept(QStringList &qstrl);
    int getReportCashier(QStringList &qstrl);
    int getReportPlu(QStringList &qstrl);
    void testMap();
private:
    void _calctotal(QString quanty, QString price, QString &sold);
    void _printResult(const JsonObject &result);
    void _printJson(const JsonObject &result);
};
/*
╔═════════════════╤═════════════════════════╤═════════════════╤═════════════════════════════╗
║ Tipo de Reporte │ Número de Reporte       │ Modo de Reporte │ Posición de la cerradura de ║
╠═════════════════╪═════════════════════════╪═════════════════╪═════════════════════════════╣
║   FINANCIERO    │            1            │      X2/Z2      │             X/Z             ║
╟─────────────────┼─────────────────────────┼─────────────────┼─────────────────────────────╢
║                 │                         │      X1/Z1      │             X/Z             ║
║      DEPT       │            2            ├─────────────────┼─────────────────────────────╢
║                 │                         │      X2/Z2      │             X/Z             ║
╟─────────────────┼─────────────────────────┼─────────────────┼─────────────────────────────╢
║                 │                         │      X1/Z1      │             X/Z             ║
║     CAJERO      │            3            ├─────────────────┼─────────────────────────────╢
║                 │                         │      X2/Z2      │             X/Z             ║
╟─────────────────┼─────────────────────────┼─────────────────┼─────────────────────────────╢
║                 │                         │      X1/Z1      │             X/Z             ║
║      MOZO       │            4            ├─────────────────┼─────────────────────────────╢
║                 │                         │      X2/Z2      │             X/Z             ║
╟─────────────────┼─────────────────────────┼─────────────────┼─────────────────────────────╢
║       PLU       │                         │      X1/Z1      │             X/Z             ║
║                 │            5            ├─────────────────┼─────────────────────────────╢
║                 │                         │      X2/Z2      │             X/Z             ║
╟─────────────────┼────────────┬──────┬─────┼─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X1/Z1      │             X/Z             ║
║                 │  POR HORA  │      │  1  ├─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X2/Z2      │             X/Z             ║
║                 ├────────────┤      ├─────┼─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X1/Z1      │             X/Z             ║
║  VENTA PERIODO  │ DIA SEMANA │  6   │  2  ├─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X2/Z2      │             X/Z             ║
║                 ├────────────┤      ├─────┼─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X1/Z1      │             X/Z             ║
║                 │  DIA MES   │      │  3  ├─────────────────┼─────────────────────────────╢
║                 │            │      │     │      X2/Z2      │             X/Z             ║
╚═════════════════╧════════════╧══════╧═════╧═════════════════╧═════════════════════════════╝
 */



#endif // REPORTS_H
