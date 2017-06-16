#include "epson.h"

//---------------------------------------------------------------
//Constructor
Epson::Epson()
{
    qDebug("[DEBUG][EPSON] Constructor Epson Done!");
    __csqnceChar = 0x00;
    __fd = -1;
    __qstrlolditm << "0"<< "0"<< "0";
    __qstrlprvitm << "0"<< "0"<< "0";
    __ucBuffer = new unsigned char [256];
    __qstrlanswr << "0"<< "0"<< "0";
    m_mod = 0;
    openPort(1,1,1);
}

//---------------------------------------------------------------
//Destructor
Epson::~Epson()
{

}

//----------------------------------------------------------------
//Metodos
/**
 * @brief Epson::openPort
 * @param port
 * @param baud
 * @param parity
 */
void Epson::openPort(int port, int baud, int parity)
{
#ifdef ARM
    struct termios options;

    Q_UNUSED(port);
    Q_UNUSED(baud);
    Q_UNUSED(parity);

//        __fd = open("/dev/ttyS1", O_RDWR);
        __fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (__fd == -1)
        {
            perror("open_port: Unable to open /dev/ttyS1");
            return;
        }

        tcgetattr(__fd, &options);
        tcflush(__fd, TCIOFLUSH);
        /* Set Baudrate */
        cfsetispeed(&options, B38400);/*Velocidad de entrada*/
        cfsetospeed(&options, B38400);/*Velocidad de salida*/

        switch (parity) {
        case 0:
            /*Even parity (8E1)*/
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;

            break;
        case 1:
            /*Parity none (8N1)*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            options.c_iflag |= (INPCK);
            options.c_cflag &= ~CRTSCTS;/* hw flow control*/
            options.c_cflag |= CLOCAL;/*not change "owner" of port and enable receiver?*/
            options.c_cflag |= CREAD;

            break;
        default:
            break;
        }

        /* Set raw input, 1 second timeout */
        tcflush(__fd,TCIOFLUSH);
//      cfmakeraw(&options);
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//      options.c_lflag &= ~(ICANON | ISIG | IEXTEN);
        options.c_oflag &= ~OPOST;
        options.c_iflag |= IGNBRK;
        options.c_oflag &= ~(ONLCR | OCRNL);
        options.c_iflag &= ~(ICRNL | INLCR);
//      options.c_cflag |= HUPCL;
        options.c_cc[VTIME] = 0;  /*para time out 0.1 sec options.c_cc[VTIME] = 1*/
        options.c_cc[VMIN] = 0;


        /*  Disablenable software flow cpntrol */
        options.c_iflag &= ~(IXON | IXOFF | IXANY);

        tcflush(__fd, TCIOFLUSH);
        if(tcsetattr(__fd, TCSANOW, &options) != 0)
        {
             perror("tcsetattr error #1");
             return;
        }
        qDebug("[DEBUG][EPSON] Open port epson done!");
#endif
}

/**
 * @brief Epson::writePort
 * @param cmd
 * @param exten
 * @param qfields
 */
void Epson::writePort(char* cmd, char* exten, QStringList qfields)
{
    qDebug("[DEBUG][EPSON][WRITEPORT] Write port ");
    char cCMD[256];
    int cLenght=0;

    memset(cCMD,0,256);

    cCMD[cLenght++]      = STX;
    cCMD[cLenght++]      = _sequence();
    cCMD[cLenght++]      = cmd[0];
    cCMD[cLenght++]      = cmd[1];
    if(cmd[2]!= NULL)
        cCMD[cLenght++]  = cmd[2];
    cCMD[cLenght++]      = FLD;
    cCMD[cLenght++]      = exten[0];
    cCMD[cLenght++]      = exten[1];
    cCMD[cLenght++]      = FLD;

    if(qfields.isEmpty())
    {
        qDebug("[DEBUG][EPSON][WRITEPORT] Asigno null al qstringlist");
        cLenght--;
        cCMD[cLenght] = ETX;
        qfields.append("null");
    }
    else
    {
        qDebug("[DEBUG][EPSON][WRITEPORT] For each y asigno separadores");
        cLenght = cLenght -2;
        foreach (const QString &str, qfields)
        {
            qDebug("[DEBUG][EPSON] str = %s",str.toLatin1().constData());
            QByteArray array = str.toLocal8Bit();
            char* ctmp = array.data();
            qDebug("[DEBUG][EPSON] ctmp = %s",ctmp);
            memcpy(cCMD+cLenght+2,ctmp,strlen(ctmp));
            cLenght = cLenght + strlen(ctmp)+1;
            cCMD[cLenght+1]=FLD;
            //sumamos cLenght
        }
        cCMD[cLenght+2] = ETX;
        cLenght = cLenght + 2;
    }
    qDebug("[DEBUG][EPSON][WRITEPORT] calculo bcc");
    qDebug("[DEBUG][EPSON][WRITEPORT] cCMD = %s",cCMD);
    uint16_t qLRC = NULL;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cCMD[i] & 0x00FF);
        i++;
    }while(cCMD[i]!=ETX);
    qLRC += (uint16_t)(cCMD[i] & 0x00FF);//uno mas por el 0x03
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    qDebug("[DEBUG][EPSON][WRITEPORT] Yeah! bcc done!");
    qDebug("[DEBUG][EPSON][WRITEPORT] Asignamos el null al final");
    memcpy(cCMD+cLenght+1,cLRC,cLenght);
    cCMD[cLenght+6]   = '\0';
    qDebug("[DEBUG][EPSON][WRITEPORT] El comando = %s",cCMD);
#ifdef ARM
    unsigned char uBuf [cLenght + 6];
    memset(uBuf, 0, cLenght + 6);
    memcpy(uBuf,cCMD,cLenght+6);
    write(__fd,uBuf,cLenght + 5);
#endif
    return;
}

/**
 * @brief Epson::sendChar
 * @param cBuf
 */
void Epson::sendChar(unsigned char*  cBuf)
{
#ifdef ARM
    const void* cvBuf = &cBuf;
    write(__fd,cvBuf,1);
#endif
}

/**
 * @brief Epson::writePortDirect
 * @param buf
 * @param bufsize
 */
void Epson::writePortDirect(unsigned char* buf, int bufsize)
{
#ifdef ARM
    write(__fd,buf,bufsize);
#endif
}

/**
 * @brief Epson::readPort
 * @return
 * No lo estoy usando
 */
char* Epson::readPort()
{
    return (char* )NAK;
}

/**
 * @brief Epson::closePort
 */
void Epson::closePort()
{
#ifdef ARM
    close(__fd);
#endif
}

/**
 * @brief Epson::_getAnswer
 * @return
 */
