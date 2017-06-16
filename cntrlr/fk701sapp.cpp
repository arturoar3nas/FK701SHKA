#include "fk701sapp.h"

Fk701sApp::Fk701sApp(QObject *parent)
    :QObject(parent)
{

}

void Fk701sApp::start()
{
    gui = new IGui*[STATE_NUMBER];
    gui[STATE_GUI_LOGIN]    = new LogUsr;
    gui[STATE_GUI_SALE]     = new PointOfSale;
    gui[STATE_GUI_CLIENT]   = new Customer;
    gui[STATE_GUI_DRAW]     = new Drawer;
    gui[STATE_GUI_SHOW_PLU] = new ShowPlu;
    gui[STATE_GUI_INDEX_H]  = new Index;
    gui[STATE_GUI_DIALOG]   = new InputDialog;
    gui[STATE_GUI_REPORT]   = new LayoutReport;
    gui[STATE_GUI_INVENTORY]= new LayoutInventory;
    gui[STATE_GUI_CASH_S]   = new CashStatement;
    uinState = STATE_GUI_LOGIN;
    fsmnState = STATE_FSM_LOGIN;
    gui[uinState]->showWidget();

    cashier = new ctCashier();

    op              = new Operation();
    bSessionSale    = false;
    bMsg            = false;
    bCashier        = false;
    bClient         = false;
    bPermission     = false;
    ptrMethod       = new char[BUFFER_MID_SIZE];
}

Fk701sApp::fsmTotalEfect fsm[]=
{
    {STATE_FSM_LOGIN   ,&Fk701sApp::pressKey_Cashier},
    {STATE_FSM_SALE    ,&Fk701sApp::pressKey_Sale},
    {STATE_FSM_CLIENT  ,&Fk701sApp::pressKey_Client},
    {STATE_FSM_INDEX_H ,&Fk701sApp::pressKey_Home},
    {STATE_FSM_INDEX_X ,&Fk701sApp::pressKey_xMode},
    {STATE_FSM_INDEX_RX,&Fk701sApp::pressKey_xOneReport},
    {STATE_FSM_INVENTORY,&Fk701sApp::pressKey_Inventory}
};

Fk701sApp::fsmEsc fsmKeyEsc[]={
  {STATE_GUI_LOGIN    ,STATE_GUI_LOGIN  ,STATE_FSM_LOGIN},
  {STATE_GUI_SALE     ,STATE_GUI_SALE   ,STATE_FSM_SALE},
  {STATE_GUI_CLIENT   ,STATE_GUI_SALE   ,STATE_FSM_SALE },
  {STATE_GUI_DRAW     ,STATE_GUI_SALE   ,STATE_FSM_SALE},
  {STATE_GUI_SHOW_PLU ,STATE_GUI_SALE   ,STATE_FSM_SALE},
  {STATE_GUI_INDEX_H  ,STATE_GUI_INDEX_H,STATE_FSM_INDEX_H},
  {STATE_GUI_INVENTORY,STATE_GUI_INDEX_H,STATE_FSM_INDEX_H},
  {STATE_GUI_REPORT   ,STATE_GUI_INDEX_H,STATE_FSM_INDEX_H},
  {STATE_GUI_CASH_S   ,STATE_GUI_INDEX_H,STATE_FSM_INDEX_H}
};

int Fk701sApp::pressKey_Plu(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_INVENTORY]->isVisibleWidget()) {
        QString strInput;
        gui[uinState]->getFromWidgets(strInput);
        if(strInput.isEmpty()){
            msg = new Message();
            msg->openMsgBoxError(OPTION_INVALID);
            bMsg = true;
        }
        op->setPlu(strInput);
        return SUCESS;
    }

    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        msg = new Message();
        msg->openMsgBoxError(OPTION_INVALID);
        bMsg = true;
        return OPTION_INVALID;
    }

    if(!bSessionSale)
        s = new Sale(); /*creamos la sesion*/
    QString qstrInpt;
    UpdateSale us;
    int ret = FAIL;
    gui[uinState]->getFromWidgets(qstrInpt);
    op->setInput(qstrInpt);
    op->setType(PLU);
    ret = s->trySale(op,us);/*realizamos la venta*/
    if(ret < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(ret);
        bMsg = true;
    }
    else{
        gui[uinState]->updateWidgets(&us);
        bSessionSale = true;
    }
    op->clear();
    return ret;
}

