#include "ctdrawer.h"

ctDrawer::ctDrawer()
{

}

int ctDrawer::handle()
{
    return 0;
}

int ctDrawer::updateDrawer(double dInput, int nOption
                           ,QString &qstrRet)
{
    /*Definimos las variables*/
    QString qstrIn;
    QString qstr;
    DataBase db;
    QByteArray array;
    double dBckGrndRetreat = 0;
    double dBckGrndRetreatin = 0;
    char cvalue [32] = {0};
    char cKey [8] = {0};
    int rc = -1;
    double    n = 0;
    int  numberOfDigt = 0;
    memcpy(cKey,"Fondo_1",7);

    if(dInput == 0){
        //!qstrIn.append(Fk701sApp::mw->getBckGrndRetreat());
        if(qstrIn.isEmpty() == true){
            return INPUT_EMPTY;
        }
        /*qstring to double*/
        dBckGrndRetreatin = qstrIn.toDouble();
    }
    else
        dBckGrndRetreatin = dInput;
    /*leemos de la base de datos y realizamos la operacion segun sea el caso*/
    db.read_db("Fondo_1",qstr);
    if(qstr.isEmpty()){
        return DRAWER_OPEN_ERROR;
    }

    dBckGrndRetreat = qstr.toDouble();/*qstring to double*/

    /*Validamos que el ingreso o el retiro no sea menor que cero*/
    if(dBckGrndRetreat < 0)
        dBckGrndRetreat = 0;

    switch (nOption){
    case 0:{/*fondo*/
        dBckGrndRetreat = dBckGrndRetreatin + dBckGrndRetreat;
        break;}
    case 1:{/*retiro*/
        /*Antes de hacer el retiro*/
        if(dBckGrndRetreat < dBckGrndRetreatin){
            return DRAWER_EMPTY;
        }
        if(dBckGrndRetreat == 0){
            return DRAWER_EMPTY;
        }

        /*Restamos*/
        dBckGrndRetreat = dBckGrndRetreat - dBckGrndRetreatin;

        /*el fondo no puede quedar en cero*/
        if((int)dBckGrndRetreat < 0){
            //!Fk701sApp::mw->message_box(QString::fromUtf8("[ERROR] El fondo no puede quedar en cero."));
            return -5;
        }
        break;}
    default:
        rc = -6;
        return rc;
    }

    /*Convertimos a qstring y guardamos en la base de datos*/
    qstrRet = QString::number(dBckGrndRetreat,'f',2);
    array = qstrRet.toLocal8Bit();
    qstrRet.prepend("Total en fondo es = ");

    n = dBckGrndRetreat;
    do
    {
        ++numberOfDigt;
        n /= 10;

    }while(n);
#ifdef ARM
    snprintf(cvalue,numberOfDigt,"%f",dBckGrndRetreat);
#endif
#ifndef ARM
    _snprintf(cvalue,numberOfDigt,"%f",dBckGrndRetreat);
#endif
    rc = db.setDataBase(cKey,cvalue);
    if(rc != 0){
        rc = -1;
        return rc;
    }
    return rc;
}

int ctDrawer::getDrawer(QString &qstr)
{
    DataBase db;
    QString qstrKey = "Fondo_1";
    db.read_db(qstrKey,qstr);
    if(qstr.isEmpty()){
        return DRAWER_OPEN_ERROR;
    }

    return 0;
}