int Epson::_getAnswer()
{
    unsigned char rev_dat[2];
    QString strtmp;
    int  i = 0;
    int contDelay=0;
    int bytes = 0;
    Sleep* s = new Sleep();
    QString fromunsignedchartoqstr;
    QRegExp delimiter("(\\{|\\}|\\,|\\\"|\\:|\\;)"); //RegEx for '{' '}' or ',' or '"' or ':' or ;

    __qstrlanswr.clear();
    strtmp.clear();
    memset((char*)__ucBuffer,0,256);
/*
  ioctl(__fd, FIONREAD, &bytes);
  qDebug("[DEBUG][EPSON] bytes = %d",bytes);
*/
#ifdef ARM
    for(int j = 0;j < 4;j++)
    {
        while(contDelay < 500)
        {
            contDelay++;
            s->msleep(1);
            if(read(__fd,rev_dat, 1) > 0)
            {

                strtmp += QChar(rev_dat[0]);
                __ucBuffer[i] = rev_dat[0];
                i++;
                if((rev_dat[0] == 0x15)&&(bytes == 1))
                {
                    qDebug("[DEBUG][EPSON][GETANSWER] 0x15 NACK");
                    goto END;
                }
                if((rev_dat[0] ==0x06)&&(bytes == 1))
                {
                    qDebug("[DEBUG][EPSON][GETANSWER] 0x06 ACK");
                }
                if((rev_dat[0] ==0x02)&&(rev_dat[1] != 0x1b))
                {

                }
                if((rev_dat[0] ==0x03)&&(rev_dat[1] == 0x80))
                {
                    qDebug("[DEBUG][EPSON][GETANSWER] 0x80 Respuesta intermedia");
                    j = 0;
                    memset((char*)__ucBuffer,0,256);
                    strtmp.clear();
                    __qstrlanswr.clear();
                    //s->msleep(500);
                }
                if((rev_dat[0] ==0x03)&&(rev_dat[1] != 0x1b)&&(rev_dat[1] != 0x80))
                {
                    qDebug("[DEBUG][EPSON][GETANSWER] ETX Done!");
                    qDebug("[DEBUG][EPSON][GETANSWER] thecountofchar = %d",i);
                    if(i > 30)
                    {
                        qDebug("[DEBUG][EPSON] i > 30");
                        __qstrlanswr = strtmp.split(QRegExp("\\\x1c"));//C
                        fromunsignedchartoqstr =  getStringFromUnsignedChar(__ucBuffer, i);
                        qDebug("[DEBUG][EPSON] unsigned char to qstring %s",fromunsignedchartoqstr.toLatin1().constData());
                        foreach(const QString &str, __qstrlanswr)
                        {
                            qDebug("[DEBUG][EPSON][GETANSWER]  return from printer = %s",str.toLatin1().constData());
                        }
                        goto END;
                    }
                    if(i > 4 && i <= 30)
                    {
                        qDebug("[DEBUG][EPSON][GETANSWER] i < 30");
                        fromunsignedchartoqstr =  getStringFromUnsignedChar(__ucBuffer, i);
                        qDebug("[DEBUG][EPSON][GETANSWER] unsigned char to qstring %s",fromunsignedchartoqstr.toLatin1().constData());
                        fromunsignedchartoqstr.replace("1c",";");
                        __qstrlanswr = fromunsignedchartoqstr.split(delimiter);
                        goto END;
                    }
                }

                rev_dat[1] = rev_dat[0];
            }
        }
    }
    __qstrlanswr.clear();
END:
    qDebug("[DEBUG][EPSON][GETANSWER] return");
    fromunsignedchartoqstr =  getStringFromUnsignedChar(__ucBuffer, i);
    qDebug("[DEBUG][EPSON][GETANSWER] cBuffer = %s",fromunsignedchartoqstr.toLatin1().constData());
    tcflush(__fd,TCIOFLUSH);
    delete s;
#endif
    return i;
}

/**
 * @brief epsonparse
 * @param pBuff
 * @param len
 * En construccion
 */
void epsonparse( unsigned char pBuff , uint16_t len )
{
    Q_UNUSED(pBuff);
    Q_UNUSED(len);
/*
    struct stgetanswer mystruct;
    uint16_t i;

    mystruct.ncampos = 0;
    mystruct.cmd = &pBuff[3];
    mystruct.exten = &pBuff[6];

    for( i = 8 ; i < len ; i++ )
    {
        if( pBuff[i] == 0x1c )
        {
            mystruct.campos[mystruct.ncampos].phc = &pBuff[i + 1];
            mystruct.campos[mystruct.ncampos].len = 0;
            if(mystruct.ncampos > 0 )
            {
                mystruct.campos[mystruct.ncampos - 1].len = mystruct.campos[mystruct.ncampos].phc - mystruct.campos[mystruct.ncampos - 1].phc - 1;
            }
           ++mystruct.ncampos;
        }
        else if(pBuff[i] == 0x03 && pBuff[i-1] != 0x1b)
        {
            if(pBuff[i-1] != 0x1c)
            {
                mystruct.campos[mystruct.ncampos - 1].len = &pBuff[i] - mystruct.campos[mystruct.ncampos - 1].phc;
            }
            break;
        }
    }
*/
}

/**
 * @brief printf_ByteArray
 * @param data
 * @param len
 * @return
 */
int printf_ByteArray(const unsigned char *data, size_t len) {
  size_t i = 0;
  int y = 0;
  int ch = data[i];
  int result = 0;
  for (i = 0; i < len; i++) {
    static const char escapec[] = "\a\b\t\n\v\f\n\'\"\?\\";
    //char *p = strchr(escapec, ch);
    char *p = (char*)strchr(escapec, ch);
    if (p && ch) {
      static const char escapev[] = "abtnvfn\'\"\?\\";
      qDebug("[DEBUG][EPSON] \\%c", escapev[p - escapec]);
    } else if (isprint(ch)) {
      qDebug("[DEBUG][EPSON] %c", ch);
    } else {
      // If at end of array, assume _next_ potential character is a '0'.
      int nch = i >= (len - 1) ? '0' : data[i + 1];
      if (ch < 8 && (nch < '0' || nch > '7')) {
        qDebug("[DEBUG][EPSON] \\%o", ch);
      } else if (!isxdigit(nch)) {
          qDebug("[DEBUG][EPSON] \\x%X", ch);
      } else {
        qDebug("[DEBUG][EPSON] \\o%03o", ch);
      }
    }
    if (y == EOF)
      return EOF;
    result += y;
  }
  return result;
}

/**
 * @brief getStringFromUnsignedChar
 * @param str
 * @param rev
 * @return
 */
QString getStringFromUnsignedChar(unsigned char *str, int rev)
{

    QString s;
    QString result = "";
//  int rev = strlen(str);

    // Print String in Reverse order....
    for ( int i = 0; i<rev; i++)
        {
           s = QString("%1").arg(str[i],0,16);

           if(s == "0"){
              s="00";
             }
         result.append(s);

         }
   return result;
}

/**
 * @brief Epson::_doBcc
 * @param cCMD
 * @return
 */
char*Epson::_doBcc(char* cCMD)
{
    //qDebug("DEBUG:calculo bcc");
    //qDebug("DEBUG:cCMD = %s",cCMD);
    uint16_t qLRC = NULL;
    char* cLRC = new char[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cCMD[i] & 0x00FF);
        i++;
    }while(cCMD[i]!=ETX);
    qLRC += (uint16_t)(cCMD[i] & 0x00FF);//uno mas por el 0x03
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    return cLRC;
}

/**
 * @brief Epson::getStatus
 * @return
 */