int Fk701sApp::pressKey_Dept(QString sender)
{
    Q_UNUSED(sender);
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    if(!bSessionSale)
        s = new Sale(); /*creamos la sesion*/
    QString qstrInpt;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrInpt);
    op->setInput(qstrInpt);
    op->setType(DEPARTAMENTO);
    int rc = s->trySale(op,us);/*realizamos la venta*/
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
    }
    if(rc == SUCESS){
        bSessionSale = true;
        gui[uinState]->updateWidgets(&us);
    }
    if(rc == UNIC_ITEM_SUCESS){
        gui[uinState]->clear();
        delete s;
        bSessionSale = false;
    }
    op->clear();

    return rc;
}

int Fk701sApp::pressKey_DirectDept(QString sender)
{
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        if(!bSessionSale)
            s = new Sale(); /*creamos la sesion*/
        UpdateSale us;
        op->setInput(sender);
        op->setType(DEPARTAMENTO);
        int rc = s->trySale(op,us);/*realizamos la venta*/
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
        }
        if(rc == SUCESS){
            gui[uinState]->updateWidgets(&us);
            bSessionSale = true;
        }
        if(rc == UNIC_ITEM_SUCESS){
            gui[uinState]->clear();
            delete s;
            bSessionSale = false;
        }
        op->clear();
    }

    return SUCESS;
}

int Fk701sApp::pressKey_Qwerty(QString sender)
{
    gui[uinState]->writeOnWidget(sender);
    return SUCESS;
}

int Fk701sApp::pressKey_BorrarC(QString sender)
{
    Q_UNUSED(sender);
    gui[uinState]->backSpaceLineEdit();
    return SUCESS;
}

int Fk701sApp::pressKey_X(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        QString qstrInput;
        float fQuanty = op->quanty();
        gui[uinState]->getFromWidgets(qstrInput);
        if( fQuanty==0){
            if(qstrInput=="")
                return INPUT_EMPTY;/*error*/
            else{
                op->setQuanty(qstrInput.toFloat());
                return SUCESS;
            }
        }
        else{
            if(qstrInput =="")
                return INPUT_EMPTY;
            else{
                op->setPriceGroup(qstrInput.toFloat());
                return SUCESS;
            }
        }
    }

    if(gui[STATE_GUI_INVENTORY]->isVisibleWidget()) {
        QString strInput;
        gui[uinState]->getFromWidgets(strInput);
        if(strInput.isEmpty()){
            msg = new Message();
            msg->openMsgBoxError(OPTION_INVALID);
            bMsg = true;
            return INPUT_EMPTY;
        }
        op->setQuanty(strInput.toFloat());
    }

    return SUCESS;
}

int Fk701sApp::pressKey_Total(QString sender)
{
    qDebug() << "fsmState " << fsmnState;
    int rc = (this->*fsm[fsmnState].fptr)(sender);
    return rc;
}

int Fk701sApp::pressKey_Esc(QString sender)
{
    Q_UNUSED(sender);
    gui[uinState]->hideWidget();
    const int SIZE_ESC_FSM = sizeof(fsmKeyEsc)/sizeof(fsmKeyEsc[0]);
    fsmEsc const * pEntry = &fsmKeyEsc[0];
    fsmEsc const * pEnd = &fsmKeyEsc[SIZE_ESC_FSM];
    bool bStateFound = false;
    while ((!bStateFound) && (pEntry != pEnd)){
        if (pEntry->fsmCurrentEscState == uinState){
                uinState = pEntry->fsmNextEscState;
                fsmnState = pEntry->fsmStateTE;
                bStateFound = true;
                break;
        }
        ++pEntry;
    }
    gui[uinState]->showWidget();
    if(uinState == STATE_GUI_INDEX_H){
        gui[uinState]->modifyLayout(OPTION_LAYOUT_HOME);
    }

    return SUCESS;
}

