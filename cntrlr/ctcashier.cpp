#include "ctcashier.h"

ctCashier::ctCashier()
{
    bCashier = false;
    msr = new MagneticStripReader;
}

int ctCashier::handle()
{
    return FAIL;
}

int ctCashier::Start(QStringList qstrlCashier)
{
    QStringList qstrl;
    QString qstrusrlogin;
    QString qstrpsw;
    int rc = -1;
    DataBase db;

    if(qstrlCashier.at(0).isEmpty()||qstrlCashier.at(1).isEmpty()){
        return CASHIER_OPEN_ERROR;
    }

    /*Con el numero de cajero vamos a la base de datos
      Acomodamos el numero del cajero*/
    qstrusrlogin = "CASHIER_" + qstrlCashier.at(0);
    rc = db.read_db(qstrusrlogin,qstrl);
    if(rc < 0){
        return FAIL;
    }
    qstrpsw = qstrl.at(1);
    if(qstrpsw.isEmpty()){
        /*si el cajero no existe arrojamos error*/
        return CASHIER_EMPTY;
    }

    /*si existe el numero de cajero comparamos la contrasena*/
    rc = qstrlCashier[1].compare(qstrpsw);
    if( rc != 0){
        /*si la contrasena no coincide arrojamos error*/
        return PASSWORR_INVALID;
    }

    if(qstrl.at(0).toInt() == 1){/*si es el cajero con max privilegios*/
       QString qstrMsr;
       rc = msr->readMsr(qstrMsr);
       if(rc < SUCESS){
           return MSR_ERROR;
       }
       //![0]
       //! Compare MSR
       //![0]
    }

    /*si la contrasena coincide entonces habilitamos las ventas y seteamos los flags*/
    bCashier = true;
    csh = new Cashier();
    csh->setNNumber(qstrl.at(0).toInt());
    csh->setNpswd(qstrl.at(1).toInt());
    csh->setQstrName(qstrl.at(2));
    csh->setNlevel(qstrl.at(3).toInt());

    return SUCESS;
}

int ctCashier::Close(QStringList &qstrl)
{
    int rc = -1;
    /*obtenemos los datos de la gui*/
    if(qstrl.at(0).isEmpty() && qstrl.at(1).isEmpty()){
        return SUCESS;
    }

    /*Si ingresaron cero cerramo el cajero*/
    rc = qstrl.at(0).compare("0");
    if( rc != 0){
        /*si la contrasena no coincide arrojamos error*/
        return INPUT_INVALID;
    }

    /*limpiamos todo*/
    bCashier = false;
    return rc;
}