int Epson::getStatus()
{
 #ifdef ARM
    qDebug("[DEBUG][EPSON] getStatus");
    /*contruimos el comando*/
    char cmd [32];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    int cLenght = 0;
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memset(cmd,0x00,32);

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x01;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;


    memcpy(uBuf,cmd,cLenght);
    //qDebug("[DEBUG][EPSON][GETSTATUS] write = %u",(unsigned int)uBuf);

    int iloop = 0;
    here:
    write(__fd,uBuf,cLenght);

    count = _getAnswer();
    qDebug("[DEBUG][EPSON][EPSON][GETSTATUS] validamos qstrlanswr");
    if(__qstrlanswr.isEmpty() == true)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] error de lectura de estatus #1");
        sendChar(ack);
        return -1;
    }
    if(count < 13)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] La respuesta esta incompleta");
        sendChar(ack);
        return -1;
    }
    if(__qstrlanswr.at(0).compare("NULL") == 0)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] NULL");
        sendChar(ack);
        return -1; //Sin documentos
    }
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS]  from answer = %s",str.toLatin1().constData());
    }
    //crear #Define para los errores
    /*
     * posiciones del retorno:
     * estado fiscal: 1 ---> c081 en transaccion | c080 no esta en transaccion
     * retorno del comando: 3 ---> 0000 = Sucess!
     */
    if(__qstrlanswr.at(1).compare("c080") == 0)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] c080");
        sendChar(ack);
        return 1; //Sin documentos
    }
    if(__qstrlanswr.at(1).compare("c081") == 0)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] c081");
        sendChar(ack);
        return 2; //Con documentos
    }
    if(__qstrlanswr.at(1).compare("c082") == 0)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] c082");
        sendChar(ack);
        return 3; //Con documentos
    }
    if(__qstrlanswr.at(1).compare("c083") == 0)
    {
        qDebug("[DEBUG][EPSON][GETSTATUS] c082");
        sendChar(ack);
        return 4; //Con documentos
    }
    iloop++;
    if(iloop == 1)
        goto here;

    qDebug("[DEBUG][EPSON][GETSTATUS] error de lectura de estatus #2");
    sendChar(ack);
#endif
    return -1;
}

/**
 * @brief Epson::getInfoSale
 * @return
 */
QStringList Epson::getInfoSale()
{
    //qDebug("[DEBUG][EPSON]  get info general");
    QStringList ret;

    char * cmd = new char [3];
    cmd[0]=0x0A;
    cmd[1]=0x0A;
    cmd[2]=NULL;
    char * extent = new char [3];
    extent[0]=0x00;
    extent[0]=0x00;
    extent[0]=NULL;
    QStringList newqFields;
    writePort(cmd,extent,newqFields);
    //getAnswer();
    unsigned char* ack = (unsigned char*)0x06;
    sendChar(ack);
    newqFields.append("null");

    return ret;
}

/**
 * @brief Epson::openTique
 * @return
 */
int Epson::openTique()
{
    qDebug("[DEBUG][EPSON]  Open Tique");
    char * cmd = new char [3];
    QStringList qstrlist;
    char * extent = new char [2];
    unsigned char* ack = (unsigned char*)0x06;
    int count =0;
    QString qstrtmp;

    cmd[0]=0x0A;
    cmd[1]=0x01;
    cmd[2]=NULL;
    extent[0]=0x00;
    extent[1]=NULL;

    qDebug("[DEBUG][EPSON]  Write Open Tique");
    writePort(cmd,extent,qstrlist);
    qDebug("[DEBUG][EPSON]  Read Open Tique");
    count = _getAnswer();
    sendChar(ack);
    if(count > 2)
    {
        qstrtmp = __qstrlanswr.at(1);
        if(qstrtmp.compare("c081") == 0)
        {
            qDebug("[DEBUG][EPSON] c081");
            return 0; //Con documentos
        }
    }
    return -1;
}

/**
 * @brief Epson::_sequence
 * @return
 */
char Epson::_sequence()
{

    if(__csqnceChar >= 0xFF)
    {
        __csqnceChar = 0x89;
    }
    else if(__csqnceChar < 0x89)
    {
        __csqnceChar = 0x89;
    }
    __csqnceChar++;
    return __csqnceChar;
}

/**
 * @brief Epson::getBalance
 * @param typeticket
 * @return
 */
QStringList Epson::getBalance(int typeticket)
{
    //qDebug("[DEBUG][EPSON] Get info general");

    char * cmd = new char [3];
    unsigned char* ack = (unsigned char*)0x06;
    char * extent = new char [3];
    QStringList qstrlfields;

    if(typeticket == __TIQUE__)
        cmd[0]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[0]=0x0B;
    if(typeticket == __NDC__)
        cmd[0]=0x0D;
    cmd[1]=0x0A;
    cmd[2]=NULL;

    extent[0]=0x00;
    extent[1]=0x00;
    //extent[0]=NULL;

    writePort(cmd,extent,qstrlfields);

    _getAnswer();

    if(__qstrlanswr.at(3).compare("1b211") == 0)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON] Close Done!");
    }

    sendChar(ack);
    qstrlfields.append("null");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG][EPSON] qstrlanswr %s", str.toLatin1().constData());

    }
    return __qstrlanswr;
}

/**
 * @brief Epson::closeTique
 * @param tipoTique
 * @return
 */
int Epson::closeTique(int tipoTique)
{
    qDebug("[DEBUG][EPSON][CLOSETIQUE] Close Tique");

    /*contruimos el comando*/
    unsigned char* ack = (unsigned char*)0x06;
    char cmd [32];
    int cLenght = 0;

    /*limpiamos el buffer*/
    memset(cmd,0x00,32);

    /*construimos el comando*/
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    if(tipoTique == __TIQUE__)
        cmd[cLenght++]=0x0A;
    if(tipoTique == __TIQUE_FACTURA__)
        cmd[cLenght++]=0x0B;
    if(tipoTique == __NDC__)
        cmd[cLenght++]=0x0D;
    cmd[cLenght++]=0x06;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x1B;
    cmd[cLenght++]=0x03;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x1C;
    if(tipoTique == __NDC__)
    {
        cmd[cLenght++]=0x1C;
    }
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);

    qDebug("[DEBUG][EPSON][CLOSETIQUE] enviamos el cierre de tique");
    write(__fd,uBuf,cLenght);
#endif
        int ret = _getAnswer();
        qDebug("[DEBUG][EPSON][CLOSETIQUE] ret = %d",ret);

        qDebug("[DEBUG][EPSON][CLOSETIQUE] ack");
        sendChar(ack);
        if(__qstrlanswr.isEmpty())
        {
            qDebug("[DEBUG][EPSON][CLOSETIQUE] is empty qstrlanswr");
            return 0;
        }
        foreach(const QString &str, __qstrlanswr)
        {
            qDebug("[DEBUG][EPSON][CLOSETIQUE] qstrlanswr = %s",str.toLatin1().constData());
        }
        if(__qstrlanswr.at(0).compare("0085") == 0)
        {
            qDebug("[DEBUG][EPSON][CLOSETIQUE] Close Done!");
            return 0;
        }
        else
        {
            qDebug("[DEBUG][EPSON][CLOSETIQUE] Epic Fail!");
        }

    return 0;
}

/**
 * @brief Epson::meansofPayment
 * @param means
 * @param payment
 * @param tipoTique
 */
void Epson::meansofPayment(int means, int payment, int tipoTique)
{
    Q_UNUSED(means);
    Q_UNUSED(payment);
    Q_UNUSED(tipoTique);
    //--------------------------------------------------------------------------//
    //02|seq|0A05|0000|1c1c1c|000|1c|Descripcion|1c|1c|medio|1c|pago(12)|03|lrc //
    //--------------------------------------------------------------------------//

}