int Fk701sApp::pressKey_Sale(QString sender)
{
    Q_UNUSED(sender);
    if(bMsg){
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }
    if(bSessionSale){
        Privileges p;
        if(!p.privileges_Midle(cashier->csh)
                && !bPermission){
            uinState = STATE_GUI_DIALOG;
            gui[uinState]->showWidget();
            memset(ptrMethod,0,BUFFER_MID_SIZE);
            memcpy(ptrMethod,"pressKey_Sale",strlen("pressKey_Sale"));
            return SUCESS;
        }
        QString qstrPay;
        gui[uinState]->getFromWidgets(qstrPay);
        int ret = s->DirectPay(qstrPay);
        if(ret < 0){
            msg = new Message();
            msg->openMsgBoxError(ret);
            bMsg = true;
            return ret;
        }
        else{
            bSessionSale = false;
            bClient = false;
            delete s;
            gui[uinState]->clear();

        }
    }

    return SUCESS;
}

int Fk701sApp::pressKey_SubTotal(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        QString qstr;
        int rc = s->subTotal(qstr);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        msg = new Message();
        msg->openMsgBox(qstr);
        bMsg = true;
        return SUCESS;
    }
}

int Fk701sApp::pressKey_Advance(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        s->advancePaper();
        return SUCESS;
    }
}

