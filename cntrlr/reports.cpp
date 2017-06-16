#include "reports.h"

//#ifdef ARM
//const char * path_db_file = "/sd/Fk701s.db";/*General db*/
const char * path_db_report_file = "/sd/Fk701sTrade.db";/*Dept y cajeros*/
const char * path_db_report_file_plu = "/sd/Fk701sTradeP.db";/*PLU*/
////const char * path_db_file = "/home/hy/DemoPos/Fk701s.db";
//#endif

using namespace std;

Reports::Reports()
{

}

/**
 * @brief Reports::updateTrade
 * @return
 * Metodo para cargar transacciones en
 * base de datos
 */
int Reports::updateTrade(DataBase *&oDb)
{
    QByteArray qby,
               qbyP;
    QStringList qstrlplu_tmp,
                qstrljson;
    QString qstrDeptnum,
            qstrquantity,
            qstrprice,
            qstrsold,
            qstr,
            qstrobjjason,
            qstrnumplu,
            qstrsubplu,
            qstrfromlist;
    bool bOk = false;
    bool bPlu = false;
    JsonArray ajson;
    JsonObject json;
    JsonObject jsonP;
    unqlite *pDb;
    unqlite_kv_cursor *pCur;
    unqlite *pDbP;
    int  rc = 0,
         n = 0,
         nIterator = 0;
    char* pdata;
    char* pdataP;
    int anSold[99] = {0};/*99 departamentos*/
    char acKey [16]   = {0};
    char acIndex[16]  = {0};
    QString subqstr;

    QStringList qstrlpluTmp;
    oDb->read_db("",qstrlpluTmp);
    qDebug() << "\n qstrlpluTmp ==> \n" << qstrlpluTmp;
    QStringList qstrlclientTmp;
    oDb->read_db("",qstrlclientTmp);

    int ncashier;// = oDb->getnCashier();

    /*leemos del temporal de venta y llenamos el array de ventas*/
    foreach(const QString &str,qstrlpluTmp)
    {
        qDebug("[DEBUG][Reports] plu_tmp = %s",str.toLatin1().constData());
        qstrlplu_tmp = str.split(";");
        /*extraemos los datos necesarios*/
        if(qstrlplu_tmp.count() > 16){/*si es un plu preguntamos de que dept es*/
            qstrDeptnum = qstrlplu_tmp.at(16);/*plu*/
            subqstr = qstrDeptnum;
            /*sacamos el numero de plu*/
            qstrnumplu = qstrlplu_tmp.at(17);
            qstrsubplu = qstrnumplu.mid(4,10);
            bPlu = true;
        }
        else{
            qstrDeptnum = qstrlplu_tmp.at(14);subqstr = qstrDeptnum.mid(5,10);
        }

        qstrquantity      = qstrlplu_tmp.at(5);/*extraemos el total vendido por ese dept*/
        qstrprice         = qstrlplu_tmp.at(6);
        qstrprice.remove("."); /*si tiene un punto lo quitamos*/
        _calctotal(qstrquantity,qstrprice,qstrsold);/*calculamos cuanto se hizo en ese departamento*/
        n = subqstr.toInt();/*cargamos los datos en un array*/
        anSold[n] += qstrsold.toInt();

        /*si es un plu lo cargamos director en el json*/
        if(bPlu){
            jsonP["Numero"] = qstrsubplu;
            jsonP["Precio"] = qstrquantity;
            jsonP["Tax"]    = qstrlplu_tmp.at(7);
            jsonP["sold"]   = qstrsold;
            qstrljson.append(QtJson::serialize(jsonP));/*vamos guardano los obejost en una lista*/
            jsonP.clear();/*limpiamos el objeto*/
        }
    }

    if(bPlu){/*pasamos el qstringlist a char* de los plu*/
       qDebug() << "\nqstrljson ==> \n" << qstrljson;
       qstrfromlist = qstrljson.join("");
       qDebug() << "\nqstrfromlist ==> \n" << qstrfromlist;
       qbyP = qstrfromlist.toLocal8Bit();
       pdataP = qbyP.data();
    }

    if(!qstrlclientTmp.isEmpty()){/*Cargamos el objeto json*/
        json["cuit"]    = qstrlclientTmp.at(0);
        json["rznscl"]  = qstrlclientTmp.at(1);
        json["id"]      = qstrlclientTmp.at(2);
        json["Resp"]    = qstrlclientTmp.at(3);
        json["dmclio"]  = qstrlclientTmp.at(4);
        json["cajero"]  = ncashier;
    }

    for(int i = 0; i < 100; i++){/*Vaciamos el array int en el array json*/
        if(anSold[i] != 0){
            qstr = QString::number(i,10);
            qstr.prepend("Dept_");
            ajson = JsonArray() << i << anSold[i];/*copiamos los resultados a json*/
            json[qstr] = ajson;
        }
    }

    /*Convertimos el objeto json a qstring*/
    qstrobjjason =  QtJson::serializeStr(json,bOk);
    if(!bOk){qDebug("[ERROR][Reports][UPDATETRADE] No se pudo serializar <<<<<<<<<<<<< ");return -1;}

    /*lo convertimos a char* */
    qby = qstrobjjason.toLocal8Bit();
    pdata = qby.data();
    qDebug() << "\ndata :" << pdata;

    /*Abrimos la base de datos*/
    rc = unqlite_open(&pDb,path_db_report_file,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){ return -1; }

    if(bPlu){
        rc = unqlite_open(&pDbP,path_db_report_file_plu,UNQLITE_OPEN_CREATE);
        if( rc != UNQLITE_OK ){ return -1; }
    }

    /* Allocate a new cursor instance */
    rc = unqlite_kv_cursor_init(pDb,&pCur);
    if( rc != UNQLITE_OK ){
        /*Fatal(0,"Out of memory");*/
        unqlite_close(pDb);
        return -1;
    }

    /* Point to the last record */
    unqlite_kv_cursor_last_entry(pCur);

    /* Iterate over the records */
    while( unqlite_kv_cursor_valid_entry(pCur) ){
        unqlite_kv_cursor_prev_entry(pCur);/* Point to the previous record */
        nIterator++;
    }
    nIterator++;
    sprintf(acKey,"%d",ncashier);/*copiamos el numero de cajero en la primera posicion*/
    sprintf(acIndex,"%ld",nIterator);
    strncat(acKey,acIndex,strlen(acIndex)+1);
    qDebug() << "\nkey ==> " << acKey;

    /*guardamos el qstring*/
    rc = unqlite_kv_store(pDb,acKey,-1,pdata,strlen(pdata));
    qDebug("[DEBUG][DATA BASE][setReports] unqlite_store: %d",rc);
    if( rc != UNQLITE_OK ){
        if( rc != UNQLITE_BUSY && rc != UNQLITE_NOTIMPLEMENTED ){
            /* Rollback */
            unqlite_rollback(pDb);
        }
        rc = -1;
    }
    if(bPlu){
        rc = unqlite_kv_store(pDbP,acKey,-1,pdataP,strlen(pdataP));
        qDebug() << "\ndata ==> " << pdataP;
        qDebug("[DEBUG][DATA BASE][setReports] unqlite_store: %d",rc);
        if( rc != UNQLITE_OK ){
            if( rc != UNQLITE_BUSY && rc != UNQLITE_NOTIMPLEMENTED ){
                /* Rollback */
                unqlite_rollback(pDb);
            }
            rc = -1;
        }
    }

    /* Finally, Release our cursor */
    unqlite_kv_cursor_release(pDb,pCur);

    /*cerramos base de datos*/
    unqlite_close(pDb);

    /*cerramos base de datos*/
    if(bPlu)
        unqlite_close(pDbP);

    return rc;
}