/**
 * @brief Epson::payTique
 * @param payment
 * @param tipoTique
 * @param description
 * @param typeticket
 * @return
 */
int Epson::payTique(double payment, int tipoTique, QString description,int typeticket)
{

    /*
     * Nota:
     * Inpresora TM U220 solo acepta max 5 pagos
     */
    qDebug("[DEBUG][EPSON][PAYTIQUE] payTique(double payment, int tipoTique, QString description,int typeticket)");
    Q_UNUSED(tipoTique);
    char* firstfield;
    char* secondfield;
    char* mount;
    char cmd [64];
    char cLRC[4];
    unsigned char* ack = (unsigned char*)0x06;
    int cLenght = 0,
        i=0;
    QString strtmp;
    QStringList qstrlfields;
    QByteArray array;
    uint16_t qLRC = 0x00;

    /*limpiamos el buffer*/
    memset(cmd,0x00,64);

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    if(typeticket == __TIQUE__)
        cmd[cLenght++]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[cLenght++]=0x0B;
    if(typeticket == __NDC__)
        cmd[cLenght++]=0x0D;
    cmd[cLenght++]=0x05;
    cmd[cLenght++]=0x1C;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x1C;
    qDebug("[DEBUG][EPSON][PAYTIQUE] description %s",description.toLatin1().constData());

    qstrlfields.append(description);
    qstrlfields.append(description);
    payment = payment * 1;

    strtmp = QString::number(payment,'f',2);//modificar
    if(strtmp.contains(".")==true)
        strtmp.replace(".","");
    qstrlfields.append(strtmp);

    foreach(const QString &str, qstrlfields)
    {
        qDebug("[DEBUG][EPSON][PAYTIQUE] qstrlfields = %s",str.toLatin1().constData());
    }

    array = qstrlfields[0].toLocal8Bit();
    firstfield = array.data();
    memcpy(cmd + cLenght ,firstfield, strlen(firstfield));
    cLenght = cLenght + strlen(firstfield);
    cmd[cLenght++]=0x1C;

    array = qstrlfields[1].toLocal8Bit();
    secondfield = array.data();
    memcpy(cmd + cLenght ,secondfield, strlen(secondfield));
    cLenght = cLenght + strlen(secondfield);
    cmd[cLenght++]=0x1C;

    array = strtmp.toLocal8Bit();
    mount = array.data();
    memcpy(cmd + cLenght ,mount, strlen(mount));
    cLenght = cLenght + strlen(mount);
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    /*crc done!!!*/
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);
    write(__fd,uBuf,cLenght);
#endif
    //writePort(cmd,extent,qstrlfields);
    _getAnswer();
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG][EPSON][PAYTIQUE] qstrlanswr = %s",str.toLatin1().constData());
    }
    if(__qstrlanswr.at(3).compare("0000") == 0)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][PAYTIQUE] qstrtmp = %s",strtmp.toLatin1().constData());
        foreach(const QString &str,qstrlfields)
        {
            qDebug("[DEBUG][EPSON] qstrlfields = %s",str.toLatin1().constData());
        }
        qDebug("[DEBUG][EPSON][PAYTIQUE] cmd = %s",cmd);
        qDebug("[DEBUG][EPSON][PAYTIQUE] mount = %s",mount);
        qDebug("[DEBUG][EPSON][PAYTIQUE] clenght = %d",cLenght);
        qDebug("[DEBUG][EPSON][PAYTIQUE] Payment Done!");
        return 0;
    }
    if(__qstrlanswr.at(3).compare("1b25") == 0)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][PAYTIQUE] Campo no opcional!");
        return -2;
    }
    if(__qstrlanswr.at(3).compare("96") == 0)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][PAYTIQUE] Demasiados Pagos involucrados!");
        return -3;
    }
    else
    {
        qDebug("[DEBUG][EPSON][PAYTIQUE] Epic Fail!");
    }
    return -1;
}

/**
 * @brief Epson::sendPLu
 * @param qfields
 * @param typeticket
 * @return
 */
int Epson::sendPLu(QStringList qstrFields,int iticket)
{
    qDebug("[DEBUG][EPSON][SENDPLU] Send Plu!");
    char aCmd [4];
    char aExten [3];
    QStringList qstrlfields;
    QString qstrtmp;
    QString qstrtmp_2;
    unsigned char* ack = (unsigned char*)0x06;

    if(iticket == __TIQUE__)
        aCmd[0]=0x0A;
    if(iticket == __TIQUE_FACTURA__)
        aCmd[0]=0x0B;
    if(iticket == __NDC__)
        aCmd[0]=0x0D;
    aCmd[1]=0x1B;
    aCmd[2]=0x02;
    aCmd[3]=0x00;
    if(qstrFields.at(13).compare("Yes") == 0){
        aExten[0]=0x00;
        aExten[1]=0x06;
        aExten[2]=NULL;
    }
    else if(qstrFields.at(11).toInt() == 4){
        aExten[0]=0x00;
        aExten[1]=0x00; //<<<<<<<< deberia ser 0x02
        aExten[2]=NULL;
    }
    else
    {
        aExten[0]=0x00;
        aExten[1]=0x00;
        aExten[2]=NULL;
    }

    qstrlfields.append("");
    qstrlfields.append("");
    qstrlfields.append("");
    qstrlfields.append("");
    qstrlfields.append(qstrFields.at(0));

    /* Para el precio:
     * Si tiene punto entonces se le quita
     * si no tiene punto se complementa con 2 cero al final
     */
    qDebug("[DEBUG][EPSON][SENDPLU] qfields.at(6) = %s",qstrFields.at(6).toLatin1().constData());
    qstrtmp = qstrFields.at(6);
    if(qstrtmp.contains(".")==true)
    {
        qstrtmp.replace(".","");
        qstrtmp.append("00");
    }
    else
        qstrtmp.append("00");
    qstrlfields.append(qstrtmp);
    qDebug("[DEBUG][EPSON][SENDPLU] qfields.at(5) = %s",qstrFields.at(5).toLatin1().constData());
    qstrtmp_2 = qstrFields.at(5);
    if(qstrtmp_2.contains(".")==true)
    {
        qstrtmp_2.replace(".","");
        qstrtmp_2.append("000");
    }
    else
        qstrtmp_2.append("0000");

    qstrlfields.append(qstrtmp_2);
    qstrlfields.append(qstrFields.at(7));
    qstrlfields.append("");//8

    /*Escribimos en el puerto*/
    qDebug("[DEBUG][EPSON][SENDPLU] Write Send Plu!");
    writePort(aCmd,aExten,qstrlfields);

    /*leemos en el puerto*/
    qDebug("[DEBUG][EPSON][SENDPLU] Read Send Plu!");
    int count = _getAnswer();

    qDebug("[DEBUG][EPSON][SENDPLU] count the plu characters = %d",count);
    if(count < 15)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][SENDPLU] Error de lectura respuesta incompleta");
        return -1;
    }

    /*
     * posiciones del retorno:
     * estado fiscal: 1 ---> c081 en transaccion | c080 no esta en transaccion
     * retorno del comando: 3 ---> 0000 = Sucess!
     */
    qDebug("[DEBUG][EPSON][SENDPLU] posicion 3  = %s",__qstrlanswr.at(3).toLatin1().constData());
    if(__qstrlanswr.at(3).compare("0000") == 0)
    {
        sendChar(ack);
        return 0;
    }
   return -1;
}

