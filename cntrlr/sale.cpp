#include "sale.h"

Sale::Sale(QObject *parent)
    :QObject(parent)
{
    ss  = new StoreSale();
    mth = new Math();
    c   = new Client();
    inv = new Invoid[9];
    bClient = false;
    bOpnTckt = false;/*! contra el teclado */
    bOpnInvoid = false;/*! contra la impresora*/
    bPlu = false;
}

int Sale::trySale(Operation *op, UpdateSale &us)
{
    /*Variables locales*/
    QString strInput = op->input();
    int type = op->type();
    QStringList qstrlItem,
                qstrlComeBack;
    DataBase db;
    int ret = FAIL;

    /*cargamos la venta*/
    switch(type){
    case DEPARTAMENTO:
    {
        if (strInput.compare("unknown") == 0){
            ret = validateInput(strInput);
            if(ret < 0){
                return INPUT_ERROR;
            }
        }
        strInput.prepend("DEPT_");/*Agregamos el key*/
        qstrlItem = db.getDept(strInput);
        if(qstrlItem.count() <= 1){/*revisamos si la lista de strings viene completa*/
            return PLU_NO_FOUND;
        }
        qstrlComeBack = department(qstrlItem,op);
        break;
    }
    case PLU:
    {
        ret = validateInput(strInput);/*validamos la entrada*/
        if(ret < 0){
            return INPUT_ERROR;
        }

        strInput.prepend("PLU_");
        qstrlItem = db.getPlu(strInput);
        if(qstrlItem.count()<=1){/*revisamos si la lista de strings viene completa*/
            return PLU_NO_FOUND;
        }

        /*realizamos la operacion de venta*/
        qstrlComeBack = plu(qstrlItem,op);
        ret = validateInv(qstrlComeBack);
        if(ret < SUCESS){
            return LOW_SOTCK;
        }
        break;
    }
    default:
        break;
    }

    mth->doCalcs(qstrlComeBack);/*actualizamos los calculos*/

    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
    qstrlComeBack[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrlComeBack.join(";"));
    bOpnInvoid = true;

    //!Guardamos el key del ultimo producto ingresado
    lastItem = strInput;

    /*guardamos el plu enviado cuando se cierra la factura o se suspende se borra esta lista*/
    QString qstr_tmp = qstrlComeBack.join(";");
    qstr_tmp.append(";"+strInput);//agregamos el key
    ss->setPlu(qstr_tmp);

    /* LCD */
    Lcdtwoline lcd;
    lcd.setLcd(qstrlComeBack.at(0),QString::number(mth->getMonto(),'f',2));

    /*Departamentos/PLU Unicos*/
    switch(type){
    case DEPARTAMENTO:/*Departamento*/
        if(qstrlItem.at(14).compare("Yes")==0){
            ret = deptUnic(us);
            return ret;
        }
        break;
    case PLU:/*PLU*/
        if(qstrlItem.at(14).compare("Yes")==0){
            ret = deptUnic(us);
            return ret;
        }
        if(qstrlComeBack.at(15).compare("Vncld_")){/*Vinculados*/
            QStringList qstrl_vncldkeys;
            QString qstrkey_vncld = qstrlComeBack.at(15);
            QString qstr_tmpvcld;
            db.read_db(qstrkey_vncld,qstrl_vncldkeys);/*obtenemos la lista de keys*/
            if(ret < SUCESS){
                return -1;
            }
            foreach(const QString &str, qstrl_vncldkeys){
                if(str.compare(strInput) != 0){/*para evitar repetir la primera llave!*/
                    db.read_db(str,qstrlItem);/*Optenemos los plu*/
                    ret = validateInv(qstrlItem);/*validamos el inventario*/
                    if(ret == SUCESS){
                        mth->doCalcs(qstrlItem);/*actualizamos los calculos*/
                        qstrlItem[6] = QString::number(mth->getBi(),'f',2);
                        qstr_tmpvcld = qstrlItem.join(";");
                        qstr_tmpvcld.append(";"+str);//agregamos el key
                        ss->setPlu(qstr_tmpvcld);/*guardamos en tmp*/
                        lastItem = str;
                        us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
                        us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
                        us.setQstrlItem(qstr_tmpvcld);
                        lcd.setLcd(qstrlItem.at(0),QString::number(mth->getMonto(),'f',2));
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    bPlu = true;
    return SUCESS;
}

int Sale::validateInv(QStringList qstrl)
{
    int nInv = qstrl.at(10).toInt();
    int nCant = qstrl.at(5).toInt();
    if(nInv < nCant)
        return FAIL;/*inventario insuficiente*/
    else
        return SUCESS;
}

int Sale::validateInput(QString qstrInput)
{
    if(qstrInput.contains(QRegExp("[a-zA-Z-.]"))){
        return INPUT_ERROR;
    }

    if(qstrInput.isEmpty() || qstrInput.isNull()){
        return INPUT_EMPTY;
    }
    return SUCESS;
}

int Sale::deptUnic(UpdateSale &us)
{
    /*Enviamos la Factura*/
    printfInvoid(true);

    /*Cargamos la gui con la informacion*/
    QString qstr = QString::number(mth->getPvp(),'f',2);/*total*/
    us.setQstrTotal(qstr);

    ss->cleanTmp();
    mth->cleanMounts();
    bOpnInvoid = false;
    return UNIC_ITEM_SUCESS;
}

QStringList Sale::department(QStringList qstrlDept, Operation *op)
{
    /*Declaramos las variables locales*/
    float flrelacion_1 = 0,
            floldPrice = 0,
            flnewPrice = 0;
    int ioldPrice = 0;
    int int16_inewPrice = 0;//!int16_t
    float quanty = op->quanty();
    float priceGroup = op->priceGroup();
    QString qstrPrice = op->price();

    /*DEPT abierto*/
    if(qstrlDept.at(12).compare("Yes")!=0){/*El usuario debe utilizar el precio programado*/
        if(!qstrPrice.isNull() && !qstrPrice.isEmpty()){
            qstrPrice = "";
        }
    }

    if(quanty == 0){
        /*Si tiene precio programado usar si no el del line edit
          Se asume un articulo*/

        if(qstrPrice==""){/*no se ingreso precio manualmente*/
            return qstrlDept;
        }
        else if(qstrPrice!=""){
            qstrlDept[6]=qstrPrice;
            return qstrlDept;
        }
        else{
            qstrlDept.clear();
            return qstrlDept;
        }
    }
    /*Si Articulos !="" entonces es una multiplicacion de venta por departamento
     *Si el departamento tiene un precio guardado, y se realiza una venta marcando un precio se omite el precio que este guardado en el departamento
     *Si Articulos no tiene punto entonces es una multiplicacion entera de venta por departamento*/
    else
    {
        if(quanty - (int)quanty == 0.0){
            if(priceGroup==0){
                /*Multiplicacion entera de venta por Dept*/
                if(qstrPrice==""){
                    /*Colocamos la cantidad*/
                    qstrlDept[5]=QString::number(quanty);
                    quanty = 0.00;
                    return qstrlDept;
                }
                else if(qstrPrice!=""){
                    /*Venta libre por dep con precio no prog*/
                    qstrlDept[5]=QString::number(quanty);
                    qstrlDept[6]=qstrPrice;
                    quanty = 0.00;
                    return qstrlDept;
                }
                else{
                    qstrlDept.clear();
                    return qstrlDept;
                }
            }
            /* Si precio grupos!=0 entonces es una venta por grupo Ejemplo(3x1)
             * int tmp=iARTICULOS/iPRECIO_GRUPOS;
             * int ifPrecio=tmp*iPrice*/
            else{
                /*Venta por Grupos*/
                flrelacion_1 = quanty / priceGroup;
                if(qstrPrice==""){
                    /*Venta por grupos con precio programado*/
                    floldPrice =qstrlDept.at(6).toFloat();
                    flnewPrice = flrelacion_1*floldPrice;
                    qstrlDept[6]=QString::number(flnewPrice);
                    qstrlDept[5]=QString::number(quanty);
                    return qstrlDept;
                }
                else if(qstrPrice!=""){
                    /*Venta grupo con departamento no programado*/
                    ioldPrice = qstrPrice.toInt();
                    int16_inewPrice = flrelacion_1*ioldPrice;
                    qstrlDept[6]=QString::number(int16_inewPrice);
                    qstrlDept[5]=QString::number(quanty);
                    return qstrlDept;
                }
            }
        }
        /*Si tiene punto es una multiplicacion fraccionaria de venta por departamento*/
        else{
            /*operacion fraccionaria*/
            if(qstrPrice.isEmpty() == false)
                qstrlDept[6]= qstrPrice;
            qstrlDept[5]=QString::number(quanty);
            quanty = 0.0;
            return qstrlDept;

        }
    }
    qstrlDept.clear();
    return qstrlDept;
}

QStringList Sale::plu(QStringList qstrlPLU, Operation *op)
{
    /*Declaramos las variables*/
    float floldPrice = 0,
          flinewPrice = 0,
          flrelacion_1 = 0;

    QString qstrPrice = op->price();
    float quanty = op->quanty();
    float priceGroup = op->priceGroup();

    /*PLU abierto*/
    if(qstrlPLU.at(12).compare("Yes")!=0){/*El usuario debe utilizar el precio programado*/
        if(!qstrPrice.isNull() && !qstrPrice.isEmpty()){
            qstrPrice = "";
        }
    }

    if(quanty == 0){
        /*Si tiene precio programado usar si no el del line edit
          Se asume un articulo*/
        if(qstrPrice==""){/*no se ingreso precio manualmente*/
            return qstrlPLU;
        }
        else if(qstrPrice!=""){
            qstrlPLU[6] = qstrPrice;
            return qstrlPLU;
        }
        else{
            qstrlPLU.clear();
            return qstrlPLU;
        }
    }
    /*Si Articulos !="" entonces es una multiplicacion de venta por departamento
     *Si el departamento tiene un precio guardado, y se realiza una venta marcando un precio se omite el precio que este guardado en el departamento
     * Si Articulos no tiene punto entonces es una multiplicacion entera de venta por departamento*/
    else
    {
        if(quanty - (int)quanty == 0.0)
        {
            if(priceGroup==0)/*Multiplicacion entera de venta por PLU*/
            {

                if(qstrPrice=="")/*Colocamos la cantidad*/
                {
                    qstrlPLU[5]=QString::number(quanty);
                    quanty = 0.00;
                    return qstrlPLU;
                }
                else if(qstrPrice!="")/*Venta libre por dep con precio no prog*/
                {
                    qstrlPLU[5]=QString::number(quanty);/*Colocamos la cantidad*/
                    qstrlPLU[6]=qstrPrice;
                    quanty = 0.00;
                    return qstrlPLU;
                }
                else
                {
                    qstrlPLU.clear();
                    return qstrlPLU;
                }
            }
            /* Si precio grupos!=0 entonces es una venta por grupo Ejemplo(3x1)
             * int tmp=iARTICULOS/iPRECIO_GRUPOS;
             * int ifPrecio=tmp*iPrice*/
            else
            {
                flrelacion_1 = quanty / priceGroup;
                if(qstrPrice=="")/*Venta por grupos con precio programado*/
                {
                    floldPrice = qstrlPLU.at(6).toFloat();
                    flinewPrice = flrelacion_1*floldPrice;
                    qstrlPLU[6]=QString::number(flinewPrice);
                    qstrlPLU[5]=QString::number(quanty);
                    return qstrlPLU;
                }
                else if(qstrPrice!="")/*Venta grupo con departamento no programado*/
                {
                    floldPrice =qstrPrice.toFloat();
                    flinewPrice = flrelacion_1*floldPrice;
                    qstrlPLU[6]=QString::number(flinewPrice);
                    return qstrlPLU;
                }
                else
                {
                    qstrlPLU.clear();
                    return qstrlPLU;
                }
            }
        }
        /*Si tiene punto es una multiplicacion fraccionaria de venta por departamento*/
        else/*operacion fraccionaria*/
        {
            if(qstrPrice.isEmpty() == false )
                qstrlPLU[6] = qstrPrice;
            qstrlPLU[5] = QString::number(quanty);
            quanty = 0.0;
            return qstrlPLU;
        }
    }
    qstrlPLU.clear();
    return qstrlPLU;
}

int Sale::DirectPay(QString qstrPay)
{
//! [0]
//! Validacion
//! [0]

    /*Set vars*/
    double dtotal = 0,
           dpay = 0,
           dcanceled = 0;
    QStringList qstrlpay;
    int ret = SUCESS;

    if(!bPlu){/* Si no existe plu registrado*/
        return PLU_NO_FOUND;
    }

    dpay = qstrPay.toDouble();
    dtotal = mth->getPvp();/*get total*/
    dcanceled = mth->getCanceled();/*Obtenemos el total*/
    qstrlpay = ss->pay();

    if(qstrlpay.isEmpty()){/*Pago Vacio*/
        ret = printfInvoid(false);
        return ret;
    }
    return ret;
}

int Sale::Pay(QString qstrPay, UpdateSale &us, int nTypePay)
{
    //! [0]
    //! Validaciones
    //! [0]

    if(!bPlu){/*Validamos que existan productos registrados*/
        return PLU_NO_FOUND;
    }

    int rc = -1;
    QString qstrkey,
            qstrdata;
    QStringList qstrlMean;
    DataBase db;
    Epson e;
    double dTotal = 0;

    qstrkey = QString::number(nTypePay);/*to QString*/
    qstrkey.prepend("PMethod_");/*Construimos el key*/

    /*read from db*/
    rc = db.read_db(qstrkey,qstrlMean);
    if(rc < SUCESS){
        return INPUT_INVALID;
    }

    dTotal = mth->getPvp();/*obtenemos el total*/
    if(qstrPay.isEmpty()){
        qstrPay = QString::number(dTotal,'f',2);
    }

    /*! Validation*/
    /*monto max*/
    int iMax = qstrlMean.at(3).toInt();
    int iPay = qstrPay.toInt();
    if(iPay > iMax){
        return INPUT_ERROR;
    }

    /*Cajon*//*Cambio*/
    /*if cajon = true -> cambio = true*/
    if(qstrlMean.at(7).compare("Y") == 0){
        e.OpenMoneyBox();
    }

    /*Pago Parcial*//*Cambio*/
    /*El pago tiene que ser total y si Cambio  = false entonces no puede ser mayor al total*/
    if(dTotal <= 0){/*No se aceptan mas pagos si el total es cero o menor*/
        //! Fk701sApp::mw->message_box("[ERROR] No se admiten mas pagos!!");
        return -1;
    }

    if(qstrlMean.at(5).compare("N") == 0){
        if(iPay < dTotal){
            return INPUT_INVALID;
        }
        if(qstrlMean.at(4).compare("N") == 0){
            if(iPay > dTotal){
                return INPUT_INVALID;
            }
        }
    }
    /*tasa divisa*/

    /*Imprimir ID*/

    /*end validation*/

    qstrlMean.append(qstrPay);/*agregamos el pago*/
    qstrdata = qstrlMean.join("\n");/*to QString*/
    ss->setPay(qstrdata);/*Guardamos el pago*/

    /*descontamos del total*/
    double dPay = qstrPay.toDouble();
    mth->discount_Discharge(0,dPay);
    mth->setCanceled(dPay);/*calculamos el pago*/

    /*actualizamos la gui*/
    QStringList qstrl;
    qstrl.append(qstrlMean.at(1));
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append(" ");
    qstrl.append(qstrPay);//!monto
    qstrl.append(" ");

    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
    qstrl[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrl.join(";"));

    mth->setChange(iPay);
    if(iPay >= dTotal){
        return PAY_COMPLETE;
    }
    else
    {
        return PAY_INCOMPLETE;
    }

    return SUCESS;
}

int Sale::printfInvoid(bool fEstcnmnto)
{
//! [0]
//! Validaciones
//! [0]
//! [1]
//! int nStep = 0;
//! qDebug() <<"[STEP][SENDINVOID] "<< nStep++ <<" [LINE] " <<__LINE__;
//! [1]
#ifdef PRINTER
    QStringList qstrlplu_tmp;
    QStringList qstrlplu;
    int ret = -1;
    Epson e;
    QString qstrMeanPay;
    QStringList qstrldesrec;
    double dpayout = 0,
           drest = 0;
    int itype = 0,
        imount = 0,
        ipayout = 0,
        itypeTique = 0,
        ticket = 0;
    /*Validamos que la impresora este conectada*/
    ret = e.getStatus();/*informamos al cliente de que se va a enviar la factura*/
    if(ret == -1){
        return PRINTER_NO_FOUND;
    }

    /*validamos si hay productos registrados*/
    qstrlplu = ss->plu();
    if(qstrlplu.isEmpty()){
        return PLU_NO_FOUND;
    }

    /*Enviar el cliente si existe*/
    if(!bOpnTckt){
        QStringList qstrlclientTmp;
        qstrlclientTmp = ss->client();
        if(!qstrlclientTmp.isEmpty()){
            /*Abrimos la Factura con el cliente*/
            qDebug() << qstrlclientTmp;
            ret = e.openTiqueFactura(qstrlclientTmp);
            if(ret == -4){
                return CLIENT_ID_RESP_ERROR;
            }
            ticket = __TIQUE_FACTURA__;
            bOpnTckt = true;
        }
        else{
            /*Abrimos la Factura*/
            ret = e.openTique();
            if(ret != 0){
                return CANT_OPEN_TICKET;
            }
            ticket = __TIQUE__;
            bOpnTckt = true;
        }
    }

    /*enviamos a la impresora los productos*/
    foreach(const QString &str, qstrlplu){
        qstrlplu_tmp = str.split(";");
        ret = e.sendPLu(qstrlplu_tmp,ticket);/*send to print*/
        if(ret != SUCESS){
            return PLU_SEND_ERROR;
        }
        if(ret == SUCESS){
            qstrlplu_tmp.clear();
        }
    }

    /* Nota:
     * Validar con subtotal
     * Enviamos el descuento o recargo a la impresora
     */
    qstrldesrec = ss->descRec();
    if(qstrldesrec.isEmpty() == false){
        QStringList qstrldesrectmp;
        foreach(const QString &str, qstrldesrec){
            qstrldesrectmp = str.split(";");
            itype = qstrldesrectmp.at(2).toInt();
            imount = qstrldesrectmp.at(0).toInt();
            ret = e.sndrecdesc(itype,qstrldesrectmp.at(1),imount,0);
            if(ret != SUCESS){
                return DISCNT_SURCHRG_ERROR;
            }
            if(ret == SUCESS){
                qstrldesrectmp.clear();
            }
        }
    }

    /*Pagamos*/
    /*convertimos el pago en int*/
    /*leemos el archivo temporal de pagos si esta vacio leemos el pago del line edit
      si el archivo esta lleno pagamos multiples veces desde ahi */
    if(fEstcnmnto == false){

        QStringList qstrlpay;
        qstrlpay = ss->pay();
        if(qstrlpay.isEmpty()){/*Si el pago es vacio*/
            QString qstrPay;//! = pos->getPay();
            if(qstrPay.isEmpty()){
                ipayout = mth->getPvp();
            }
            else{
                ipayout = qstrPay.toInt();
            }
            dpayout = (double)ipayout;
            qstrMeanPay = "EFECTIVO";

            /*Enviamos el pago a la impresora*/
            itypeTique = ticket;
            ret = e.payTique(dpayout,itypeTique,qstrMeanPay,ticket);
            if(ret == -1){
                return PAY_ERROR;
            }
            else if(ret == -2){
                return MEAN_OF_PAY_ERROR;
            }
            else if(ret == -3){
                qDebug("[DEBUG][SENDINVOID] Pago completo");
                drest = drest + dpayout;
            }

            /*Abrimos la Gaveta*/
            e.OpenMoneyBox();
        }
        else{/*Si existe el pago*/
            QStringList tmp;
            double dpay = 0;
            itypeTique = ticket;
            foreach(const QString &str, qstrlpay){
                tmp = str.split("\n");
                dpay = tmp.at(9).toDouble();
                ret = e.payTique(dpay,itypeTique,tmp[1],ticket);
                if(ret != SUCESS){
                    return PAY_ERROR;
                }
                else
                    tmp.clear();
            }
        }
    }

    /*Cerramos el ticket*/
    ret = e.closeTique(ticket);//tipo tique
    if(ret != SUCESS){
        return CLOSE_TICKET_ERROR;
    }

    /*si el medio de pago es efectivo descontamos del cajon de dinero*/
    if(qstrMeanPay.compare("EFECTIVO") == 0){
        double dchange = mth->getChange();
        if(dchange != 0){
            /*descontamos*/
            //!__ioption = 1; //gaveta

            //!double dtotal = mth->getPvp();
            //!ret = BckGrndRetreat(dtotal);
            if(ret != 0){
                qDebug("[DEBUG][SENDINVOID] Fail!!! descuento gaveta");
            }
        }
    }

    bOpnTckt = false;
#endif
    /*Descontamos el inventario*/
    /*!
    if(!fEstcnmnto){
        foreach(const QString &str, qstrlplu){
            qDebug("[DEBUG][SENDINVOID] qstrlplu = %s",str.toLatin1().constData());
            qstrlplu_tmp = str.split(";");//! to QStringList
            //!Fk701sApp::db->discountInv(qstrlplu_tmp);//!descontamos en db
            if(ret != SUCESS){
                //!Fk701sApp::mw->message_box("[ERROR] No se pudo enviar el Item!");
                qDebug("[DEBUG][SENDINVOID][ERROR] No se pudo enviar el Item!");
                return PLU_SEND_ERROR;
            }
            if(ret == SUCESS){
                qstrlplu_tmp.clear();
            }
        }
    }
    */

    /*guardamos la venta en db*/
 /*! Reports* oRp = new Reports();
     oRp->updateTrade(Fk701sApp::db);
     delete oRp;
 */
    return SUCESS;
}

int Sale::setClient(QStringList qstrl)
{
    ctClient client;
    int rc = client.create(qstrl,c);
    if(rc < SUCESS){
        return rc;
    }
    if(!bClient)
        bClient = true;
    ss->setClient(qstrl);
    return 0;
}

int Sale::discount(QString qstr,UpdateSale &us)
{
    int nOption = 0;
    QStringList qstrdata;
    int nMount;

    /*validamos que se encuentren registrados productos*/
    if(!bPlu){
        return PLU_NO_FOUND;
    }

    if(qstr.isEmpty()){
        return INPUT_EMPTY;
    }

    if(qstr.contains(".")==true){
        qstr.replace(".","");/*limitar 2 decimales*/
    }
    else
        qstr.append("00");

    nMount = qstr.toInt();/*Validamos el monto*/
    if(nMount < 0){
        return INPUT_INVALID;
    }

    qstrdata.append(qstr);
    qstrdata.append(";Descuento;");
    qstrdata.append(QString::number(nOption,10));//type
    qstrdata.append(";");

    ss->setDescRec(qstrdata);/*Guardamos en el file*/

    double dDescrec = qstr.toDouble();
    dDescrec = dDescrec/100;
    /*Calculamos el monto nuevo*/
    mth->discount_Discharge(nOption,dDescrec);

    QStringList qstrl;
    qstrl.append("Descuento");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append(" ");
    qstrl.append(QString::number(dDescrec,'f',2));//!monto
    qstrl.append(" ");

    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
    qstrl[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrl.join(";"));

    return SUCESS;
}

int Sale::surcharge(QString qstr,UpdateSale &us)
{
    int nOption = 1;
    QStringList qstrdata;
    int nMount;

    /*validamos que se encuentren registrados productos*/
    if(!bPlu){
        return PLU_NO_FOUND;
    }

    if(qstr.isEmpty()){
        return INPUT_EMPTY;
    }

    if(qstr.contains(".")==true){
        qstr.replace(".","");/*limitar 2 decimales*/
    }
    else
        qstr.append("00");

    nMount = qstr.toInt();/*Validamos el monto*/
    if(nMount < 0){
        return INPUT_INVALID;
    }

    qstrdata.append(qstr);
    qstrdata.append(";Recargo;");
    qstrdata.append(QString::number(nOption,10));//type
    qstrdata.append(";");

    ss->setDescRec(qstrdata);/*Guardamos en el file*/

    double dDescrec = qstr.toDouble();
    dDescrec = dDescrec/100;

    /*Calculamos el monto nuevo*/
    mth->discount_Discharge(nOption,dDescrec);

    QStringList qstrl;
    qstrl.append("Recargo");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append("null");
    qstrl.append(" ");
    qstrl.append(QString::number(dDescrec,'f',2));//!monto
    qstrl.append(" ");

    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
    qstrl[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrl.join(";"));

    return SUCESS;
}

int Sale::anul(UpdateSale &us)
{
    QString qstrKey;
    int nIndex = 0;
    bool bExist = false;
    QString strInput;
    QStringList qstrldata;
    QStringList qstrlplu;
    QStringList qstrltmp;
    DataBase db;

    db.read_db(lastItem,qstrlplu);/*reading from db*/
    if(qstrlplu.count()<=1){/*check the list*/
        return PLU_NO_FOUND;
    }

    /*reading from tmp*/
    qstrldata = ss->plu();

    /*find the item*/
    foreach(const QString &str, qstrldata){
        qstrltmp = str.split(";");
        qstrKey = qstrltmp.takeLast();/*buscamos en la ultima posicion el key y comparamos*/
        if(qstrKey.compare(lastItem) == 0)
        {bExist = true;break;}/*si existe el item */
        nIndex++;
    }

    if(!bExist){
        return PLU_NO_FOUND;
    }

    /*si existe lo removemos*/
    ss->removeAtPlu(nIndex);

    /*Descontamos de los montos*/
    mth->discountByanulItem(qstrltmp);

    /*load the object*/
    QString qstrMont;
    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    qstrMont = QString::number(mth->getMonto(),'f',2);
    qstrMont.prepend("-");
    us.setQstrCalcs(qstrMont);
    qstrltmp[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrltmp.join(";"));

    return SUCESS;
}

int Sale::correc(QString strtmp, UpdateSale &us)
{
    QStringList qstrldata;
    QStringList qstrlplu;
    QStringList qstrltmp;
    DataBase db;
    QString qstrKey;
    int nIndex = 0;
    bool bExist = false;

    strtmp.prepend("PLU_");
    db.read_db(strtmp,qstrlplu);/*leemos el plu de la base de datos*/
    if(qstrlplu.count()<=1){/*revisamos si la lista de strings viene completa*/
        return PLU_NO_FOUND;
    }

    /*get from tmp*/
    qstrldata = ss->plu();

    /*find the item*/
    foreach(const QString &str, qstrldata){
        qstrltmp = str.split(";");
        qstrKey = qstrltmp.takeLast();/*buscamos en la ultima posicion el key y comparamos*/
        if(qstrKey.compare(strtmp) == 0)
        {bExist = true;break;}/*si existe el item */
        nIndex++;
    }
    if(!bExist){
        return PLU_NO_FOUND;
    }

    /*si existe lo removemos*/
    ss->removeAtPlu(nIndex);

    /*Descontamos de los montos*/
    mth->discountByanulItem(qstrltmp);

    /*lo mostamos en gui como negativo*/
    QString qstrMont;
    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    qstrMont = QString::number(mth->getMonto(),'f',2);
    qstrMont.prepend("-");
    us.setQstrCalcs(qstrMont);
    qstrltmp[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrltmp.join(";"));

    return SUCESS;
}

int Sale::advancePaper()
{
    Epson e;
    e.advancepaper();
    return 0;
}

int Sale::bottle(QString qstrTmp, UpdateSale &us)
{
    QString qstrKey;
    QStringList qstrlplu,
                qstrltmp,
                qstrldata;
    int nIndex = 0;
    bool bExist = false;
    DataBase db;

    /*leemos el numero de plu de la gui*/
    qstrTmp.prepend("PLU_");

    /*leemos el plu de la base de datos*/
    db.read_db(qstrTmp,qstrlplu);

    /*verificamos que tenga un 4 en la posicion 11*/
    if(qstrlplu.at(11).compare("4") != 0){
        return FAIL;
    }

    /*leemos el tmp en db*/
    qstrldata = ss->plu();

    /*buscamos el item*/
    foreach(const QString &str, qstrldata){
        qstrltmp = str.split(";");
        qstrKey = qstrltmp.takeLast();/*buscamos en la ultima posicion el key y comparamos*/
        if(qstrKey.compare(qstrTmp) == 0)
        {bExist = true;break;}/*si existe el item */
        nIndex++;
    }

    if(!bExist){
        return PLU_NO_FOUND;
    }

    /*si existe lo removemos*/
    ss->removeAtPlu(nIndex);

    /*Descontamos de los montos*/
    mth->discountByanulItem(qstrlplu);

    /*lo mostamos en gui como negativo*/
    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
    qstrltmp[6] = QString::number(mth->getBi(),'f',2);
    us.setQstrlItem(qstrlplu.join(";"));

    return SUCESS;
}

int Sale::suspend(QString qstr, UpdateSale &us)
{
    /*Declaramos las variables*/
    QStringList qstrlplu_db,
            qstrlplu_inv,
            qstrlclient;
    int nInvoid = 0;
    bool bInvoid = false;

    /*verificamos si hay plu registrados*/
    if(!ss->isPlu())
        bInvoid = true;

    /*Leemos el numero de la factura*/
    nInvoid = qstr.toInt();
    if(qstr.isEmpty()){
        return INPUT_EMPTY;
    }
    if(nInvoid > 9 || nInvoid == 0){
        return INPUT_INVALID;
    }

    /*si no hay factura preguntamos si en ese numero existe una y la restablecemos*/
    if(!bInvoid){
        inv[nInvoid].getClient(qstrlclient);
        inv[nInvoid].getItems(qstrlplu_inv);
        if( qstrlplu_inv.isEmpty() ){/*no existe factura en esa posicion*/
            return INVOID_NO_FOUND;
        }
        else{/*cargamos la factura en gui y en db tmp*/
            /*Cargamos los items*/
            QStringList qstrltmp;
            foreach(const QString &str, qstrlplu_inv){
                ss->setPlu(str); /*cargamos en db*/
                qstrltmp = str.split(";");
                mth->doCalcs(qstrltmp);/*Hacemos los calculos*/
                us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
                us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
                qstrltmp[6] = QString::number(mth->getBi(),'f',2);
                us.setQstrlItem(qstrltmp.join(";"));
            }
            if(!qstrlclient.isEmpty())/*Cargamos el cliente*/
                    ss->setClient(qstrlclient);
            inv->oldInvoid = nInvoid;/*Guardamos el numero de factura*/
            return SUCESS;
        }
    }
    else/*Si existe Factura la guardamos y limpiamos*/
    {
        inv[nInvoid].getClient(qstrlclient);/*leemos de Invoid*/
        inv[nInvoid].getItems(qstrlplu_inv);

        if( qstrlplu_inv.isEmpty() ){/*Y no existe factura en esa posicion*/
            qstrlclient = ss->client();/*get client*/
            qstrlplu_db = ss->plu();
            if(!qstrlclient.isEmpty())
                foreach(const QString &str ,qstrlclient)
                    inv[nInvoid].setClient(str);
            foreach (const QString &str, qstrlplu_db)/*set PLU*/
                inv[nInvoid].setItem(str);

            /*limpiamos*/
            ss->cleanTmp();
            mth->cleanMounts();
            //!bPlu = false;
            //!bClient = false;
            return CLEAR_UI;
        }
        else{/*si existe la factura y existe una factura en curso*/
            if((int)inv->oldInvoid == nInvoid){/*sobre escribimos y guardamos*/
                inv[nInvoid].cleanItems();/*limpiamos primero*/
                qstrlclient = ss->client();/*get client*/
                qstrlplu_db = ss->plu();
                if(!qstrlclient.isEmpty())
                    foreach(const QString &str ,qstrlclient)
                        inv[nInvoid].setClient(str);
                foreach (const QString &str, qstrlplu_db)/*set PLU*/
                    inv[nInvoid].setItem(str);

                /*limpiamos*/
                ss->cleanTmp();
                mth->cleanMounts();
                //!bPlu = false;
                //!bClient = false;
                return CLEAR_UI;
            }
            else{
                /*primero guardamos la factura en la posicion recordada*/
                inv[inv->oldInvoid].cleanItems();/*limpiamos primero*/
                qstrlclient = ss->client();/*set client*/
                if(!qstrlclient.isEmpty())
                    foreach(const QString &str ,qstrlclient)
                        inv[inv->oldInvoid].setClient(str);
                qstrlplu_db = ss->plu();
                foreach (const QString &str, qstrlplu_db)/*set PLU*/
                    inv[inv->oldInvoid].setItem(str);

                /*limpiamos*/
                ss->cleanTmp();
                mth->cleanMounts();

                /*luego cargamos la nueva factura*/
                QStringList qstrltmp;
                foreach(const QString &str, qstrlplu_inv){
                    ss->setPlu(str); /*cargamos en db*/
                    qstrltmp = str.split(";");
                    mth->doCalcs(qstrltmp);/*Hacemos los calculos*/
                    us.setQstrTotal(QString::number(mth->getPvp(),'f',2));
                    us.setQstrCalcs(QString::number(mth->getMonto(),'f',2));
                    qstrltmp[6] = QString::number(mth->getBi(),'f',2);
                    us.setQstrlItem(qstrltmp.join(";"));
                }

                if(!qstrlclient.isEmpty())/*Cargamos el cliente*/
                       ss->setClient(qstrlclient);

                inv->oldInvoid = nInvoid;/*Guardamos el numero de factura*/
            }
        }
        return SUCESS;
    }

    return SUCESS;
}

int Sale::NDC()
{
    int ret = 0;
    int ticket = __NDC__;
    QStringList qstrlplu;
    QStringList qstrlplu_tmp;
    Epson e;

    /*Validamos que la impresora este conectada*/
    ret = e.getStatus();
    if(ret == -1){
        return PRINTER_NO_FOUND;
    }

    /*validamos de que hayan productos registrados*/
    if(!bPlu){
        return PLU_NO_FOUND;
    }

    /*abrimos la ndc*/
    QStringList qstrlclient = ss->client();
    qDebug() << qstrlclient;
    if(!qstrlclient.isEmpty()){
        ret = e.OpenNDC(qstrlclient);
        if(ret == -4){
            return CANT_OPEN_TICKET;
        }
    }
    else
        return CLIENT_EMPTY;

    qstrlplu = ss->plu();
    foreach(const QString &str, qstrlplu){
        qstrlplu_tmp = str.split(";");
        ret = e.sendPLu(qstrlplu_tmp,ticket);//send to print
        if(ret != SUCESS){
            return PLU_SEND_ERROR;
        }
        if(ret == SUCESS){
            qstrlplu_tmp.clear();
        }
    }

    /*Realizamos el pago*/
    int     ipayout = 0,
            itypeTique = 0;
    double  dpayout = 0.00,
            drest = 0.00;

    ipayout = mth->getPvp();
    dpayout = (double)ipayout;

    QString qstrMeanPay = "EFECTIVO";
    itypeTique = ticket;
    ret = e.payTique(dpayout,itypeTique,qstrMeanPay,ticket);
    if(ret == -1){
        return PAY_ERROR;
    }
    else if(ret == -2){
        return PAY_FOUL_PAY;
    }
    else if(ret == -3){
        drest = drest + dpayout;
    }

    /*cerramos el documento*/
    ret = e.closeTique(ticket);//tipo tique
    if(ret != SUCESS){
        return CLOSE_TICKET_ERROR;
    }

    /*limpiamos el archivo de cliente*/
    ss->cleanTmp();

    return 0;
}

int Sale::subTotal(QString &qstr)
{
    /*! desplegamos un msg box con los datos
     *  del iva y el subtotal*/
    if(!bPlu){
        return PLU_NO_FOUND;
    }
    QString qstrtotal,
            qstrsub,
            qstriva;

    qstrtotal = QString::number(mth->getPvp(),'f',2);
    qstrsub = QString::number(mth->getSubTotal(),'f',2);
    qstriva = QString::number(mth->getIva(),'f',2);

    qstr = "TOTAL = "+qstrtotal+"\n"+ "SUBTOTAL = "+
            qstrsub+"\n"+"IVA = "+qstriva;

    return SUCESS;
}

int Sale::change(double &nChange)
{
    nChange = mth->getChange();
    return SUCESS;
}

int Sale::readBalance(QString &qstr)
{
    Balance b;
    int rc = b.readBalance(qstr);
    return rc;
}
