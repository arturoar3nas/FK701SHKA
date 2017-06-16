#include "keyboard.h"

#define nullptr NULL

KeyBoard::KeyBoard(QObject * parent) : QObject(parent)
{
    stopThread = false;
    fd = -1;
    Qwerty = true;
    Calc = false;
    Shift = false;
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(MsgLoop_KeyBoard()));
    connect(&thread,SIGNAL(finished()),this,SLOT(deleteLater()));
    thread.start();
}

/**
 * @brief KeyBoard::Start
 * Start Thread
 */
void KeyBoard::Start()
{
    stopThread = false;
    thread.start();
}

/**
 * @brief KeyBoard::Stop
 * Detener Thread
 */
void KeyBoard::Stop()
{
    thread.wait();      // if you want synchronous stop
}

void KeyBoard::Close(){}

/**
 * @brief KeyBoard::Killme
 * Terminar Thread
 */
void KeyBoard::Killme()
{
   emit thread.terminate();
}

/**
 * @brief KeyBoard::Open
 * Open Dev
 */
void KeyBoard::Open()
{
#ifdef ARM
    const char *dev = "/dev/hy_FK_78";
    fd = open(dev, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        qDebug("[DEBUG][KeyBoard] Cannot open!");
        return;
    }
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
}

/**
 * @brief KeyBoard::MsgLoop_KeyBoard
 * Pooling keyboard
 */