/**
 * @brief Epson::openTiqueFactura
 * @param clientFields
 * @return
 * Si existe cliente es un tique factura
 */
int Epson::openTiqueFactura(QStringList clientFields)
{
    qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] TiqueFactura");

    /*contruimos el comando*/
    char cmd [128];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    QStringList qstrltmp;
    uint16_t qLRC = 0x00;
    int cLenght = 0;

    memset(cmd,0x00,128);

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x0B;
    cmd[cLenght++]=0x01;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;

    qstrltmp.append(clientFields.at(3));//nombre #1
    qstrltmp.append("nombre #2");//nombre #2
    qstrltmp.append(clientFields.at(4));//domiclio #1
    qstrltmp.append("domiclio #2");//domiclio #2
    qstrltmp.append("domiclio #3");//domiclio #3
    qstrltmp.append(clientFields.at(0));//tipo del documento del comprador
    qstrltmp.append(clientFields.at(1));//numero del documento del comprador
    qstrltmp.append(clientFields.at(2));//responsabilidad ante el iva
    qstrltmp.append("remitos #1");//linea de remitos #1
    qstrltmp.append("remitos #2");//linea de remitos #2

    foreach (const QString &str, qstrltmp)
    {
        qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] str = %s",str.toLatin1().constData());
        QByteArray array = str.toLocal8Bit();
        char* ctmp = array.data();
        qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] ctmp = %s",ctmp);
        memcpy(cmd+cLenght,ctmp,strlen(ctmp));
        cLenght = cLenght + strlen(ctmp);
        cmd[cLenght++]=FLD;
    }
    cmd[cLenght++] = ETX;
    qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] Do CRC!");

    /*calculamos el crc*/
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);

    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);

    //qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] Write Port = %u",(unsigned int)uBuf);
    write(__fd,uBuf,cLenght);
#endif
    count = _getAnswer();
    qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] count the plu characters = %d",count);
    if(count < 15)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] Error de lectura respuesta incompleta");
        return -1;
    }
    /*
     * posiciones del retorno:
     * estado fiscal: 1 ---> c081 en transaccion | c080 no esta en transaccion
     * retorno del comando: 3 ---> 0000 = Sucess!
     */
    qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] posicion 3  = %s",__qstrlanswr.at(3).toLatin1().constData());
    if(__qstrlanswr.at(3).compare("0000") == 0)
    {
        qDebug("[DEBUG][FACADE][OPENTICKETFACTURA] Enviamos el ACK");
        sendChar(ack);
        qDebug("[DEBUG][FACADE][OPENTICKETFACTURA] return SUCESS");
        return 0;
    }
    if(__qstrlanswr.at(3).compare("1b25") == 0)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON][OPENTICKETFACTURA] Error Campo no Opcional");
        return -2;
    }

    return -4;
}

/**
 * @brief Epson::OpenNDC
 * @param clientFields
 * @return
 */
int Epson::OpenNDC(QStringList clientFields)
{

    qDebug("[DEBUG][EPSON] Nota de Credito");
    qDebug() << clientFields;
    /*contruimos el comando*/
    char cmd [128] = {0};
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    QStringList qstrltmp;

    int cLenght = 0;
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x0D;
    cmd[cLenght++]=0x01;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;

    qstrltmp.append(clientFields.at(3));//nombre #1
    qstrltmp.append("nombre #2");//nombre #2
    qstrltmp.append(clientFields.at(4));//domiclio #1
    qstrltmp.append("domiclio #2");//domiclio #2
    qstrltmp.append("domiclio #3");//domiclio #3
    qstrltmp.append(clientFields.at(0));//tipo del documento del comprador
    qstrltmp.append(clientFields.at(1));//numero del documento del comprador
    qstrltmp.append(clientFields.at(2));//responsabilidad ante el iva
    qstrltmp.append("remitos #1");//linea de remitos #1
    qstrltmp.append("remitos #2");//linea de remitos #2
    qstrltmp.append("Linea de Comprobante");

    foreach (const QString &str, qstrltmp)
    {
        qDebug("[DEBUG][EPSON] str = %s",str.toLatin1().constData());
        QByteArray array = str.toLocal8Bit();
        char* ctmp = array.data();
        qDebug("[DEBUG][EPSON] ctmp = %s",ctmp);
        memcpy(cmd+cLenght,ctmp,strlen(ctmp));
        cLenght = cLenght + strlen(ctmp);
        cmd[cLenght++]=FLD;
    }
    cmd[cLenght-1] = 0x03;
    //cLenght = cLenght + 1;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memcpy(uBuf,cmd,cLenght);
    write(__fd,uBuf,cLenght);
    count = _getAnswer();
#endif
    qDebug("[DEBUG][EPSON] count the plu characters = %d",count);
    if(count < 15)
    {
        sendChar(ack);
        qDebug("[DEBUG][EPSON] Error de lectura respuesta incompleta");
        return -1;
    }
    /*
     * posiciones del retorno:
     * estado fiscal: 1 ---> c081 en transaccion | c080 no esta en transaccion
     * retorno del comando: 3 ---> 0000 = Sucess!
     */
    qDebug("[DEBUG][EPSON] posicion 3  = %s",__qstrlanswr.at(3).toLatin1().constData());
    if(__qstrlanswr.at(3).compare("0000") == 0)
    {
        sendChar(ack);
        return 0;
    }

    return -2;
}

/**
 * @brief Epson::sndrecdesc
 * @param int type, QString desc, int mount,int typeticket
 * @return int
 * Descuentos y Recargos GLOBALES
 */
int Epson::sndrecdesc(int type, QString desc, int mount,int typeticket)
{
    qDebug("[DEBUG][EPSON] Recargo y Descuentos!");

    /*contruimos el comando*/
    char cmd [32];
    memset(cmd,0x00,strlen(cmd));

    int cLenght = 0;
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    if(typeticket == __TIQUE__)
        cmd[cLenght++]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[cLenght++]=0x0B;
    if(typeticket == __NDC__)
        cmd[cLenght++]=0x0D;
    cmd[cLenght++]=0x04;
    cmd[cLenght++]=FLD;
    QString qstrmount = QString::number(mount);
    /*Descuento*/
    if(type == 0)
    {
        cmd[cLenght++]=0x00;
        cmd[cLenght++]=0x00;
    }
    /*Recargo*/
    if(type == 1)
    {
        cmd[cLenght++]=0x00;
        cmd[cLenght++]=0x01;
    }
    cmd[cLenght++]=FLD;
    QByteArray array = desc.toLocal8Bit();
    char* ctmp = array.data();
    memcpy(cmd+cLenght,ctmp,strlen(ctmp));
    cLenght = cLenght + strlen(ctmp);
    cmd[cLenght++]=FLD;
    QByteArray qbyarray = qstrmount.toLocal8Bit();
    char* cctmp = qbyarray.data();
    memcpy(cmd+cLenght,cctmp,strlen(cctmp));
    cLenght = cLenght + strlen(cctmp);
    cmd[cLenght++] = ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght++]   = 0x00;
    //writePortDirect(cmd,cLenght);
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memcpy(uBuf,cmd,cLenght);
    write(__fd,uBuf,cLenght);
#endif
    int ianswr = 0;
    ianswr = _getAnswer();
    if(ianswr <= 0)return -1;
    else
    {
        unsigned char* ack = (unsigned char*)0x06;
        sendChar(ack);
        return 0;
    }

    return -1;
}