int Fk701sApp::pressKey_Cashier(QString sender)
{
    /*! si el cajero esta iniciad entonces damos la opcion de cerrar
        si no esta iniciado entonces damos la opcion de login
     */
    Q_UNUSED(sender);
    if(bMsg){
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }
    if(gui[STATE_GUI_DIALOG]->isVisibleWidget()){
        ctCashier myCashier;
        QStringList qstrl;
        gui[uinState]->getFromWidgets(qstrl);
        int rc = myCashier.Start(qstrl);
        if(rc < SUCESS){
            gui[uinState]->clear();
            return rc;
        }
        gui[uinState]->clear();
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SALE;
        bPermission = true;
        QMetaObject::invokeMethod(this,ptrMethod);
        return SUCESS;
    }
    if(bCashier == true &&
      (uinState == STATE_GUI_INDEX_H || uinState == STATE_GUI_SALE)){
        /*change view*/
        if(bSessionSale){
            /*no se puede salir en medio de una venta*/
            return INPUT_INVALID;
        }
        if(gui[STATE_GUI_SALE]->isVisibleWidget())
            gui[STATE_GUI_SALE]->hideWidget();
        if(gui[STATE_GUI_INDEX_H]->isVisibleWidget())
            gui[STATE_GUI_INDEX_H]->hideWidget();
        uinState = STATE_GUI_LOGIN;
        gui[uinState]->showWidget();
        return SUCESS;
    }
    if(bCashier == true && uinState == STATE_GUI_LOGIN){
        QStringList qstrl;
        gui[uinState]->getFromWidgets(qstrl);
        if(qstrl.at(0).isEmpty() && qstrl.at(1).isEmpty()){
            gui[uinState]->hideWidget();
            uinState = STATE_GUI_SALE;
            gui[uinState]->showWidget();
            return SUCESS;
        }
        int rc = cashier->Close(qstrl);
        if(rc < SUCESS){
            gui[uinState]->clear();
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        bCashier = false;
        gui[uinState]->clear();
        return SUCESS;
    }
    if(bCashier == false &&
      (uinState == STATE_GUI_INDEX_H || uinState == STATE_GUI_SALE)){
        QStringList qstrl;
        gui[uinState]->getFromWidgets(qstrl);
        int rc = cashier->Start(qstrl);
        if(rc < SUCESS){
            delete cashier;
            gui[uinState]->clear();
            return rc;
        }
        /*change view*/
        gui[uinState]->clear();
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SALE;
        fsmnState = STATE_FSM_SALE;
        gui[uinState]->showWidget();
        bCashier = true;
        return SUCESS;
    }
    if(bCashier == false && uinState == STATE_GUI_LOGIN){
        QStringList qstrl;
        gui[uinState]->getFromWidgets(qstrl);
        if(qstrl.at(0).isEmpty()&&qstrl.at(1).isEmpty())return INPUT_EMPTY;//<<<<<<<<<<<<<<<<<<<<<<
        int rc = cashier->Start(qstrl);
        if(rc < SUCESS){
            gui[uinState]->clear();
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            delete cashier;
            return rc;
        }
        /*change view*/
        gui[uinState]->clear();
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SALE;
        fsmnState = STATE_FSM_SALE;
        gui[uinState]->showWidget();
        bCashier = true;
        return SUCESS;
    }

    return SUCESS;
}

int Fk701sApp::pressKey_Client(QString sender)
{
    Q_UNUSED(sender);
    if(bMsg){
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }
    if(gui[STATE_GUI_CLIENT]->isVisibleWidget()){
        /*first we need check if there data in the gui*/
        uinState = STATE_GUI_CLIENT;
        QStringList qstrl;
        gui[uinState]->getFromWidgets(qstrl);
        if(qstrl.at(0).isEmpty()&&qstrl.at(1).isEmpty()&&
           qstrl.at(2).isEmpty()&&qstrl.at(3).isEmpty()&&
           qstrl.at(4).isEmpty()){
            gui[uinState]->hideWidget();
            uinState = STATE_GUI_SALE;
            gui[uinState]->showWidget();
            return SUCESS;
        }
        else{
            if(!bSessionSale)
                s = new Sale();
            int rc = s->setClient(qstrl);
            if(rc < SUCESS){
                msg = new Message();
                msg->openMsgBoxError(rc);
                bMsg = true;
                if(!bSessionSale)
                    delete s;
                return rc;
            }
            bSessionSale = true;
        }
        gui[uinState]->clear();
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SALE;
        fsmnState = STATE_FSM_SALE;
        gui[uinState]->showWidget();
        return SUCESS;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_CLIENT;
        fsmnState = STATE_FSM_CLIENT;
        gui[uinState]->showWidget();
        return SUCESS;
    }
}

int Fk701sApp::pressKey_Drawer(QString sender)
{
    int nOption = sender.toInt();
    Privileges p;
    if(!p.privileges_Low(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_Client",strlen("pressKey_Client"));
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        /*change view*/
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_DRAW;
        gui[uinState]->showWidget();
        QString qstr;
        ctDrawer d;
        d.getDrawer(qstr);
        /*upload*/
        gui[STATE_GUI_DRAW]->updateWidgets(&qstr);
        return SUCESS;
    }
    else if(gui[STATE_GUI_DRAW]->isVisibleWidget()){
        QString qstr, qstrRet;
        gui[uinState]->getFromWidgets(qstr);
        if(!qstr.isEmpty()){
            ctDrawer d;
            d.updateDrawer(qstr.toDouble(),nOption,qstrRet);
            gui[uinState]->updateWidgets(&qstrRet);
            return SUCESS;
        }
        else{
            gui[uinState]->hideWidget();
            uinState = STATE_GUI_SALE;
            gui[uinState]->showWidget();
            return SUCESS;
        }
    }
    else
    {
        return SUCESS;
    }
}

int Fk701sApp::pressKey_ShowPlu(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        QString qstr;
        QStringList qstrl;
        DataBase db;

        /*Get Plu*/
        gui[uinState]->getFromWidgets(qstr);
        qstr = QString::number(qstr.toInt());
        qstr.prepend("PLU_");
        int rc = db.read_db(qstr,qstrl);
        if(qstrl.isEmpty()){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }

        /*change view*/
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SHOW_PLU;
        gui[uinState]->showWidget();

        /*upload*/
        gui[uinState]->updateWidgets(&qstrl);
        return SUCESS;
    }
    else if(gui[STATE_GUI_SHOW_PLU]->isVisibleWidget())
    {
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_SALE;
        gui[uinState]->showWidget();
        return SUCESS;
    }
    else
    {
        return SUCESS;
    }
}

int Fk701sApp::pressKey_Price(QString sender)
{
    Q_UNUSED(sender);
    QString qstrPrice;
    gui[uinState]->getFromWidgets(qstrPrice);

    /*verificamos el precio en la gui*/
    if(qstrPrice.contains(QRegExp("[a-zA-Z]")))/*validamos que no contenga letras*/
        return INPUT_INVALID;

    if(qstrPrice.contains(".") == false && qstrPrice.isEmpty() == false){/*si no tiene coma agregamos 2 ceros*/
        qstrPrice.append("00");
    }
    if(qstrPrice.contains(".")){
        int npoints = qstrPrice.count(".");
        if(npoints > 1){
            return INPUT_INVALID;
        }
        QString qstrsub = qstrPrice.mid(0,1);
        if(qstrsub.compare(".") == 0){
            qstrPrice.prepend("0");
        }
        QStringList qstrlsub = qstrPrice.split(".");
        if(qstrlsub[1].length() > 2){
            return INPUT_INVALID;
        }
    }
    op->setPrice    (qstrPrice);
}

int Fk701sApp::pressKey_ChangeView(QString sender)
{
    Q_UNUSED(sender);
    switch(uinState){
    case STATE_GUI_LOGIN:{
        if(gui[STATE_GUI_LOGIN]->isVisibleWidget())
            gui[STATE_GUI_LOGIN]->hideWidget();
        if(!gui[STATE_GUI_LOGIN]->isVisibleWidget())
            gui[STATE_GUI_SALE]->showWidget();
        uinState++;
        break;
    }
    case STATE_GUI_SALE:{
        gui[uinState]->hideWidget();
        uinState++;
        gui[uinState]->showWidget();
        break;
    }
    case STATE_GUI_CLIENT:{
        gui[uinState]->hideWidget();
        uinState++;
        gui[uinState]->showWidget();
        break;
    }
    case STATE_GUI_DRAW:{
        gui[uinState]->hideWidget();
        uinState++;
        gui[uinState]->showWidget();
        break;
    }
    case STATE_GUI_SHOW_PLU:{
        gui[uinState]->hideWidget();
        uinState++;
        gui[uinState]->showWidget();
        break;
    }
    case STATE_GUI_INDEX_H:{
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_LOGIN;
        gui[uinState]->showWidget();
        break;
    }
    default:
        uinState = STATE_GUI_LOGIN;
        break;
    }
    return SUCESS;
}

int Fk701sApp::pressKey_FocusUp(QString sender)
{
    Q_UNUSED(sender);
    gui[uinState]->focusUp();
    return SUCESS;
}

int Fk701sApp::pressKey_FocusDown(QString sender)
{
    Q_UNUSED(sender);
    gui[uinState]->focusDown();
    return SUCESS;
}

int Fk701sApp::pressKey_Surcharge(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_Surcharge",strlen("pressKey_Surcharge"));
        return SUCESS;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
    QString qstrRec;
    UpdateSale us;
    /*get from gui*/
    gui[uinState]->getFromWidgets(qstrRec);

    int rc = s->surcharge(qstrRec,us);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }

    /*update gui*/
    gui[uinState]->updateWidgets(&us);
    }
    return SUCESS;
}

