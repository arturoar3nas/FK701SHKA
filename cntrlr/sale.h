#ifndef SALE_H
#define SALE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "util.h"
#include "mdl/database.h"
#include "mdl/storesale.h"
#include "mdl/math.h"
#include "mdl/operation.h"
#include "mdl/updatesale.h"
#include "mdl/client.h"
#include "mdl/invoid.h"
#include "cntrlr/ctclient.h"
#include "dev/lcdtwoline.h"
#include "dev/epson.h"
#include "dev/balance.h"

class Sale  : public QObject
{
    Q_OBJECT
public:
    Sale(QObject * parent = NULL);
    int trySale(Operation *op, UpdateSale &us);
    int DirectPay(QString qstrPay);
    int setClient(QStringList qstrl);
    int discount(QString qstr, UpdateSale &us);
    int surcharge(QString qstr, UpdateSale &us);
    int anul(UpdateSale &us);
    int correc(QString strtmp, UpdateSale &us);
    int advancePaper();
    int Pay(QString qstrPay, UpdateSale &us, int nTypePay);
    int bottle(QString qstrTmp, UpdateSale &us);
    int suspend(QString qstr, UpdateSale &us);
    int NDC();
    int subTotal(QString &qstr);
    int printfInvoid(bool fEstcnmnto);
    int change(double &nChange);
    int readBalance(QString &qstr);
private:
    bool bOpnInvoid;
    bool bClient;
    bool bOpnTckt;
    bool bPlu;
    Client *c;
    Math *mth;
    StoreSale *ss;
    QString lastItem;
    Invoid *inv;
private:
    int validateInv(QStringList qstrl);
    int validateInput(QString qstrInput);
    int deptUnic(UpdateSale &us);
    QStringList plu(QStringList qstrlPLU, Operation *op);
    QStringList department(QStringList qstrlDept, Operation *op);
signals:
    void sgSndMsg(QString);
};


#endif // SALE_H
