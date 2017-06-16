#ifndef FK701SAPP_H
#define FK701SAPP_H

#include "cntrlr/sale.h"
#include "cntrlr/ctcashier.h"
#include "cntrlr/ctclient.h"
#include "cntrlr/ctdrawer.h"
#include "cntrlr/privileges.h"
#include "cntrlr/xmode.h"
#include "mdl/operation.h"
#include "mdl/updatesale.h"
#include "mdl/database.h"
#include "mdl/client.h"
#include "view/pointofsale.h"
#include "view/message.h"
#include "view/customer.h"
#include "view/drawer.h"
#include "view/index.h"
#include "view/logusr.h"
#include "view/showplu.h"
#include "view/IGui.h"
#include "view/inputdialog.h"
#include "view/layoutinventory.h"
#include "view/layoutreport.h"
#include "view/cashstatement.h"
#include "util.h"


#include <QVector>
#include <QString>
#include <QList>

class Fk701sApp : public QObject
{
    Q_OBJECT
public:
    Fk701sApp(QObject * parent = NULL);
private:
    IGui **gui;
    Sale *s;
    Operation *op;
    Message *msg;
    ctCashier *cashier;
    QList<const char*> lfoo;
    InputDialog *id;
    char *ptrMethod;
    bool bSessionSale;
    bool bCashier;
    bool bClient;
    bool bMsg;
    bool bPermission;
    int uinState;
    int fsmnState;

public:
    typedef struct{
        int fsmState;
        int (Fk701sApp::*fptr)(QString);
    }fsmTotalEfect;
    typedef struct{
        int fsmCurrentEscState;
        int fsmNextEscState;
        int fsmStateTE;
    }fsmEsc;

public slots:
     void start();
     int pressKey_Plu(QString sender);
     int pressKey_Dept(QString sender);
     int pressKey_DirectDept(QString sender);/*QString*/
     int pressKey_Qwerty(QString sender);/*QString*/
     int pressKey_BorrarC(QString sender);
     int pressKey_X(QString sender);
     int pressKey_Total(QString sender);
     int pressKey_SubTotal(QString sender);
     int pressKey_Price(QString sender);
     int pressKey_Cash(QString sender);
     int pressKey_Check(QString sender);
     int pressKey_Debit(QString sender);
     int pressKey_Credit(QString sender);
     int pressKey_ChangeView(QString sender);
     int pressKey_Drawer(QString sender);/*const int*/
     int pressKey_FocusUp(QString sender);
     int pressKey_FocusDown(QString sender);
     int pressKey_Cashier(QString sender);
     int pressKey_Client(QString sender);
     int pressKey_ShowPlu(QString sender);
     int pressKey_Sale(QString sender);
     int pressKey_Surcharge(QString sender);
     int pressKey_Discount(QString sender);
     int pressKey_Home(QString sender);
     int pressKey_CloseInvoid(QString sender);
     int pressKey_Anul(QString sender);
     int pressKey_Correc(QString sender);
     int pressKey_meansOfPay(QString sender);
     int pressKey_Advance(QString sender);
     int pressKey_bottle(QString sender);
     int pressKey_suspend(QString sender);
     int pressKey_NDC(QString sender);
     int pressKey_balance(QString sender);
     int pressKey_xMode(QString sender);
     int pressKey_zMode(QString sender);
     int pressKey_xOneReport(QString sender);
     int pressKey_zOneReport(QString sender);
     int pressKey_zTwoReport(QString sender);
     int pressKey_Esc(QString sender);
     int pressKey_Inventory(QString sender);
};

#endif // FK701SAPP_H
