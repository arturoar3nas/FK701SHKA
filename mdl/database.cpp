#include "mdl/database.h"

#ifdef ARM
const char * path_db_file = "/sd/Fk701s.db";/*General db*/
//const char * path_db_file = "/home/hy/DemoPos/Fk701s.db";
#endif
#ifndef ARM
//const char * path_db_file = "C:\\comparitdoubuntu\\DemoPos\\Fk701s.db";/*General db*/
//const char * path_db_file = "/home/hy/M3/linux-cortexm-1.10.0/A2F/qt-4.7.3/demos/DemoPos/Fk701s.db";
//const char * path_db_file = "/home/hy/DemoPos/Fk701s.db";
#endif
DataBase::DataBase()
{

}

QStringList DataBase::getDept(QString numero_dept)
{
    int  rc = -1;
    unqlite *pDb;
    unqlite_int64 nBytes;
    char zBuf[MAX_BUF];
    QStringList qstrlFail;
    qstrlFail.append("0");
    QRegExp delimiter("(\\{|\\}|\\,|\\\"|\\:|\\;)"); //RegEx for '{' '}' or ',' or '"' or ':' or ;

    /*Open our database*/
    rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){ return qstrlFail; }

    QByteArray ByteArray_num = numero_dept.toUtf8();
    char * cGet_Key =ByteArray_num.data();

    /*Extract record content*/
    nBytes = MAX_BUF;
    rc = unqlite_kv_fetch(pDb,cGet_Key,-1,zBuf,&nBytes);
    if( rc != UNQLITE_OK ){
        unqlite_close(pDb);
        return qstrlFail;
    }

    //Transformamos lo que nos devolvio unqlite en un string
    QString qstrTmp = QString::fromLocal8Bit((char *) zBuf);

    //Lo picamos en pedacitos (jjajajajajajajajaa xD)
    QStringList aqstrPlu = qstrTmp.split(delimiter);

    //Auto-commit the transaction and close our handle.
    unqlite_close(pDb);

    return aqstrPlu;
}

QStringList DataBase::getPlu(QString numero_plu)
{
    int  rc;
    unqlite *pDb;
    unqlite_int64 nBytes;
    char zBuf[MAX_BUF];
    memset(zBuf,0,sizeof(zBuf));
    QStringList qstrlFail;
    qstrlFail.append("0");

    QRegExp delimiter("(\\{|\\}|\\,|\\\"|\\:|\\;)"); //RegEx for '{' '}' or ',' or '"' or ':' or ;

    // Open our database;
    rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_READONLY | UNQLITE_OPEN_MMAP);
    if( rc != UNQLITE_OK ){ return qstrlFail; }

    /*QString to Char* */
    QByteArray ByteArray_num = numero_plu.toUtf8();
    char * cGet_Key =ByteArray_num.data();

    //Extract record content
    nBytes = MAX_BUF;
    rc = unqlite_kv_fetch(pDb,cGet_Key,-1,zBuf,&nBytes);
    if( rc != UNQLITE_OK ){
        unqlite_close(pDb);
        return qstrlFail;
    }

    //Transformamos lo que nos devolvio unqlite en un string
    QString qstrTmp=QString::fromLocal8Bit((char *) zBuf);

    //Lo picamos en pedacitos (jjajajajajajajajaa xD)
    QStringList aqstrPlu = qstrTmp.split(delimiter);

    //Auto-commit the transaction and close our handle.
    rc = unqlite_close(pDb);

    return aqstrPlu;
}


int DataBase::read_db(QString qstrkey, QStringList &qstrlarray)
{
    int  rc;
    unqlite *pDb;
    unqlite_int64 nBytes;
    char zBuf[MAX_BUF];
    QString qstrtmp;
    memset(zBuf,0,sizeof(zBuf));

    QRegExp delimiter("(\\{|\\}|\\,|\\\"|\\:|\\;)"); //RegEx for '{' '}' or ',' or '"' or ':' or ;

    /*Open our database*/
    rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK )
    {
        rc = unqlite_close(pDb);
        qDebug("[DEBUG][DATA BASE] unqlite_close: %d",rc);
        return rc;
    }

    /*QString to Char* */
    QByteArray ByteArray_num = qstrkey.toUtf8();
    char * cGet_Key =ByteArray_num.data();

    /*Extract record content*/
    nBytes = MAX_BUF;
    rc = unqlite_kv_fetch(pDb,cGet_Key,-1,zBuf,&nBytes);
    if( rc != UNQLITE_OK ){
      qDebug("[DEBUG][DATA BASE] rc = %d",rc);
      rc = unqlite_close(pDb);
      return rc;
    }

    /*Transformamos lo que nos devolvio unqlite en un string*/
    qstrtmp = QString::fromLocal8Bit((char *) zBuf);
    qstrlarray = qstrtmp.split(delimiter);

    /*Auto-commit the transaction and close our handle.*/
    rc = unqlite_close(pDb);

    return rc;
}

