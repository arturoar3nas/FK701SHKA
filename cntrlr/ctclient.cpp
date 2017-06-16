#include "ctclient.h"

ctClient::ctClient()
{

}

int  ctClient::create(QStringList qstrl, Client *&c)
{
    QString qstrkey;
    QString qstrdata;
    QStringList qstrlclnt;
    QString s0,s1,s2,s3,s4;
    DataBase db;
    char thecKey [32] = {0};
    char clienTmp[128] = {0};
    int rc = -1;

    /*check cases*/
    if((qstrl.at(0).isEmpty())&&(!qstrl.at(1).isEmpty())
            &&(qstrl.at(2).isEmpty())&&(qstrl.at(3).isEmpty())
            &&(qstrl.at(4).isEmpty())){
        /*Validamos que no existe el usuario en la base de datos
          Si existe se carga el cliente en la gui*/
        qstrkey.append("CLIENT_");
        qstrkey.append(qstrl.at(1));
        int ret = db.read_db(qstrkey,qstrlclnt);
        if(ret < 0){
            return CLIENT_EMPTY;
        }
        return SUCESS;
    }
    else if(qstrl.at(0).isEmpty()){
        return CLIENT_EMPTY_ID;
    }
    else if(qstrl.at(1).isEmpty()){
        return CLIENT_EMPTY_NDOCUMENT;
    }
    else if(qstrl.at(2).isEmpty()){
        return CLIENT_EMPTY_RESP;
    }
    else if(qstrl.at(3).isEmpty()){
        return CLIENT_EMPTY_RSOCIAL;
    }
    else if(qstrl.at(4).isEmpty()){
        return CLIENT_EMPTY_ADDRESS;
    }
    else
    {
        /*Validamos el cliente*/
        if(qstrl.at(1).contains(QRegExp("[a-zA-Z-.]"))){
            return NO_LETTER_ERROR;
        }
        if(qstrl.at(1).length() > 11){
            return LENGTH_NDOC_ERROR;
        }
        if(qstrl.at(3).length() > 40){
            return LENGTH_RSOC_ERROR;
        }
        if(qstrl.at(0).contains(QRegExp("[0-9a-zABDF-KMNOUW-Z-.]"))){
            return CLIENT_ID_ERROR;
        }
        if(qstrl.at(0).length() > 1){
            return LENGHT_ID_ERROR;
        }
        if(qstrl.at(2).contains(QRegExp("[0-9a-zA-DGHJ-LOQ-SUW-Z-.]"))){
            return CLIENT_RESP_ERROR;
        }
        if(qstrl.at(2).length() > 1){
            return LENGTH_RESP;
        }
        if(qstrl.at(4).length() > 100){
            return LENGHT_ADDRESS;
        }

        /*lo enviamos a la db tmp*/
        qstrdata = qstrl.join("\n");

        /*Contruimos el Key*/
        int index = qstrl.at(0).toInt();
#ifndef ARM
        _snprintf(thecKey , sizeof thecKey ,"CLIENT_%d",index);
#endif
#ifdef ARM
        snprintf(thecKey , sizeof thecKey ,"CLIENT_%d",index);
#endif
        s0 = qstrl.at(0).toLatin1();
        s1 = qstrl.at(3).toLatin1();
        s2 = qstrl.at(2).toLatin1();
        s3 = qstrl.at(4).toLatin1();
        s4 = qstrl.at(1).toLatin1();

        /*creamos la trama de medios de pago*/
#ifndef ARM
        _snprintf(clienTmp, sizeof clienTmp,"%s;%s;%s;%s;%s;",
                 s0.toLocal8Bit().constData(),
                 s1.toLocal8Bit().constData(),
                 s2.toLocal8Bit().constData(),
                 s3.toLocal8Bit().constData(),
                 s4.toLocal8Bit().constData());
#endif
#ifdef ARM
        snprintf(clienTmp, sizeof clienTmp,"%s;%s;%s;%s;%s;",
                 s0.toLocal8Bit().constData(),
                 s1.toLocal8Bit().constData(),
                 s2.toLocal8Bit().constData(),
                 s3.toLocal8Bit().constData(),
                 s4.toLocal8Bit().constData());
#endif
        /*luego llamamos a la base de datos*/
        rc = db.setDataBase(thecKey,clienTmp);
        if(rc < 0){
            rc = FAIL;
            return rc;
        }
        c->setId(&qstrl[0]);
        c->setNDocument(&qstrl[1]);
        c->setResp(&qstrl[2]);
        c->setRazonSocial(&qstrl[3]);
        c->setAddress(&qstrl[4]);
    }

    return SUCESS;
}
