#ifndef BALANCE_H
#define BALANCE_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <QDebug>
#include "util.h"

class Balance
{
public:
    Balance();
    int readBalance(QString &qstr);
    void Close();
private:
    int fd;
    void Open();
};

#endif // BALANCE_H
