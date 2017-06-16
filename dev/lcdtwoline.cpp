#include "lcdtwoline.h"

/**
 * @brief lcdtwoline::lcdtwoline
 */
Lcdtwoline::Lcdtwoline()
{
    __fd = -1;

    /* P0.11 (D) = GPIO */
    (*(volatile unsigned int *)0x4002C02C) &= ~(0x07);      //IOCON port4[15]
    (*(volatile unsigned int *)0x20098000) |= (0x01 << 11); //FIODIR port0
    (*(volatile unsigned int *)0x20098014) |= (0x01 << 11); //FIOPIN

    Open();
}

/**
 * @brief lcdtwoline::Close
 */
void Lcdtwoline::Close()
{
    if (__fd >= 0) {
        close(__fd);
    }
}

/**
 * @brief lcdtwoline::Open
 */
void Lcdtwoline::Open()
{
    char* dev_name =(char*) "/dev/hy_lcd_WH2002L";
    if ((__fd = open(dev_name, O_RDWR)) < 0)
    {
        fprintf(stderr, "%s: unable to open: %s\n", dev_name, strerror(errno));
    }

}

/**
 * @brief lcdtwoline::setLcd
 * @param pmsg
 * Basicamente
 * Borramos y escribimos en el lcd
 */
void Lcdtwoline::setLcd(QString qstrMsg, QString qstrCalcs)
{
    char aMsg[40];
    memset(aMsg,0,40);
    QByteArray qbarray = qstrMsg.toLocal8Bit();
    memcpy(aMsg,qbarray.data(),qbarray.length());
    strncat(aMsg," ",1);
    qbarray = qstrCalcs.toLocal8Bit();
    strncat(aMsg,qbarray.data(),qbarray.length());

    ioctl(__fd, IOCTL_CMD_CLEAR, NULL);/*Borramos*/
    ioctl(__fd, IOCTL_CMD_WRITE, aMsg);/*Escribimos*/
    return;
}