void KeyBoard::MsgLoop_KeyBoard()
{
    unsigned char key[2];
    Open();
    Sleep s;
    BarCode bc;
    QString qstrBc;
    while(true){
#ifdef ARM
        s.msleep(10);
        ioctl(fd, IOCTL_KEY_GET,key);
#endif
#ifndef ARM
        //![1]   s.msleep(1000);
        //![1]    key[1]=MAKE;
        //![1]    key[0]=0x4D;
#endif
        if(key[1] == MAKE){
            switch(key[0]){
            case 0x00:emit sigMoas(SIGUSR_ESC);break;
            case 0x01:emit sigMoas(SIGUSR_CALCULADORA);Calc =! Calc;break;
            case 0x02:emit sigMoas(SIGUSR_SUSPENDER);break;
            case 0x03:emit sigMoas(SIGUSR_BALANZA);break;
            case 0x04:emit sigMoas(SIGUSR_ENVASE);break;
            case 0x05:emit sigMoas(SIGUSR_GARAGE_IN);break;
            case 0x06:emit sigMoas(SIGUSR_GARAGE_OUT);break;
            case 0x07:emit sigMoas(SIGUSR_ANUL);break;
            case 0x08:emit sigMoas(SIGUSR_RECIBO);break;
            case 0x09:if(Shift==false){emit sigMoas(SIGUSR_PRECIO);break;}else{emit sigMoas(SIGUSR_VER_ITEM);Shift=!Shift;break;}break;
            case 0x0A:emit sigMoas(SIGUSR_CLIENTE);break;
            case 0x0B:emit sigMoas(SIGUSR_UP);break;
            case 0x0C:emit sigMoas(SIGUSR_DOWN);break;
            case 0x0D:emit sigMoas(SIGUSR_DEPARTAMENTO_1);break;
            case 0x0E:emit sigMoas(SIGUSR_DEPARTAMENTO_2);break;
            case 0x0F:emit sigMoas(SIGUSR_DEPARTAMENTO_3);break;
            case 0x10:emit sigMoas(SIGUSR_DEPARTAMENTO_4);break;
            case 0x11:emit sigMoas(SIGUSR_DEPARTAMENTO_5);break;
            case 0x12:emit sigMoas(SIGUSR_DEPARTAMENTO_6);break;
            case 0x13:emit sigMoas(SIGUSR_DEPARTAMENTO_7);break;
            case 0x14:emit sigMoas(SIGUSR_CORREC);break;
            case 0x15:{if(Calc == false){emit sigMoas(SIGUSR_EXCANTIDAD);break;}}
            case 0x16:{if(Calc == false){emit sigMoas(SIGUSR_PLU);break;}}
            case 0x17:emit sigMoas(SIGUSR_DEPARTAMENTO);break;
            case 0x18:{this->Shift=!this->Shift;if(this->Shift==true){/*emit notifyUsr("INFO: Shift ON");*/break;}}
            case 0x19:if(Shift==false){emit sigMoas(SIGUSR_CAJERO);break;}
            case 0x1A:{if(Qwerty==true){emit sigMoas(SIGUSR_Q);}break;}
            case 0x1B:{if(Qwerty==true){emit sigMoas(SIGUSR_W);}break;}
            case 0x1C:{if(Qwerty==true){emit sigMoas(SIGUSR_E);}break;}
            case 0x1D:{if(Qwerty==true){emit sigMoas(SIGUSR_R);}break;}
            case 0x1E:{if(Qwerty==true){emit sigMoas(SIGUSR_T);}break;}
            case 0x1F:{if(Qwerty==true){emit sigMoas(SIGUSR_Y);}break;}
            case 0x20:if(Qwerty==true && Shift==false){emit sigMoas(SIGUSR_U);break;}
            case 0x21:if(Calc == false){emit sigMoas(SIGUSR_SURCHARGE);break;}
            case 0x22:emit sigMoas(SIGUSR_7);break;
            case 0x23:emit sigMoas(SIGUSR_8);break;
            case 0x24:emit sigMoas(SIGUSR_9);break;
            case 0x25:if(Shift==false){emit sigMoas(SIGUSR_CASH);break;}else{emit sigMoas(SIGUSR_CREDIT);Shift = Shift;break;}
            case 0x26:if(Shift==false){emit sigMoas(SIGUSR_DEBIT);break;}else{emit sigMoas(SIGUSR_CHEQ);Shift = Shift;break;}
            case 0x27:{if(Qwerty==true && Shift==false){emit sigMoas(SIGUSR_A);break;}}
            case 0x28:{if(Qwerty==true){emit sigMoas(SIGUSR_S);}break;}
            case 0x29:{if(Qwerty==true){emit sigMoas(SIGUSR_D);}break;}
            case 0x2A:{if(Qwerty==true){emit sigMoas(SIGUSR_F);}break;}
            case 0x2B:{if(Qwerty==true){emit sigMoas(SIGUSR_G);}break;}
            case 0x2C:{if(Qwerty==true){emit sigMoas(SIGUSR_H);}break;}
            case 0x2D:{if(Qwerty==true){emit sigMoas(SIGUSR_J);}break;}
            case 0x2E:{if(Calc == false){emit sigMoas(SIGUSR_DISCOUNT);break;}}
            case 0x2F:emit sigMoas(SIGUSR_4);break;
            case 0x30:emit sigMoas(SIGUSR_5);break;
            case 0x31:emit sigMoas(SIGUSR_6);break;
            case 0x32:emit sigMoas(SIGUSR_NDC);
            case 0x33:emit sigMoas(SIGUSR_MEANOFPAY);break;
            case 0x34:{if(Qwerty==true && Shift==false){emit sigMoas(SIGUSR_I);break;}}
            case 0x35:{if(Qwerty==true){emit sigMoas(SIGUSR_O);}break;}
            case 0x36:{if(Qwerty==true){emit sigMoas(SIGUSR_P);}break;}
            case 0x37:{if(Qwerty==true){emit sigMoas(SIGUSR_K);}break;}
            case 0x38:{if(Qwerty==true){emit sigMoas(SIGUSR_L);}break;}
            case 0x39:{if(Qwerty==true && Shift==false){emit sigMoas(SIGUSR_N);break;}}
            case 0x3A:{if(Qwerty==true){emit sigMoas(SIGUSR_M);}break;}
            case 0x3B:if(Shift==true){emit sigMoas(SIGUSR_FONDO);Shift=!Shift;break;}else{emit sigMoas(SIGUSR_RETIRO);break;}
            case 0x3C:emit sigMoas(SIGUSR_1);break;
            case 0x3D:emit sigMoas(SIGUSR_2);break;
            case 0x3E:emit sigMoas(SIGUSR_3);break;
            case 0x3F:
            case 0x40:emit sigMoas(SIGUSR_SUB_TOTAL);break;
            case 0x41:emit sigMoas(SIGUSR_HOME);break;
            case 0x42:{if(Qwerty==true){emit sigMoas(SIGUSR_Z);}break;}
            case 0x43:{if(Qwerty==true){emit sigMoas(SIGUSR_X);}break;}
            case 0x44:{if(Qwerty==true){emit sigMoas(SIGUSR_C);}break;}
            case 0x45:{if(Qwerty==true){emit sigMoas(SIGUSR_V);}break;}
            case 0x46:{if(Qwerty==true){emit sigMoas(SIGUSR_B);}break;}
            case 0x47:emit sigMoas(SIGUSR_SPACE);break;
            case 0x48:if(Shift==false){emit sigMoas(SIGUSR_BORRAR_C);break;}else{emit sigMoas(SIGUSR_CANCELINVOID);this->Shift=!this->Shift;break;}
            case 0x49:emit sigMoas(SIGUSR_0);break;
            case 0x4A:emit sigMoas(SIGUSR_00);break;
            case 0x4B:emit sigMoas(SIGUSR_POINT);break;
            case 0x4C:
            case 0x4D:if(Shift==false){emit sigMoas(SIGUSR_TOTAL);break;}
            default :break;
            }
        }

        bc.readBarCode(qstrBc);
        if(!qstrBc.isEmpty()){
            emit sigBarCode(qstrBc);
            qstrBc.clear();
        }

    }
}