/**
 * @brief Reports::getReportDept
 * @param qstrl
 * @return
 * Metodo para generar reportes
 * por Departamento
 * Numero de Departamento
 * Cantidad Vendida
 * Impuesto asociado al departamento
 */
int Reports::getReportDept(QStringList &qstrl)
{
    QString qstrobjjason,
            qstrindex,
            qstrtmp;
    bool bOk = false;
    JsonArray jsonlist;
    JsonObject json;
    unqlite *pDb;
    unqlite_kv_cursor *pCur;
    unqlite_int64 iData;
    int  rc       = 0,
         nKeyLen  = 0,
         nIterator = 0;
    char* pBuf;
    char* pBufData;
    int ansold[99] = {0};/*99 departamentos*/

    /*leemos todas las transacciones*/
    rc = unqlite_open(&pDb,path_db_report_file,UNQLITE_OPEN_CREATE);/*Abrimos la base de datos*/
    if( rc != UNQLITE_OK ){
        return -1;
    }

    rc = unqlite_kv_cursor_init(pDb,&pCur);/* Allocate a new cursor instance */
    if( rc != UNQLITE_OK ){
        /*Fatal(0,"Out of memory");*/
        unqlite_close(pDb);
        return -1;
    }

    /* Point to the last record */
    unqlite_kv_cursor_last_entry(pCur);

    while( unqlite_kv_cursor_valid_entry(pCur) ){/* Iterate over the records */
        /* Consume the key */
        unqlite_kv_cursor_key(pCur,0,&nKeyLen); /* Extract key length */
        //qDebug("\nKey ==> %u\n\t",nKeyLen);

        /*Realloc size*/
        pBuf = (char*)realloc(0,nKeyLen);
        unqlite_kv_cursor_key(pCur,pBuf,&nKeyLen); /* Extract key */
        //qDebug("\npBuf ==> %s\n\t",pBuf);

        /*sumamos en un array los montos*/
        /* Extract data length */
        unqlite_kv_cursor_data(pCur,NULL,&iData);
        //printf("\nData length ==> %lld\n\t",iData);

        pBufData = (char*)realloc(0,iData);
        unqlite_kv_cursor_data(pCur,pBufData,&iData); /* Extract key */
        //qDebug("\npBufData ==> %s\n\t",pBufData);

        /*convertimos de char* a qstring*/
        qstrobjjason = QString::fromUtf8(pBufData,iData);
        //qDebug("\nqstrobjjason ==> %s\n\t",pBufData);

        /*Convertimos a json*/
        json = QtJson::parse(qstrobjjason, bOk).toMap();
        //printResult(json);/*vemos como quedo el obj*/

        /*extraemos lo que necesitamos*/
        for(int i = 1;i < 100;i++){
            qstrindex = QString::number(i,10);
            qstrindex.prepend("Dept_");
            jsonlist = json[qstrindex].toList();
            if(!jsonlist.isEmpty())
                ansold[i] += jsonlist.at(1).toInt();
        }

        unqlite_kv_cursor_prev_entry(pCur);/* Point to the previous record */
        qDebug() << "iterator ==> " << nIterator;
        nIterator++;
    }

    /*acomodamos el resultado*/
    for(int i = 1;i < 100;i++){
        qstrindex = QString::number(i,10);
        qstrindex.prepend("Deptartamento");
        qstrl.append(qstrindex);
        qstrtmp = QString::number(ansold[i],10);
        qstrl.append("    ");
        qstrtmp.prepend("Vendido : ");
        qstrl.append(qstrtmp);
        qstrl.append("\n");
    }

    /* Finally, Release our cursor */
    unqlite_kv_cursor_release(pDb,pCur);

    /*cerramos base de datos*/
    unqlite_close(pDb);
    return 0;
}