int Fk701sApp::pressKey_Discount(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_Discount",strlen("pressKey_Discount"));
        return SUCESS;
     }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        QString qstrRec;
        UpdateSale us;

        /*get from gui*/
        gui[uinState]->getFromWidgets(qstrRec);

        int rc = s->discount(qstrRec,us);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }

        /*update gui*/
        gui[uinState]->updateWidgets(&us);
    }
    return SUCESS;
}

int Fk701sApp::pressKey_CloseInvoid(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Midle(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_CloseInvoid",strlen("pressKey_CloseInvoid"));
        return SUCESS;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget() && bSessionSale){
        delete s;
        gui[uinState]->clear();
        bSessionSale = false;
        return SUCESS;
    }
}

int Fk701sApp::pressKey_Home(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_High(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget() && !bSessionSale){
        gui[uinState]->hideWidget();
        uinState = STATE_GUI_INDEX_H;
        fsmnState = STATE_FSM_INDEX_H;
        gui[uinState]->showWidget();
        return SUCESS;
    }
    if(gui[STATE_GUI_INDEX_H]->isVisibleWidget()){
        if(uinState == STATE_GUI_INDEX_H && fsmnState == STATE_FSM_INDEX_H){
            int rc = gui[uinState]->isFocus();
            switch(rc){
            case 0: msg = new Message();
                    msg->openMsgBoxError(INPUT_INVALID);
                    bMsg = true;break;
            case 1:gui[uinState]->hideWidget();
                   uinState = STATE_GUI_SALE;
                   fsmnState = STATE_FSM_SALE;
                   gui[uinState]->showWidget();break;
            case 2:fsmnState = STATE_FSM_INDEX_X;
                   gui[uinState]->modifyLayout(OPTION_LAYOUT_X_MODE);break;
            case 3://nState = STATE_Z_MODE;
                   gui[uinState]->modifyLayout(OPTION_LAYOUT_Z_MODE);break;
            default:break;}
            return SUCESS;
        }
        if(fsmnState != STATE_FSM_INDEX_H){
            uinState = STATE_GUI_INDEX_H;
            fsmnState = STATE_FSM_INDEX_H;
            gui[uinState]->modifyLayout(OPTION_LAYOUT_HOME);
            return SUCESS;
        }
    }
}