/**
 * @brief Epson::TiqueCancl
 * @param typeticket
 * @return
 */
int Epson::TiqueCancl(int typeticket)
{
    char * cmd = new char [3];
    char * extent = new char [3];
    unsigned char* ack = (unsigned char*)0x06;
    QStringList qstrlfields;
    if(typeticket == __TIQUE__)
        cmd[0]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[0]=0x0B;
    if(typeticket == __NDC__)
        cmd[0]=0x0D;
    cmd[1]=0x07;
    cmd[2]=NULL;
    extent[0]=0x00;
    extent[1]=0x00;
    extent[2]=NULL;
    writePort(cmd,extent,qstrlfields);
    _getAnswer();
    if(__qstrlanswr.at(0).compare("NULL") == 0)
        return -1;
    sendChar(ack);

    return 0;

}

/**
 * @brief Epson::TiqueAnlItm
 * @param qstrlitem
 * @param typeticket
 * @return
 */
int Epson::TiqueAnlItm(QStringList qstrlitem,int typeticket)
{
    //qDebug("[DEBUG][EPSON] Send Plu!");

    //if(this->qstrlolditm.isEmpty()==true)
    //{
        //qDebug("[WARNING] ultimo item vacio! ");
    //    return -1;
    //}

    char* cmd = new char[4];
    char* exten= new char [3];
    QStringList qstrlfield;
    QString qstrtmp;
    QString qstrtmp_2;

    if(typeticket == __TIQUE__)
        cmd[0]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[0]=0x0B;
    if(typeticket == __NDC__)
        cmd[0]=0x0D;
    cmd[1]=0x1B;
    cmd[2]=0x02;
    cmd[3]=0x00;

    exten[0]=0x00;
    exten[1]=0x11;
    exten[2]=NULL;

    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append(qstrlitem.at(0));
    qstrtmp = qstrlitem.at(6);
    if(qstrtmp.contains(".")==true)
        qstrtmp.replace(".","");
    else
        qstrtmp.append("00");
    qstrlfield.append(qstrtmp);
    qstrtmp_2 = qstrlitem.at(5);
    if(qstrtmp_2.contains(".")==true)
    {
        qstrtmp_2.replace(".","");
        qstrtmp_2.append("0000");
    }
    else
        qstrtmp_2.append("000000");
    qstrlfield.append(qstrtmp_2);
    qstrlfield.append(qstrlitem.at(7));
    qstrlfield.append("");
    writePort(cmd,exten,qstrlfield);

    _getAnswer();
    if(__qstrlanswr.at(0).compare("NULL") == 0)
        return -1;

    return 0;

}

/**
 * @brief Epson::TiqueAnlprv
 * @param qstrlitem
 * @param typeticket
 * @return
 */
int Epson::TiqueAnlprv(QStringList qstrlitem,int typeticket)
{
    //qDebug("[DEBUG][EPSON] Anulprv!");

    char* cmd = new char[4];
    QString qstrtmp;
    QString qstrtmp_2;
    QStringList qstrlfield;

    if(typeticket == __TIQUE__)
        cmd[0]=0x0A;
    if(typeticket == __TIQUE_FACTURA__)
        cmd[0]=0x0B;
    if(typeticket == __NDC__)
        cmd[0]=0x0D;
    cmd[1]=0x1B;
    cmd[2]=0x02;
    cmd[3]=0x00;
    //strcpy(cmd,"0A1B");
    char* exten= new char [3];
    exten[0]=0x00;
    exten[1]=0x11;
    exten[2]=NULL;
    //strcpy(exten,"0000");
    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append("");
    qstrlfield.append(qstrlitem.at(0));
    qstrtmp = qstrlitem.at(6);
    if(qstrtmp.contains(".")==true)
        qstrtmp.replace(".","");
    else
        qstrtmp.append("00");
    qstrlfield.append(qstrtmp);
    qstrtmp_2 = qstrlitem.at(5);
    if(qstrtmp_2.contains(".")==true)
    {
        qstrtmp_2.replace(".","");
        qstrtmp_2.append("0000");
    }
    else
        qstrtmp_2.append("000000");
    qstrlfield.append(qstrtmp_2);
    qstrlfield.append(qstrlitem.at(7));
    qstrlfield.append("");

    writePort(cmd,exten,qstrlfield);

    //qstrlanswr = _getAnswer();
    if(__qstrlanswr.at(0).compare("NULL") == 0)
        return -1;

    return 0;
}

/**
 * @brief Epson::setDataTime
 * @param pTime
 * @param pData
 * @return
 */
int Epson::setDataTime(char* pTime, char* pData )
{
    qDebug("[DEBUG][EPSON] setDataTime");
    /*Nota:
     *El formato de la hora debe ser militar*/
    /*contruimos el comando*/
    char * cmd = new char [32];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    int cLenght = 0;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memset(cmd,0x00,strlen(cmd));

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x05;
    cmd[cLenght++]=0x01;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;

    memcpy(cmd+cLenght,pData,strlen(pData));
    cLenght = cLenght + strlen(pData);
    cmd[cLenght++]=FLD;

    memcpy(cmd+cLenght,pTime,strlen(pTime));//hhmmss
    cLenght = cLenght + strlen(pTime);
    cmd[cLenght++] = ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;


    memcpy(uBuf,cmd,cLenght);
    //qDebug("[DEBUG][EPSON] write = %u",(unsigned int)uBuf);
    write(__fd,uBuf,cLenght);

    count = _getAnswer();
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    if(__qstrlanswr.isEmpty() == true)
    {
        qDebug("[DEBUG] error de lectura de estatus #1");
        sendChar(ack);
        return -1;
    }
    if(count < 13)
    {
        qDebug("[DEBUG] La respuesta esta incompleta");
        sendChar(ack);
        return -1;
    }
    if(__qstrlanswr.at(0).compare("NULL") == 0)
    {
        qDebug("[DEBUG] NULL");
        sendChar(ack);
        return -1; //Sin documentos
    }
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG]  from answer = %s",str.toLatin1().constData());
    }

    qDebug("[DEBUG][EPSON]Done!");
    sendChar(ack);
#endif
    return 0;
}

/**
 * @brief Epson::setBauds
 * @param bauds
 * @return
 */
int Epson::setBauds(int bauds)
{
    qDebug("[DEBUG][EPSON] setBauds");
    /*contruimos el comando*/
    char * cmd = new char [32];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    int cLenght = 0;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memset(cmd,0x00,strlen(cmd));

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x0A;
    cmd[cLenght++]=FLD;
    if(bauds == 38400)
    {
        cmd[cLenght++]=0x00;
        cmd[cLenght++]=0x00;
    }
    else if(bauds == 19200)
    {
        cmd[cLenght++]=0x00;
        cmd[cLenght++]=0x01;
    }
    else if(bauds == 9600)
    {
        cmd[cLenght++]=0x00;
        cmd[cLenght++]=0x01;
    }
    else
    {
        qDebug("[DEBUG][EPSON] Velocidad no permitida");
        return -1;
    }
    cmd[cLenght++] = ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;


    memcpy(uBuf,cmd,cLenght);
    //qDebug("[DEBUG][EPSON] write = %u",(unsigned int)uBuf);
    write(__fd,uBuf,cLenght);

    count = _getAnswer();
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    if(__qstrlanswr.isEmpty() == true)
    {
        qDebug("[DEBUG] error de lectura de estatus #1");
        sendChar(ack);
        return -1;
    }
    if(count < 13)
    {
        qDebug("[DEBUG] La respuesta esta incompleta");
        sendChar(ack);
        return -1;
    }
    if(__qstrlanswr.at(0).compare("NULL") == 0)
    {
        qDebug("[DEBUG] NULL");
        sendChar(ack);
        return -1; //Sin documentos
    }
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG]  from answer = %s",str.toLatin1().constData());
    }

    qDebug("[DEBUG][EPSON]Done!");
    sendChar(ack);