int DataBase::read_db(QString qstrkey, QString &qstr)
{
    int  rc = -1;
    unqlite *pDb;
    unqlite_int64 nBytes;
    char zBuf[MAX_BUF] = {0};

    /*Open our database*/
    rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){
        rc = unqlite_close(pDb);
        qDebug("[DEBUG][DATA BASE] unqlite_close: %d",rc);
        return rc;
    }

    /*QString to Char* */
    QByteArray by = qstrkey.toUtf8();
    char *cGet_Key = by.data();

    /*Extract record content*/
    nBytes = MAX_BUF;
    rc = unqlite_kv_fetch(pDb,cGet_Key,-1,zBuf,&nBytes);
    if( rc != UNQLITE_OK ){
      qDebug("[DEBUG][DATA BASE] rc = %d",rc);
      rc = unqlite_close(pDb);
      return rc;
    }

    /*Transformamos lo que nos devolvio unqlite en un string*/
    qstr = QString::fromLocal8Bit((char *) zBuf);

    /*Auto-commit the transaction and close our handle.*/
    rc = unqlite_close(pDb);

    return rc;
}
int DataBase::setDataBase(char * key,char * data)
{
     unqlite *pDb;
     int  rc;
     int  rc1;
     int resp = 0;

     qDebug("[DEBUG] [DATABASE][setDatabase] key = %s ",key);
     qDebug("[DEBUG] [DATABASE][setDatabase] data = %s ",data);
     // Open our database;
     rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_CREATE);
     if( rc != UNQLITE_OK ){ return -1; }
     qDebug("[DEBUG][DATA BASE][setDatabase][rc] unqlite_open: %d",rc);

    // Store records
    rc = unqlite_kv_store(pDb,key,-1,data,strlen(data));
    qDebug("[DEBUG][DATA BASE][setDatabase][rc] unqlite_open: %d",rc);

    if( rc != UNQLITE_OK ){
      if( rc != UNQLITE_BUSY && rc != UNQLITE_NOTIMPLEMENTED ){
        /* Rollback */
      unqlite_rollback(pDb);
      }
      resp =-1;
    }

    rc1=unqlite_commit(pDb);
    qDebug("[DEBUG][DATA BASE][setDatabase][rc1] unqlite_commit: %d",rc1);

    //Auto-commit the transaction and close our handle.
    rc1=unqlite_close(pDb);
    qDebug("[DEBUG][DATA BASE][setDatabase][rc1] unqlite_close: %d",rc1);

    return resp;
}

int DataBase::iterateOverDb(char* ptrCmp , char**& ptrBuf)
{

    unqlite *pDb;                /* Database handle */
    int rc;
    char* pBuf;
    int i = 0;

    /*key*/
    pBuf = (char*)malloc(1);

    // Open our database;
    rc = unqlite_open(&pDb,path_db_file,UNQLITE_OPEN_CREATE);
    if( rc != UNQLITE_OK ){ return -1; }

    /* Iterate over the inserted records */
    unqlite_kv_cursor *pCur;

    /* Allocate a new cursor instance */
    rc = unqlite_kv_cursor_init(pDb,&pCur);
    if( rc != UNQLITE_OK ){
        unqlite_close(pDb);
        return -1;
    }

    /* Point to the first record */
    unqlite_kv_cursor_first_entry(pCur);

    /* Iterate over the entries */
    while( unqlite_kv_cursor_valid_entry(pCur) )
    {
        int nKeyLen;

        /* Consume the key */
        unqlite_kv_cursor_key(pCur,0,&nKeyLen); /* Extract key length */
        qDebug("\nKey ==> %u\n\t",nKeyLen);

        /*Realloc size*/
        pBuf = (char*)realloc(0,nKeyLen);

        unqlite_kv_cursor_key(pCur,pBuf,&nKeyLen); /* Extract key */
        qDebug("\npBuf ==> %s\n\t",pBuf);

        if(strncmp(pBuf,ptrCmp,4) == 0)
        {
            qDebug("\ncompare OK! ==> %s\n\t",pBuf);
            ptrBuf[i] = pBuf;
            i++;
        }

        /* Point to the next entry */
        unqlite_kv_cursor_next_entry(pCur);
    }

    for(int j = 0; j < i; j++)qDebug("\nptrBuf array ==> %s\n\t",ptrBuf[j]);

    /* Finally, Release our cursor */
    unqlite_kv_cursor_release(pDb,pCur);


    /* All done, close our database */
    unqlite_close(pDb);

    free(pBuf);

    return i;
}