int Fk701sApp::pressKey_xMode(QString sender)
{

    Q_UNUSED(sender);
    if(bMsg){
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }
    if(gui[STATE_GUI_INDEX_H]->isVisibleWidget()){
            int rc = gui[uinState]->isFocus();
            XMode modex;
            QStringList qstrl;
            switch(rc){
            case 0:{
                    rc = modex.close_x();
                    return rc;}
            case 1:{
                   fsmnState = STATE_FSM_INDEX_RX;
                   gui[uinState]->modifyLayout(OPTION_LAYOUT_X1);
                   return SUCESS;}
            case 2:{                   
                   gui[uinState]->hideWidget();
                   uinState = STATE_GUI_CASH_S;
                   gui[uinState]->showWidget();
                   rc = modex.declareBox(qstrl);
                   if(rc < SUCESS){
                       msg = new Message();
                       msg->openMsgBoxError(INPUT_INVALID);
                       bMsg = true;
                       return rc;
                   }
                   break;}
            case 3:{
                   gui[uinState]->hideWidget();
                   uinState = STATE_GUI_INVENTORY;
                   fsmnState = STATE_FSM_INVENTORY;
                   gui[uinState]->showWidget();
                   rc = modex.stock(qstrl);
                   if(rc < SUCESS){
                       msg = new Message();
                       msg->openMsgBoxError(INPUT_INVALID);
                       bMsg = true;
                       return rc;
                   }
                   break;}
            case 4:{
                    msg = new Message();
                    msg->openMsgBoxError(INPUT_INVALID);
                    bMsg = true;
                    gui[uinState]->showWidget();
                    rc=modex.garage(qstrl);
                    break;}
            case 5:{
                    msg = new Message();
                    msg->openMsgBoxError(INPUT_INVALID);
                    bMsg = true;
                    gui[uinState]->showWidget();
                    rc=modex.deleteGarage(qstrl);
                    break;}
            case 6:{
                   msg = new Message();
                   msg->openMsgBoxError(INPUT_INVALID);
                   bMsg = true;
                   gui[uinState]->showWidget();
                   rc=modex.saleBook(qstrl);
                   break;}
            default:break;}
            if(rc < SUCESS){
                msg = new Message();
                msg->openMsgBoxError(OPTION_INVALID);
                bMsg = true;
                return rc;
            }
            gui[uinState]->updateWidgets(&qstrl);
            return SUCESS;
    }
}

