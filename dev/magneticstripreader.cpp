#include "magneticstripreader.h"

MagneticStripReader::MagneticStripReader()
{
    fd = 0;
    Open();
}

void MagneticStripReader::Open()
{

     const char *dev = "/dev/hy_msr";
     int grab = 1;
     fd = open(dev, O_RDONLY | O_NONBLOCK);
     if (fd == -1) {
         fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
         return;
     }
     int flags = fcntl(fd, F_GETFL, 0);
     fcntl(fd, F_SETFL, flags | O_NONBLOCK);

     if(ioctl(fd,EVIOCGRAB,&grab) != 0)
         qDebug("[DEBUG][MSR] Grab failed!");
     qDebug("[DEBUG][MSR] msrcard /dev/hy_msr");

}

void MagneticStripReader::Close()
{
        ::close(fd);
}

int MagneticStripReader::readMsr(QString &qstr)
{
    char aBuffer[128] = {0};
    if(ioctl(fd,IOCTL_CMD_READ,aBuffer) > 0){
        qDebug("[DEBUG][MSR] buf = %s",aBuffer);
        if(strlen(aBuffer) > 0){
            qstr = QString::fromUtf8(aBuffer,strlen(aBuffer));
            return 0;
        }
    }
    else
        return -1;
}
