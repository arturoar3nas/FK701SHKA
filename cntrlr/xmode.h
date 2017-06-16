#ifndef XMODE_H
#define XMODE_H

#include "dev/epson.h"
#include "cntrlr/reports.h"
#include "mdl/operation.h"
#include "util.h"

class XMode
{
public:
    XMode();
    int close_x();
    int report_x(int nOption, QStringList &qstrl);
    int declareBox(QStringList &qstrl);
    int stock(QStringList &qstrl);
    int garage(QStringList &qstrl);
    int deleteGarage(QStringList &qstrl);
    int saleBook(QStringList &qstrl);
    int setStock(Operation *op, QStringList &strlPlu);
private:
    typedef int (*fptr)(void);
    int report_plu(QStringList &qstrl);
    int report_department(QStringList &qstrl);
    int report_cashier(QStringList &qstrl);
    int report_mozo(QStringList &qstrl);
    int report_sale(QStringList &qstrl);
};

#endif // XMODE_H