int Fk701sApp::pressKey_Inventory(QString sender)
{
    Q_UNUSED(sender);
    if(bMsg) {
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }

    if(!gui[STATE_GUI_INVENTORY]->isVisibleWidget()) {
        return SUCESS;
    }
    XMode xmode;
    QStringList strl;
    int rc = xmode.setStock(op,strl);
    if(rc < SUCESS) {
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    QStringList strlstrl;
    strlstrl.append(strl.join(";"));
    gui[uinState]->updateWidgets(&strlstrl);
    return SUCESS;
}

int Fk701sApp::pressKey_zMode(QString sender)
{
    Q_UNUSED(sender);
    if(gui[STATE_GUI_INDEX_H]->isVisibleWidget()){
            int rc = gui[uinState]->isFocus();
            XMode modex;
            QStringList qstrl;
            switch(rc){
            case 0:rc=modex.close_x();return SUCESS;
            case 1:gui[uinState]->modifyLayout(3);break;
            case 2:gui[uinState]->showWidget();
                   rc=modex.declareBox(qstrl);break;
            case 3:gui[uinState]->showWidget();
                   rc=modex.stock(qstrl);break;
            case 4:gui[uinState]->showWidget();
                   rc=modex.garage(qstrl);break;
            case 5:gui[uinState]->showWidget();
                   rc=modex.deleteGarage(qstrl);break;
            case 6:gui[uinState]->showWidget();
                   rc=modex.saleBook(qstrl);break;}
            if(rc < SUCESS){
                msg = new Message();
                msg->openMsgBoxError(OPTION_INVALID);
                bMsg = true;
                return rc;
            }
            gui[uinState]->updateWidgets(&qstrl);
            return SUCESS;
    }
}

int Fk701sApp::pressKey_xOneReport(QString sender)
{

    Q_UNUSED(sender);
    if(bMsg){
        msg->closeMsgBoxError();
        delete msg;
        bMsg = false;
        return SUCESS;
    }

    if(gui[STATE_GUI_INDEX_H]->isVisibleWidget()){
            int rc = gui[uinState]->isFocus();
            gui[uinState]->hideWidget();
            uinState = STATE_GUI_REPORT;
            gui[uinState]->showWidget();
            XMode modex;
            QStringList qstrl;
            rc = modex.report_x(rc,qstrl);
            if(rc < SUCESS){
                msg = new Message();
                msg->openMsgBoxError(OPTION_INVALID);
                bMsg = true;
                return rc;
            }
            gui[uinState]->updateWidgets(&qstrl);
            return SUCESS;
    }
}

int Fk701sApp::pressKey_zOneReport(QString sender)
{
    Q_UNUSED(sender);
    return SUCESS;
}

int Fk701sApp::pressKey_zTwoReport(QString sender)
{
   Q_UNUSED(sender);
   return SUCESS;
}

int Fk701sApp::pressKey_Anul(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Midle(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_Anul",strlen("pressKey_Anul"));
        return SUCESS;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        UpdateSale us;
        bPermission = false;
        int rc = s->anul(us);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        gui[uinState]->updateWidgets(&us);
    }
    return SUCESS;
}

int Fk701sApp::pressKey_Correc(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Midle(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_Correc",strlen("pressKey_Correc"));
        return SUCESS;
    }
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        UpdateSale us;
        QString qstr;
        gui[uinState]->getFromWidgets(qstr);
        int rc = s->correc(qstr,us);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        gui[uinState]->updateWidgets(&us);
    }
    return SUCESS;
}

int Fk701sApp::pressKey_Cash(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstrPay;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrPay);
    int rc = s->Pay(qstrPay,us,1);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    if(rc == PAY_COMPLETE){
        s->printfInvoid(false);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        else
        {   double nChange = 0;
            s->change(nChange);
            msg = new Message();
            bMsg = true;
            QString qstrMsg = "Cambio: " + QString::number(nChange,'f',2);
            msg->openMsgBoxInfo(qstrMsg);
            bSessionSale = false;
            bClient = false;
            delete s;
            gui[uinState]->clear();
            return SUCESS;
        }
    }
    gui[uinState]->updateWidgets(&us);
    return SUCESS;
}

int Fk701sApp::pressKey_Check(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstrPay;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrPay);
    int rc = s->Pay(qstrPay,us,2);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    if(rc == PAY_COMPLETE){
        s->printfInvoid(false);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        else
        {   double nChange = 0;
            s->change(nChange);
            msg = new Message();
            bMsg = true;
            QString qstrMsg = "Cambio: " + QString::number(nChange,'f',2);
            msg->openMsgBoxInfo(qstrMsg);
            bSessionSale = false;
            bClient = false;
            delete s;
            gui[uinState]->clear();
            return SUCESS;
        }
    }
    gui[uinState]->updateWidgets(&us);
    return SUCESS;
}

int Fk701sApp::pressKey_Debit(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstrPay;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrPay);
    int rc = s->Pay(qstrPay,us,3);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    if(rc == PAY_COMPLETE){
        s->printfInvoid(false);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        else
        {   double nChange = 0;
            s->change(nChange);
            msg = new Message();
            bMsg = true;
            QString qstrMsg = "Cambio: " + QString::number(nChange,'f',2);
            msg->openMsgBoxInfo(qstrMsg);
            bSessionSale = false;
            bClient = false;
            delete s;
            gui[uinState]->clear();
            return SUCESS;
        }
    }
    gui[uinState]->updateWidgets(&us);
    return SUCESS;
}

int Fk701sApp::pressKey_Credit(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstrPay;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrPay);
    int rc = s->Pay(qstrPay,us,4);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    if(rc == PAY_COMPLETE){
        s->printfInvoid(false);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            return rc;
        }
        else
        {   double nChange = 0;
            s->change(nChange);
            msg = new Message();
            bMsg = true;
            QString qstrMsg = "Cambio: " + QString::number(nChange,'f',2);
            msg->openMsgBoxInfo(qstrMsg);
            bSessionSale = false;
            bClient = false;
            delete s;
            gui[uinState]->clear();
            return SUCESS;
        }
    }
    gui[uinState]->updateWidgets(&us);
    return SUCESS;
}

