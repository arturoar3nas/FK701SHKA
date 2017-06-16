#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <dev/keyboard.h>
#include <cntrlr/fk701sapp.h>
#include "fk701sapp.h"

#define TIMER_MAX_TIME_CONTROL        10

typedef enum
{
    EVENT_QWERTY,
    EVENT_PLU,
    EVENT_DEPARTMENT,
    EVENT_DIRECT_DEPT,
    EVENT_BORRAR_C,
    EVENT__KEY_X,
    EVENT_TOTAL,
    EVENT_PRICE,
    EVENT_TEST,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_CLIENT,
    EVENT_DRAWER,
    EVENT_SHOW_PLU,
    EVENT_CAJERO,
    EVENT_HOME,
    EVENT_FONDO,
    EVENT_RETIRO,
    EVENT_DISCOUNT,
    EVENT_SURCHARGE,
    EVENT_CANCEL_INVOID,
    EVENT_CORREC,
    EVENT_ANUL,
    EVENT_CASH,
    EVENT_DEBIT,
    EVENT_CREDIT,
    EVENT_CHEQ,
    EVENT_ENVASE,
    EVENT_SUSPEND,
    EVENT_SUBTOTAL,
    EVENT_MEANS_OF_PAY,
    EVENT_NDC,
    EVENT_BALANZA,
    EVENT_ESC,
    EVENT_NULL
}e_events;

typedef struct
{
    e_events metodo;
    QString valor;

}nodo;


class EventControl : public QObject
{
    Q_OBJECT
public:
    explicit EventControl(QObject *parent = 0);
    void FIFO(nodo tecla);
    void start();
    typedef struct{
        int nSignal;
        e_events events;
        QString data;
    }st_listenerSignal;
    typedef struct{
        e_events events;
        int (Fk701sApp::*fptr)(QString);
    }fsmEventControl;

private:
    bool bWakeUp;
    QQueue<nodo> fifo;
    KeyBoard *kb;
    Fk701sApp *fk;
    int control;                                    /*Variable de control*/
    QString Valorfifo;                              /*Variable de la fifo*/

public slots:
    void FIFOkill();
    void SignalListener(int nSignal);
    void unlockBlockError(int nSignal);
    void eventControlBarCode(QString qstr);
    void event();
private slots:

};
#endif // EVENTCONTROL_H
