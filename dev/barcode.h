#ifndef BARCODE_H
#define BARCODE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <linux/input.h>
#include <unistd.h>
#include <QDebug>
#include "util.h"


/* defined at the end of this file */
char cvt_ev_char(int);
void debug_rcvd_event(struct input_event *);

/* Key state */
#define MAKE					1
#define BREAK					2
#define TYPEMATIC				3

#define INDEX_NO_KEY			0xFF
#define INDEX_GHOST				0xFE
#define INDEX_REPEATING			0xFD

#define IOCTL_KEY_MAGIC         0xF8
#define IOCTL_KEY_GET           _IOR(IOCTL_KEY_MAGIC, 0x00, int) /* Get key index and key status */

#define nullptr NULL
#define MAX_IN_SIZE 30

class BarCode
{
public:
    BarCode();
    void Close();
    void readBarCode(QString &qstrScanner);
private:
    int fd;
    void Open();
signals:
    void strRet(char*);
};

#endif // BARCODE_H