int Fk701sApp::pressKey_meansOfPay(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Low(cashier->csh)){
        msg = new Message();
        msg->openMsgBoxError(INPUT_INVALID);
        bMsg = true;
        return INPUT_INVALID;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    int nNumberPay = op->meanofpay();
    if(nNumberPay == 0){
        QString qstr;
        gui[uinState]->getFromWidgets(qstr);
        op->setMeanofpay(qstr.toInt());
        return SUCESS;
    }
    if(nNumberPay != 0){
        QString qstrPay;
        UpdateSale us;
        gui[uinState]->getFromWidgets(qstrPay);
        int rc = s->Pay(qstrPay,us,nNumberPay);
        if(rc < SUCESS){
            msg = new Message();
            msg->openMsgBoxError(rc);
            bMsg = true;
            op->clear();
            return rc;
        }
        if(rc == PAY_COMPLETE){
            s->printfInvoid(false);
            if(rc < SUCESS){
                msg = new Message();
                msg->openMsgBoxError(rc);
                bMsg = true;
                return rc;
            }
            else
            {   double nChange = 0;
                s->change(nChange);
                msg = new Message();
                bMsg = true;
                QString qstrMsg = "Cambio: " + QString::number(nChange,'f',2);
                msg->openMsgBoxInfo(qstrMsg);
                bSessionSale = false;
                bClient = false;
                delete s;
                gui[uinState]->clear();
                return SUCESS;
            }
        }
        gui[uinState]->updateWidgets(&us);
        op->clear();
        return SUCESS;
    }
}

int Fk701sApp::pressKey_bottle(QString sender)
{
    Q_UNUSED(sender);
    Privileges p;
    if(!p.privileges_Midle(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_bottle",strlen("pressKey_bottle"));
        return SUCESS;
    }
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstrPay;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstrPay);
    int rc = s->bottle(qstrPay,us);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    gui[uinState]->updateWidgets(&us);
}

int Fk701sApp::pressKey_suspend(QString sender)
{
    Q_UNUSED(sender);
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    QString qstr;
    UpdateSale us;
    gui[uinState]->getFromWidgets(qstr);
    if(!bSessionSale)return SUCESS;//<<<<<<<<<<<<<<<<<<<<<<
    int rc = s->suspend(qstr,us);
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }
    if(rc == CLEAR_UI){
        gui[uinState]->clear();
        return SUCESS;
    }
    if(rc == SUCESS){
        gui[uinState]->clear();
        gui[uinState]->updateWidgets(&us);
        return rc;
    }
}

int Fk701sApp::pressKey_NDC(QString sender)
{
    Q_UNUSED(sender);
    if(!gui[STATE_GUI_SALE]->isVisibleWidget()){
        return SUCESS;
    }
    Privileges p;
    if(!p.privileges_High(cashier->csh)
            && !bPermission){
        uinState = STATE_GUI_DIALOG;
        gui[uinState]->showWidget();
        memset(ptrMethod,0,BUFFER_MID_SIZE);
        memcpy(ptrMethod,"pressKey_NDC",strlen("pressKey_NDC"));
        return SUCESS;
    }
    int rc = s->NDC();
    if(rc < SUCESS){
        msg = new Message();
        msg->openMsgBoxError(rc);
        bMsg = true;
        return rc;
    }


}

int Fk701sApp::pressKey_balance(QString sender)
{
    //![0]
    //! Validate for balance
    //![0]
    Q_UNUSED(sender);
    if(gui[STATE_GUI_SALE]->isVisibleWidget()){
        QString qstr;
        s->readBalance(qstr);
        qstr.remove(QRegExp("[^0-9]\\."));
        QByteArray qby = qstr.toLocal8Bit();
        char *ptr = qby.data();
        qDebug() << ptr;
        float f;
        f = (float)atof(ptr+1);
        op->setQuanty(f);
        qDebug() << "[6]" << f;
        qDebug() << "[7]" << qstr;
        return SUCESS;
    }

    return SUCESS;
}

