#include "balance.h"

Balance::Balance()
{
    fd = 0;
    Open();
}

void Balance::Open()
{
    struct termios options;

    qDebug("[DEBUG][DEVICE BALANCE] open ttys2");
    fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1){
        perror("open_port: Unable to open /dev/ttyS2");
        return;
    }

    tcgetattr(fd, &options);
    tcflush(fd, TCIOFLUSH);
    /* Set Baudrate */
    cfsetispeed(&options, B9600);/*Velocidad de entrada*/
    cfsetospeed(&options, B9600);/*Velocidad de salida*/

    switch (1) {
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
    tcflush(fd,TCIOFLUSH);
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

    tcflush(fd, TCIOFLUSH);
    if(tcsetattr(fd, TCSANOW, &options) != 0){
        perror("tcsetattr error #1");
        return;
    }
    qDebug("[DEBUG][DEVICE BALANCE] open done!");

}
void Balance::Close()
{
    ::close(fd);
}

int Balance::readBalance(QString &qstr)
{
    char aRevDat[8] = {0};
    unsigned char cmd [1];
    cmd [0] = 0x57;
    Sleep s;

    /*esperamos que responda la balanza*/
    while(true){
        s.msleep(2);
        if(read(fd,aRevDat, 8) == 8){
            qstr = QString::fromUtf8(aRevDat,8);
            qDebug() << qstr;
            return 0;
        }
    }
}
