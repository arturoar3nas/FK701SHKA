#ifndef EPSON_H
#define EPSON_H

#include <stdio.h>


#include "errno.h"
#include <fcntl.h>
#include <stdint.h>
#include <QStringList>
#include <iostream>
#include <string>
#include <stdlib.h>

#include <QStringList>
#include <QString>
#include <QThread>
#include <QDebug>
#include "util.h"
#ifdef ARM
#include <termios.h>
#include "sys/ioctl.h"
#include <unistd.h>
#endif


#define __TIQUE__           0
#define __TIQUE_FACTURA__   1
#define __NDC__             2

using namespace std;

struct Campo
{
    char* phc;
    uint8_t len;
};

struct stgetanswer
{
    uint16_t* cmd;
    uint16_t* exten;
    Campo campos[32];
    uint8_t ncampos;
};

int printf_ByteArray(const unsigned char *data, size_t len);
QString getStringFromUnsignedChar(unsigned char *str, int rev);
void epsonparse( unsigned char pBuff , uint16_t len );

class Command
{
public:
  Command();
  ~Command();
  const char* getFieldOne(){return Field_1;}
  const char* getFieldTwo(){return Field_2;}
  const char* getFieldThre(){return Field_3;}
  const char* getFieldFour(){return Field_4;}
  void setFieldOne(char *ptr){Field_1 = new char[strlen(ptr+1)];
                              strncpy(Field_1,ptr,strlen(ptr));}
private:
  char Field_1[2];
  char Field_2[2];
  char Field_3[2];
  char Field_4[2];

};

class Epson
{

public:
//----------------------------------------------------
//Constructor
    Epson();
   ~Epson();

//-------------------------------------------------------
//Metodos Publicos
public:
    void openPort(int port, int baud, int parity);
    void writePort(char* cmd,char* exten, QStringList qfields);
    char * readPort();
    void closePort();
    int openTique();
    int  closeTique(int tipoTique);
    int getStatus();
    void meansofPayment(int means,int payment,int tipoTique);
    QStringList getBalance(int typeticket);
    int sendPLu(QStringList qstrFields, int typeticket);
    int openTiqueFactura(QStringList clientFields);
    int payTique(double payment, int tipoTique, QString description,int typeticket);
    int sndrecdesc(int type, QString desc, int mount,int typeticket);
    int TiqueCancl(int typeticket);
    int TiqueAnlItm(QStringList qstrlitem,int typeticket);
    int TiqueAnlprv(QStringList qstrlitem,int typeticket);
    int OpenNDC(QStringList clientFields);
    int setDataTime(char* pTime, char* pData );
    int setBauds(int bauds);
    int setHeader(char cline, char* pHeader);
    int setIva(char* pIva);
    int DNFiscal(QStringList qstrl);
    int Reporte_X();
    int Reporte_Z(int iReport);
    int advancepaper();
    unsigned OpenMoneyBox();
    long int _balance;

//-------------------------------------------------------------
//Metodos Privados

private:
    char* _doBcc(char* cCmd);
    char _sequence();
    int _getAnswer();
    void sendChar(unsigned char* cBuf);
    void writePortDirect(unsigned char* buf, int bufsize);
    QStringList getInfoSale();

//------------------------------------------------------------
//Propiedades Privadas
    int __fd;
    char __csqnceChar;
    QStringList __qstrlolditm;
    QStringList __qstrlprvitm;
    unsigned char* __ucBuffer;
    QStringList __qstrlanswr;
    int m_mod;

signals:
    void inter_response();

};




#endif // EPSON_H