/**
 * @brief Reports::getReportCashier
 * @param qstrl
 * @return
 * Metodo para generar reportes
 * por Departamento
 * Numero
 * Venta x Dept
 */
int Reports::getReportCashier(QStringList &qstrl)
{
    QString qstrobjjason;
    QString qstrindex;
    QString qstrcashier;
    bool ok = false;
    JsonArray jsonlist;
    JsonObject json;
    unqlite *pDb;
    unqlite_kv_cursor *pCur;
    unqlite_int64 iData;
    int  rc       = 0,
         nKeyLen  = 0,
         iterator = 0;
    int cashiertype = 0;
    char  Cashiernum[1]={0};
    char* pBuf;
    char* pBufData;
    int     soldarray_1[99] = {0},
            soldarray_2[99] = {0},
            soldarray_3[99] = {0},
            soldarray_4[99] = {0},
            soldarray_5[99] = {0},
            soldarray_6[99] = {0},
            soldarray_7[99] = {0},
            soldarray_8[99] = {0},
            soldarray_9[99] = {0};/*99 departamentos*/


    /*leemos todas las transacciones*/
    rc = unqlite_open(&pDb,path_db_report_file,UNQLITE_OPEN_CREATE);/*Abrimos la base de datos*/
    if( rc != UNQLITE_OK ){ return -1; }

    rc = unqlite_kv_cursor_init(pDb,&pCur);/* Allocate a new cursor instance */
    if( rc != UNQLITE_OK ){
        /*Fatal(0,"Out of memory");*/
        unqlite_close(pDb);
        return -1;
    }

    /* Point to the last record */
    unqlite_kv_cursor_last_entry(pCur);

    while( unqlite_kv_cursor_valid_entry(pCur) ){/* Iterate over the records */
        /* Consume the key */
        unqlite_kv_cursor_key(pCur,0,&nKeyLen); /* Extract key length */
        //qDebug("\nKey ==> %u\n\t",nKeyLen);

        /*Realloc size*/
        pBuf = (char*)realloc(0,nKeyLen);
        unqlite_kv_cursor_key(pCur,pBuf,&nKeyLen); /* Extract key */
        //qDebug("\npBuf ==> %s\n\t",pBuf);

        /*extraemos el numero de cajero*/
        memcpy(Cashiernum,&pBuf[0],1);
        cashiertype = atoi(Cashiernum);

        /*sumamos en un array los montos*/
        /* Extract data length */
        unqlite_kv_cursor_data(pCur,NULL,&iData);
        //printf("\nData length ==> %lld\n\t",iData);

        pBufData = (char*)realloc(0,iData);
        unqlite_kv_cursor_data(pCur,pBufData,&iData); /* Extract key */
        //qDebug("\npBufData ==> %s\n\t",pBufData);

        /*convertimos de char* a qstring*/
        qstrobjjason = QString::fromUtf8(pBufData,iData);
        //qDebug("\nqstrobjjason ==> %s\n\t",pBufData);

        /*Convertimos a json*/
        json = QtJson::parse(qstrobjjason, ok).toMap();
        //printResult(json);/*vemos como quedo el obj*/

        /*extraemos lo que necesitamos*/
        for(int i = 1;i < 100;i++){
            qstrindex = QString::number(i,10);
            qstrindex.prepend("Dept_");
            jsonlist = json[qstrindex].toList();
            if(!jsonlist.isEmpty()){
                switch(cashiertype)
                {
                case 1:soldarray_1[i] += jsonlist.at(1).toInt();break;
                case 2:soldarray_2[i] += jsonlist.at(1).toInt();break;
                case 3:soldarray_3[i] += jsonlist.at(1).toInt();break;
                case 4:soldarray_4[i] += jsonlist.at(1).toInt();break;
                case 5:soldarray_5[i] += jsonlist.at(1).toInt();break;
                case 6:soldarray_6[i] += jsonlist.at(1).toInt();break;
                case 7:soldarray_7[i] += jsonlist.at(1).toInt();break;
                case 8:soldarray_8[i] += jsonlist.at(1).toInt();break;
                case 9:soldarray_9[i] += jsonlist.at(1).toInt(); break;
                default:break;
                }
            }
        }
        unqlite_kv_cursor_prev_entry(pCur);/* Point to the previous record */
        qDebug() << "iterator ==> " << iterator;
        iterator++;
    }

    /*acomodamos el resultado*/
    QString qstrtmp;
    for(int ii = 1; ii < 10 ; ii++){
        qstrcashier = QString::number(ii,10);
        qstrcashier.prepend(">>>>>>>>> Cajero ");
        qstrl.append(qstrcashier);
        for(int i = 1;i < 100;i++){
            qstrindex = QString::number(i,10);
            qstrindex.prepend("Departamento ");
            qstrl.append(qstrindex);
            switch(ii)
            {
                case 1:qstrtmp = QString::number(soldarray_1[i],10);break;
                case 2:qstrtmp = QString::number(soldarray_2[i],10);break;
                case 3:qstrtmp = QString::number(soldarray_3[i],10);break;
                case 4:qstrtmp = QString::number(soldarray_4[i],10);break;
                case 5:qstrtmp = QString::number(soldarray_5[i],10);break;
                case 6:qstrtmp = QString::number(soldarray_6[i],10);break;
                case 7:qstrtmp = QString::number(soldarray_7[i],10);break;
                case 8:qstrtmp = QString::number(soldarray_8[i],10);break;
                case 9:qstrtmp = QString::number(soldarray_9[i],10);break;
                default:break;
            }
            qstrtmp.prepend("Vendido : ");
            qstrl.append(qstrtmp);
        }
    }

    /* Finally, Release our cursor */
    unqlite_kv_cursor_release(pDb,pCur);

    /*cerramos base de datos*/
    unqlite_close(pDb);

    return 0;
}

