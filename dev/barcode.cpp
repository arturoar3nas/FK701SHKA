#include "barcode.h"

BarCode::BarCode()
{
    fd = 0;
    Open();
}

void BarCode::Open()
{
     const char *dev = "/dev/input/event0";
     int grab = 1;
     fd = open(dev, O_RDONLY | O_NONBLOCK);
     if (fd == -1) {
         fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
         return;
     }
     int flags = fcntl(fd, F_GETFL, 0);
     fcntl(fd, F_SETFL, flags | O_NONBLOCK);

     if(ioctl(fd,EVIOCGRAB,&grab) != 0)
         qDebug("[DEBUG][DEVICE] Grab failed!");
     qDebug("[DEBUG][DEVICE] Device /dev/input/event0");
}

void BarCode::Close()
{
    ::close(fd);
}

void BarCode::readBarCode(QString &qstrScanner)
{
    int nDebug = 0;
    struct input_event ev[64];
    int nBytesRead = 0;
    int i;
    char aScannerIn[30];
    char cScanChar;
    int wr_ptr = 0;
    Sleep s;
    while (1) {
        int nSize = sizeof(struct input_event) * 64;
        nBytesRead = read(fd, &ev,nSize );
        if(nBytesRead < 0)
            return;
        s.msleep(100);
        if (nBytesRead > 0) {
            qDebug() << "[4]";
            for (i=0; i < (int) (nBytesRead / sizeof(struct input_event)); i++) {
                /* Look for a "key press" event */
                if ((ev[i].type == EV_KEY) && (ev[i].value == 1)) {
                    if (ev[i].code != KEY_LEFTSHIFT) {
                        cScanChar = cvt_ev_char(ev[i].code);         /* Extract the character from the event */
                        if (nDebug) {
                            debug_rcvd_event(&ev[i]);
                        }
                        if (ev[i].code != KEY_ENTER) {
                            aScannerIn[wr_ptr++] = cScanChar;
                        }
                        else {
                            aScannerIn[wr_ptr] = '\0';
                            qDebug("[DEBUG] Input from Scanner: \"%s\"\n", aScannerIn);
                            qstrScanner = QString::fromUtf8(aScannerIn,strlen(aScannerIn));
                            qDebug() << "QString = " << qstrScanner;
                            wr_ptr = 0;
                            return;
                        }
                    } /* if (ev[i].code ...) */
                } /* if ((ev[i].type.....)) */

            } /* for (i=0...) */
        }
    }
    return;
}

char cvt_ev_char(int foo) {

  char bar;

  switch (foo) {
    case KEY_0: bar = '0'; break;
    case KEY_1: bar = '1'; break;
    case KEY_2: bar = '2'; break;
    case KEY_3: bar = '3'; break;
    case KEY_4: bar = '4'; break;
    case KEY_5: bar = '5'; break;
    case KEY_6: bar = '6'; break;
    case KEY_7: bar = '7'; break;
    case KEY_8: bar = '8'; break;
    case KEY_9: bar = '9'; break;

    case KEY_A: bar = 'A'; break;
    case KEY_B: bar = 'B'; break;
    case KEY_C: bar = 'C'; break;
    case KEY_D: bar = 'D'; break;
    case KEY_E: bar = 'E'; break;
    case KEY_F: bar = 'F'; break;
    case KEY_G: bar = 'G'; break;
    case KEY_H: bar = 'H'; break;
    case KEY_I: bar = 'I'; break;
    case KEY_J: bar = 'J'; break;
    case KEY_K: bar = 'K'; break;
    case KEY_L: bar = 'L'; break;
    case KEY_M: bar = 'M'; break;
    case KEY_N: bar = 'N'; break;
    case KEY_O: bar = 'O'; break;
    case KEY_P: bar = 'P'; break;
    case KEY_Q: bar = 'Q'; break;
    case KEY_R: bar = 'R'; break;
    case KEY_S: bar = 'S'; break;
    case KEY_T: bar = 'T'; break;
    case KEY_U: bar = 'U'; break;
    case KEY_V: bar = 'V'; break;
    case KEY_W: bar = 'W'; break;
    case KEY_X: bar = 'X'; break;
    case KEY_Y: bar = 'Y'; break;
    case KEY_Z: bar = 'Z'; break;

    case KEY_ENTER: bar = '\n'; break;

    default: bar = '?';

  }

  return bar;

}

void debug_rcvd_event(struct input_event *ev) {

  char type_str[15];

  switch (ev->type) {
    case EV_SYN:  strcpy(type_str, "EV_SYN"); break;
    case EV_KEY:  strcpy(type_str, "EV_KEY"); break;
    case EV_REL:  strcpy(type_str, "EV_REL"); break;
    case EV_ABS:  strcpy(type_str, "EV_ABS"); break;
    case EV_MSC:  strcpy(type_str, "EV_MSC"); break;
    case EV_SW:   strcpy(type_str, "EV_SW"); break;
    case EV_LED:  strcpy(type_str, "EV_LED"); break;
    case EV_SND:  strcpy(type_str, "EV_SND"); break;
    case EV_REP:  strcpy(type_str, "EV_REP"); break;
    case EV_FF:   strcpy(type_str, "EV_FF"); break;
    case EV_PWR:  strcpy(type_str, "EV_PWR"); break;
    case EV_MAX:  strcpy(type_str, "EV_MAX"); break;

    case EV_FF_STATUS: strcpy(type_str, "EV_FF_STATUS"); break;

    default: strcpy(type_str, "UNK");

  }

}
