#include "xmode.h"

XMode::XMode()
{


}

int XMode::close_x()
{
    Epson e;
    int rt = e.Reporte_X();
    return rt;
}

int XMode::report_x(int nOption, QStringList &qstrl)
{
    switch(nOption){
        case 0:report_department(qstrl);break;
        case 1:report_cashier(qstrl);break;
        case 2:/*Mozo*/break;
        case 3:report_plu(qstrl);break;
        case 4:/*Venta P*/break;/*comando 0803*/
        default:break;}
    return EXIT_SUCCESS;
}

int XMode::stock(QStringList &qstrl)
{
    char** ptrBuf;
    ptrBuf = new char* [1024];
    char* ptrCmp = (char*)"PLU_";
    QStringList qstrltmp;
    DataBase db;

    int ret = db.iterateOverDb(ptrCmp,ptrBuf);/*iteramos a traves de la base de datos*/

    qstrl.append("Descripcion;0;0;0;0;0;Precio;0;0;0;Inventario;0;0;0;0;0;Departamento;Codigo");

    for(int j = 0; j < ret; j++){/*we send key by key*/
        QString qstrkey = QString::fromUtf8(ptrBuf[j]);
        db.read_db(qstrkey,qstrltmp);
        if(qstrltmp.isEmpty()){
            return FAIL;
        }
        qstrl.append(qstrltmp.join(";")+qstrkey);
    }
    delete[] ptrBuf;

    return SUCESS;
}

int XMode::setStock(Operation *op, QStringList &strlPlu)
{
    QString strQuanty = QString::number(op->quanty(),'f',0);
    QString strPluKey = op->plu();
    strPluKey.prepend("PLU_");
    strPluKey.remove(";");
    DataBase db;
    qDebug() << strPluKey;
    qDebug() << strQuanty;

    db.read_db(strPluKey,strlPlu);
    if (!strlPlu.isEmpty() && (strlPlu.at(0) !="0")){
        strlPlu[10] = strQuanty;
        QByteArray byPlu = strlPlu.join(";").toLocal8Bit();
        char *ptrPlu = byPlu.data();
        QByteArray byKey = strPluKey.toLocal8Bit();
        char *ptrKey = byKey.data();
        db.setDataBase(ptrKey,ptrPlu);
        return SUCESS;
    }
    else
       return PLU_NO_FOUND;

    return SUCESS;
}

int XMode::garage(QStringList &qstrl)
{

    return 0;
}

int XMode::deleteGarage(QStringList &qstrl)
{
    return 0;
}

int XMode::declareBox(QStringList &qstrl)
{
    return 0;
}

int XMode::saleBook(QStringList &qstrl)
{
    /*comando 08 0b en 01*/
    return 0;
}

int XMode::report_plu(QStringList &qstrl)
{
    Reports Rp;
    Rp.getReportPlu(qstrl);/*llamamos al metodo*/
    return SUCESS;
}

int XMode::report_department(QStringList &qstrl)
{
    Reports Rp;
    Rp.getReportDept(qstrl);/*llamamos al metodo*/
    return SUCESS;
}

int XMode::report_cashier(QStringList &qstrl)
{
    Reports Rp;
    Rp.getReportCashier(qstrl);/*llamamos al metodo*/
    return SUCESS;
}

int XMode::report_mozo(QStringList &qstrl)
{
    return 0;
}