/**
 * @brief Reports::getReportPlu
 * @param qstrl
 * @return
 * Metodo para generar reportes
 * por PLU
 * numero
 * precio
 * impuesto
 * sold
 */
int Reports::getReportPlu(QStringList &qstrl)
{
    QStringList qstrljsonobj;
    QString qstrobjjason,
            qstrtmp;
    bool bOk = false;
    JsonObject json;
    unqlite *pDb;
    unqlite_kv_cursor *pCur;
    unqlite_int64 iData;
    int  rc       = 0,
         nKeyLen  = 0,
         nIterator = 0;
    char* pBuf;
    char* pBufData;
    QMap<int,int>mpnsold;
    QMap<int,QString>mpqstrtax;
    QMap<int,int>mpnquanty;

    /*leemos todas las transacciones*/
    rc = unqlite_open(&pDb,path_db_report_file_plu,UNQLITE_OPEN_CREATE);/*Abrimos la base de datos*/
    if( rc != UNQLITE_OK ){ return -1; }

    rc = unqlite_kv_cursor_init(pDb,&pCur);/* Allocate a new cursor instance */
    if( rc != UNQLITE_OK ){
        /*Fatal(0,"Out of memory");*/
        unqlite_close(pDb);
        return -1;
    }

    /* Point to the last record */
    unqlite_kv_cursor_last_entry(pCur);

    while( unqlite_kv_cursor_valid_entry(pCur) ){/* Iterate over the records */
        /* Consume the key */
        unqlite_kv_cursor_key(pCur,0,&nKeyLen); /* Extract key length */

        /*Realloc size*/
        pBuf = (char*)realloc(0,nKeyLen);
        unqlite_kv_cursor_key(pCur,pBuf,&nKeyLen); /* Extract key */
        qDebug("\npBuf ==> %s\n\t",pBuf);

        /*sumamos en un array los montos*/
        unqlite_kv_cursor_data(pCur,NULL,&iData);/* Extract data length */

        pBufData = (char*)realloc(0,iData);
        unqlite_kv_cursor_data(pCur,pBufData,&iData); /* Extract data */
        qDebug("\npBufData ==> %s\n\t",pBufData);

        /*convertimos de char* a qstring*/
        qstrobjjason = QString::fromUtf8(pBufData,iData);
        qDebug("\nqstrobjjason ==> %s\n\t",pBufData);

        /*qstring to qstringlist*/
        qstrobjjason.replace("}","};");/*para hacer split a ; no a }*/
        qstrljsonobj = qstrobjjason.split(";");

        int nkeys = 0;

        foreach(const QString &str, qstrljsonobj){
           qDebug() << "\nqstrljsonobj ==> \n" << str;
           json = QtJson::parse(str, bOk).toMap();/*convertimos a json*/
           qDebug() << "\nok ==>\n" << bOk;
           _printResult(json);/*vemos como quedo el obj*/
           nkeys = json["Numero"].toInt();
           mpnsold[nkeys] += json["sold"].toInt();
           mpqstrtax[nkeys] = " Tax: " + json["Tax"].toString();
           mpnquanty[nkeys] += json["Precio"].toInt();
           qDebug() << "\nplu[]= \n" << mpnsold[nkeys] << endl;
        }

        unqlite_kv_cursor_prev_entry(pCur);/* Point to the previous record */
        qDebug() << "iterator ==> " << nIterator;
        nIterator++;
    }

    /*acomodamos el resultado*/
    QMap<int, int>::const_iterator i = mpnsold.constBegin();
    QMap<int, int>::const_iterator ii = mpnquanty.constBegin();
    QMap<int, QString>::const_iterator iii = mpqstrtax.constBegin();
    while (i != mpnsold.constEnd()) {
        qstrtmp.append("Numero de PLU: ");
        qstrtmp.append(QString::number(i.key(),10));
        qstrl.append(qstrtmp);
        qstrtmp.clear();
        qstrtmp.append("Vendido: ");
        qstrtmp.append(QString::number(i.value(),10));
        qstrl.append(qstrtmp);
        qstrtmp.clear();
        qstrtmp.append("Cantidad: ");
        qstrtmp.append(QString::number(ii.value(),10));
        qstrl.append(qstrtmp);
        qstrtmp.clear();
        qstrtmp.append(iii.value());
        qstrl.append(qstrtmp);
        qstrtmp.clear();
        ++i;
        ++ii;
        ++iii;
    }

    /* Finally, Release our cursor */
    unqlite_kv_cursor_release(pDb,pCur);

    /*cerramos base de datos*/
    unqlite_close(pDb);

    return 0;
}