#endif
    return 0;
}

/**
 * @brief Epson::setHeader
 * @param cline
 * @param pHeader
 * @return
 */
int Epson::setHeader(char cline, char* pHeader)
{
    qDebug("[DEBUG][EPSON] setHeader");
    /*contruimos el comando*/
    char cmd  [64];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    int cLenght = 0;

    memset(cmd,0x00,64);

    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x05;
    cmd[cLenght++]=0x08;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=cline;
    cmd[cLenght++]=FLD;
    memcpy(cmd+cLenght,pHeader,strlen(pHeader));
    cLenght = cLenght + (strlen(pHeader));
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);

    //qDebug("[DEBUG][EPSON] write = %u",(unsigned int)uBuf);
    write(__fd,uBuf,cLenght);
#endif
    count = _getAnswer();
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    if(__qstrlanswr.isEmpty() == true)
    {
        qDebug("[DEBUG] error de lectura de estatus #1");
        sendChar(ack);
        return -1;
    }
    if(count < 13)
    {
        qDebug("[DEBUG] La respuesta esta incompleta");
        sendChar(ack);
        return -1;
    }
    if(__qstrlanswr.at(0).compare("NULL") == 0)
    {
        qDebug("[DEBUG] NULL");
        sendChar(ack);
        return -1; //Sin documentos
    }
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG]  from answer = %s",str.toLatin1().constData());
    }

    qDebug("[DEBUG][EPSON]Done!");
    sendChar(ack);
    return 0;

}

/**
 * @brief Epson::setIva
 * @param pIva
 * @return
 */
int Epson::setIva(char* pIva)
{
    qDebug("[DEBUG][EPSON] setHeader");
    /*contruimos el comando*/
    char * cmd = new char [32];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;
    int cLenght = 0;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, sizeof(uBuf));
    memset(cmd,0x00,strlen(cmd));
#endif
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x05;
    cmd[cLenght++]=0x05;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;
    memcpy(cmd+cLenght,pIva,strlen(pIva));//nnnn
    cLenght = cLenght + strlen(pIva);
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);
    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);
    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;

#ifdef ARM
    memcpy(uBuf,cmd,cLenght);
    //qDebug("[DEBUG][EPSON] write = %u",(unsigned int)uBuf);
    write(__fd,uBuf,cLenght);
#endif
    count = _getAnswer();
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    if(__qstrlanswr.isEmpty() == true)
    {
        qDebug("[DEBUG] error de lectura de estatus #1");
        sendChar(ack);
        return -1;
    }
    if(count < 13)
    {
        qDebug("[DEBUG] La respuesta esta incompleta");
        sendChar(ack);
        return -1;
    }
    if(__qstrlanswr.at(0).compare("NULL") == 0)
    {
        qDebug("[DEBUG] NULL");
        sendChar(ack);
        return -1; //Sin documentos
    }
    qDebug("[DEBUG][EPSON] validamos qstrlanswr");
    foreach(const QString &str, __qstrlanswr)
    {
        qDebug("[DEBUG]  from answer = %s",str.toLatin1().constData());
    }

    qDebug("[DEBUG][EPSON]Done!");
    sendChar(ack);
    return 0;

}

/**
 * @brief Epson::DNFiscal
 * @param qstrlines
 * @return
 */
