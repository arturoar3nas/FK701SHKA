#ifndef MAGNETICSTRIPREADER_H
#define MAGNETICSTRIPREADER_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <QDebug>

#define IOCTL_MSR_MAGIC   0xF1
#define IOCTL_CMD_READ    _IOR(IOCTL_MSR_MAGIC, 0x01, int)

class MagneticStripReader
{
public:
    MagneticStripReader();
    void Close();
    int readMsr(QString &qstr);
private:
    int fd;
    void Open();
};

#endif // MAGNETICSTRIPREADER_H