/**
 * @brief Reports::_calctotal
 * @param quanty
 * @param price
 * @param sold
 */
void Reports::_calctotal(QString quanty, QString price, QString &sold)
{
    int iquanty = quanty.toInt();
    int iprice = price.toInt();
    int isold = iquanty * iprice;
    sold = QString::number(isold,10);
}

/**
 * @brief Reports::_printResult
 * @param result
 */
void Reports::_printResult(const JsonObject &result) {
    qDebug() << "encoding:" << result["encoding"].toString();
    qDebug() << "plugins:";

    JsonArray plugins = result["plug-ins"].toList();
    foreach(QVariant plugin, plugins) {
        qDebug() << "  -" << plugin.toString();
    }

    JsonObject indent = result["indent"].toMap();
    qDebug() << "length:" << indent["length"].toInt();
    qDebug() << "use_space:" << indent["use_space"].toBool();
}

/**
 * @brief Reports::_printJson
 * @param result
 */
void Reports::_printJson(const JsonObject &result)
{
    qDebug() << QtJson::serialize(result);
}

void Reports::testMap()
{

    QMap<int, QString> Employees;

    Employees[5234] = "Mike C.";
    Employees[3374] = "Charlie M.";
    Employees[1923] = "David D.";
    Employees[7582] = "John A.";
    Employees[5328] = "Peter Q.";
    qDebug() << "Employees[3374]= " << Employees[3374] << endl;
    qDebug() << "Map size: " << Employees.size() << endl;

    QMap<int, QString>::const_iterator i = Employees.constBegin();
    while (i != Employees.constEnd()) {
        qDebug() << i.key() << ": " << i.value() << endl;
        ++i;
    }
}