int Epson::DNFiscal(QStringList qstrl)
{
    char acCmd [4096];
    char acLRC[4];
    unsigned char* byAck = (unsigned char*)0x06;
    int ncount  = 0,
        nlentmp = 0,
        ni      = 0,
        nret    = 0;
    int nLenght = 0;
    int nlines = 34;//28 + 6
    uint16_t qwLRC = 0x00;
    char* ptmp =(char*) malloc(1);
    QByteArray qbya;

    if(qstrl.isEmpty()){
        qDebug("[ERROR][EPSON][DNFiscal] qstrlines empty! <<<<<<<<<<<<<<<<<<<");
        free(ptmp);
        return -1;
    }

    memset(acCmd,0x00,4096);
    /*contruimos el comando*/
    acCmd[nLenght++] = STX;
    acCmd[nLenght++] = _sequence();
    acCmd[nLenght++] = 0x0E;
    acCmd[nLenght++] = 0x30;
    acCmd[nLenght++] = FLD;
    acCmd[nLenght++] = 0x00;
    acCmd[nLenght++] = 0x00;//04
    acCmd[nLenght++] = FLD;

    int nn = 0;
    foreach (const QString &str, qstrl){/*iteramos la lista y agregamos los campos al bufer*/
        qDebug() << "[DEBUG][EPSON][DNFiscal] str = " << str;
        qbya = str.toLocal8Bit();
        nlentmp = qbya.length();
        ptmp = (char*)realloc(ptmp,nlentmp);
        memset(ptmp,0,nlentmp);
        memcpy(ptmp,qbya.data(),nlentmp);
        memcpy(acCmd+nLenght,ptmp,nlentmp);
        nLenght = nLenght + nlentmp;
        acCmd[nLenght++]=FLD;
        qbya.clear();
        if(nn > nlines - 6)//28
            break;
        nn++;
    }
    nlines = nlines - nn;

    for(int i = 0 ; i < nlines ; i++ ){/*dejamos vacios los campos faltantes*///34 estacionamiento//son 37 1c //6 reportes
        acCmd[nLenght++]=FLD;
    }

    acCmd[nLenght-1] = 0x03;

    /*calculamos el crc*/
    do{
        qwLRC += (uint16_t)(acCmd[ni] & 0x00FF);
        ni++;
    }while(ni < nLenght);

    sprintf(acLRC,"%04x",qwLRC);
    acLRC[0] = toupper(acLRC[0]);
    acLRC[1] = toupper(acLRC[1]);
    acLRC[2] = toupper(acLRC[2]);
    acLRC[3] = toupper(acLRC[3]);

    memcpy(acCmd+nLenght,acLRC,4);
    nLenght = nLenght + 4;
    acCmd[nLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [nLenght];
    memset(uBuf, 0, nLenght);
    memcpy(uBuf,acCmd,nLenght);
    qDebug() << "\n uBuf ==>\n"     << uBuf
             << "\n nlenght ==>\n"  << nLenght
             << "\n acCmd ==>\n"    << acCmd;
    /*Write Port*/
    nret = write(__fd,uBuf,nLenght);
    if (nret <= 0) {
         if (errno != EINTR && errno != EAGAIN) {
            return -1;
         }
    }
#endif
    /*Read Port*/
    ncount = _getAnswer();
    qDebug("[DEBUG][EPSON][DNFiscal] count the plu characters = %d",ncount);
    if(ncount < 15){
        sendChar(byAck);
        qDebug("[DEBUG][EPSON][DNFiscal] Error de lectura respuesta incompleta <<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        free(ptmp);
        return -1;
    }

    /*SUCESS*/
    sendChar(byAck);
    free(ptmp);
    return 0;
}

/**
 * @brief Epson::Reporte_X
 * @return
 */
int Epson::Reporte_X()
{
    char acCmd [256];
    char acLRC[4];
    unsigned char* byAck = (unsigned char*)0x06;
    int ncount  = 0;
    int nLenght = 0;
    int ni      = 0;
    int nret    = 0;
    uint16_t qwLRC = 0x00;

    memset(acCmd,0x00,strlen(acCmd));

    /*contruimos el comando*/
    acCmd[nLenght++]=STX;
    acCmd[nLenght++]=_sequence();
    acCmd[nLenght++]=0x08;
    acCmd[nLenght++]=0x1B;
    acCmd[nLenght++]=0x02;
    acCmd[nLenght++]=FLD;
    acCmd[nLenght++]=0x00;
    acCmd[nLenght++]=0x01;
    acCmd[nLenght++]=ETX;
    /*calculamos el crc*/
    do{
        qwLRC += (uint16_t)(acCmd[ni] & 0x00FF);
        ni++;
    }while(ni < nLenght);

    sprintf(acLRC,"%04x",qwLRC);
    acLRC[0] = toupper(acLRC[0]);
    acLRC[1] = toupper(acLRC[1]);
    acLRC[2] = toupper(acLRC[2]);
    acLRC[3] = toupper(acLRC[3]);

    memcpy(acCmd+nLenght,acLRC,4);
    nLenght = nLenght + 4;
    acCmd[nLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [nLenght];
    memset(uBuf, 0, nLenght);
    memcpy(uBuf,acCmd,nLenght);
    nret = write(__fd,uBuf,nLenght);
    if (nret <= 0) {
         if (errno != EINTR && errno != EAGAIN) {
            qDebug() << "\n Error Unistd write port <<<<<<<<<<<<<<<<\n" << uBuf;
            return -1;
         }
    }
#endif
    ncount = _getAnswer();
    sendChar(byAck);
    return 0;
}

/**
 * @brief Epson::Reporte_Z
 * @param iReport
 * @return
 */
int Epson::Reporte_Z(int iReport)
{

    qDebug("[DEBUG][EPSON][ReporteZ] Reporte Z reporte =  %d", iReport);


    char acCmd [256];
    char acLRC[4];
    unsigned char* byAck = (unsigned char*)0x06;
    int ncount = 0;
    int nLenght = 0;
    int ni=0;
    uint16_t qwLRC = 0x00;

    memset(acCmd,0x00,strlen(acCmd));

    /*contruimos el comando*/
    acCmd[nLenght++]=STX;
    acCmd[nLenght++]=_sequence();
    acCmd[nLenght++]=0x08;

    switch(iReport){
    case 0:{
        /*Cierre Z*/
        acCmd[nLenght++]=0x01;
        acCmd[nLenght++]=FLD;
        acCmd[nLenght++]=0x00;
        acCmd[nLenght++]=0x00;
        acCmd[nLenght++]=ETX;
        break;
    }
    case 1:
        /*Reporte Z1*/
        acCmd[nLenght++]=0x1B;
        acCmd[nLenght++]=0x03;
        acCmd[nLenght++]=FLD;
        acCmd[nLenght++]=0x04;
        acCmd[nLenght++]=0x00;
        acCmd[nLenght++]=ETX;
        break;
    case 2:
        /*Reporte Z2*/
        acCmd[nLenght++]=0x21;
        acCmd[nLenght++]=FLD;
        acCmd[nLenght++]=0x00;
        acCmd[nLenght++]=0x01;
        acCmd[nLenght++]=FLD;
        acCmd[nLenght++]=0x30;
        acCmd[nLenght++]=0x30;
        acCmd[nLenght++]=0x30;
        acCmd[nLenght++]=0x30;
        acCmd[nLenght++]=FLD;
        acCmd[nLenght++]=0x39;
        acCmd[nLenght++]=0x39;
        acCmd[nLenght++]=0x39;
        acCmd[nLenght++]=0x39;
        acCmd[nLenght++]=ETX;
        break;
    case 3:/*Reporte de Auditoria*/
        break;
    case 4:
        break;
    default:
        break;
    }

    /*calculamos el crc*/
    do
    {
        qwLRC += (uint16_t)(acCmd[ni] & 0x00FF);
        ni++;
    }while(ni < nLenght);

    sprintf(acLRC,"%04x",qwLRC);
    acLRC[0] = toupper(acLRC[0]);
    acLRC[1] = toupper(acLRC[1]);
    acLRC[2] = toupper(acLRC[2]);
    acLRC[3] = toupper(acLRC[3]);

    memcpy(acCmd+nLenght,acLRC,4);
    nLenght = nLenght + 4;
    acCmd[nLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [nLenght];
    memset(uBuf, 0, nLenght);
    memcpy(uBuf,acCmd,nLenght);
    write(__fd,uBuf,nLenght);
    ncount = _getAnswer();
#endif
    sendChar(byAck);

    return 0;
}

/**
 * @brief Epson::advancepaper
 * @return
 */
int Epson::advancepaper()
{
    qDebug("[DEBUG][EPSON][advancepaper] advancepaper");

    /*contruimos el comando*/
    char cmd [256];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;

    memset(cmd,0x00,strlen(cmd));
    int cLenght = 0;
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x07;
    cmd[cLenght++]=0x01;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x31;
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);

    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);

    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);
    write(__fd,uBuf,cLenght);
    count = _getAnswer();
#endif
    sendChar(ack);

    return 0;
}

/**
 * @brief Epson::OpenMoneyBox
 */
unsigned Epson::OpenMoneyBox()
{
    qDebug("[DEBUG][EPSON][advancepaper] OpenMoneyBox");

    /*contruimos el comando*/
    char cmd [256];
    unsigned char* ack = (unsigned char*)0x06;
    int count = 0;

    memset(cmd,0x00,strlen(cmd));
    int cLenght = 0;
    cmd[cLenght++]=STX;
    cmd[cLenght++]=_sequence();
    cmd[cLenght++]=0x07;
    cmd[cLenght++]=0x07;
    cmd[cLenght++]=FLD;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=0x00;
    cmd[cLenght++]=ETX;

    /*calculamos el crc*/
    uint16_t qLRC = 0x00;
    char cLRC[4];
    int i=0;
    do
    {
        qLRC += (uint16_t)(cmd[i] & 0x00FF);
        i++;
    }while(i < cLenght);

    sprintf(cLRC,"%04x",qLRC);
    cLRC[0] = toupper(cLRC[0]);
    cLRC[1] = toupper(cLRC[1]);
    cLRC[2] = toupper(cLRC[2]);
    cLRC[3] = toupper(cLRC[3]);

    memcpy(cmd+cLenght,cLRC,4);
    cLenght = cLenght + 4;
    cmd[cLenght+1]   = 0x00;
#ifdef ARM
    unsigned char uBuf [cLenght];
    memset(uBuf, 0, cLenght);
    memcpy(uBuf,cmd,cLenght);
    write(__fd,uBuf,cLenght);
    count = _getAnswer();
#endif
    sendChar(ack);

    return 0;
}

int Epson::sendCommand()
{




    return 0;
}
