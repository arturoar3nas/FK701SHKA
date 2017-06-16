#ifndef LCDTWOLINE_H
#define LCDTWOLINE_H

#include <QObject>
#include <QThread>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define LCD_MAX_CHAR        40 // 20 x 2

#define IOCTL_LCD_MAGIC         0xF0
#define IOCTL_CMD_CLEAR         _IO(IOCTL_LCD_MAGIC,0x00)
#define IOCTL_CMD_WRITE         _IOW(IOCTL_LCD_MAGIC, 0x01, int)
#define IOCTL_CMD_READ_LCD      _IOR(IOCTL_LCD_MAGIC, 0x02, int)
#define IOCTL_CMD_SET_CURSOR    _IOW(IOCTL_LCD_MAGIC, 0x03, int)

#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x01

class Lcdtwoline : public QObject
{
    Q_OBJECT

public:
    Lcdtwoline();

public slots:
    void Close();
    void Open();
    void setLcd(QString qstrMsg, QString qstrCalcs);


private:
    int __fd;
};


#endif // LCDTWOLINE_H
