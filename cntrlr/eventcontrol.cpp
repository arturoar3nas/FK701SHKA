#include "eventcontrol.h"

EventControl::EventControl(QObject *parent) : QObject(parent)
{

}

void EventControl::start()
{
    kb = new KeyBoard();
    fk = new Fk701sApp();
    fk->start();
    bWakeUp = false;
    connect(kb,SIGNAL(sigMoas(int)),this,SLOT(SignalListener(int)));
    connect(kb,SIGNAL(sigBarCode(QString)),this,SLOT(eventControlBarCode(QString)));
}

void EventControl::eventControlBarCode(QString qstr)
{
    fk->pressKey_Qwerty(qstr);
}

EventControl::st_listenerSignal st_listener[]={
    {	SIGUSR_ESC              ,EVENT_ESC,         "" },
    {	SIGUSR_CALCULADORA  	,EVENT_NULL,        "" },
    {	SIGUSR_SUSPENDER  		,EVENT_SUSPEND,     "" },
    {	SIGUSR_BALANZA  		,EVENT_BALANZA,     "" },
    {	SIGUSR_ENVASE  			,EVENT_ENVASE,      "" },
    {	SIGUSR_GARAGE_IN  		,EVENT_NULL,        "" },
    {	SIGUSR_GARAGE_OUT  		,EVENT_NULL,        "" },
    {	SIGUSR_ANUL  			,EVENT_ANUL,        "" },
    {	SIGUSR_RECIBO		    ,EVENT_NULL,        "" },
    {	SIGUSR_VER_ITEM  		,EVENT_SHOW_PLU,    "" },
    {	SIGUSR_CLIENTE  		,EVENT_CLIENT,      "" },
    {	SIGUSR_UP   			,EVENT_UP,          "" },
    {	SIGUSR_DOWN  			,EVENT_DOWN,        "" },
    {	SIGUSR_DEPARTAMENTO_1  	,EVENT_DIRECT_DEPT, "1"},
    {	SIGUSR_DEPARTAMENTO_2  	,EVENT_DIRECT_DEPT, "2"},
    {	SIGUSR_DEPARTAMENTO_3  	,EVENT_DIRECT_DEPT, "3"},
    {	SIGUSR_DEPARTAMENTO_4  	,EVENT_DIRECT_DEPT, "4"},
    {	SIGUSR_DEPARTAMENTO_5  	,EVENT_DIRECT_DEPT, "5"},
    {	SIGUSR_DEPARTAMENTO_6  	,EVENT_DIRECT_DEPT, "6"},
    {	SIGUSR_DEPARTAMENTO_7  	,EVENT_DIRECT_DEPT, "7"},
    {	SIGUSR_CORREC  			,EVENT_CORREC,      "" },
    {	SIGUSR_EXCANTIDAD  		,EVENT__KEY_X,   "keyX"},
    {	SIGUSR_PLU  			,EVENT_PLU,         "" },
    {	SIGUSR_DEPARTAMENTO  	,EVENT_DEPARTMENT,  "" },
    {	SIGUSR_CAJERO  			,EVENT_CAJERO,      "" },
    {	SIGUSR_Q  				,EVENT_QWERTY,      "Q"},
    {	SIGUSR_W  				,EVENT_QWERTY,      "W"},
    {	SIGUSR_E  				,EVENT_QWERTY,      "E"},
    {	SIGUSR_R  				,EVENT_QWERTY,      "R"},
    {	SIGUSR_T  				,EVENT_QWERTY,      "T"},
    {	SIGUSR_Y  				,EVENT_QWERTY,      "Y"},
    {	SIGUSR_U  				,EVENT_QWERTY,      "U"},
    {	SIGUSR_7  				,EVENT_QWERTY,      "7"},
    {	SIGUSR_8  				,EVENT_QWERTY,      "8"},
    {	SIGUSR_9  				,EVENT_QWERTY,      "9"},
    {	SIGUSR_A  				,EVENT_QWERTY,      "A"},
    {	SIGUSR_S  				,EVENT_QWERTY,      "S"},
    {	SIGUSR_D  				,EVENT_QWERTY,      "D"},
    {	SIGUSR_F  				,EVENT_QWERTY,      "F"},
    {	SIGUSR_G  				,EVENT_QWERTY,      "G"},
    {	SIGUSR_H  				,EVENT_QWERTY,      "H"},
    {	SIGUSR_J  				,EVENT_QWERTY,      "J"},
    {	SIGUSR_DISCOUNT  		,EVENT_DISCOUNT,    "" },
    {	SIGUSR_4  				,EVENT_QWERTY,      "4"},
    {	SIGUSR_5  				,EVENT_QWERTY,      "5"},
    {	SIGUSR_6  				,EVENT_QWERTY,      "6"},
    {	SIGUSR_I  				,EVENT_QWERTY,      "I"},
    {	SIGUSR_O  				,EVENT_QWERTY,      "O"},
    {	SIGUSR_P  				,EVENT_QWERTY,      "P"},
    {	SIGUSR_K  				,EVENT_QWERTY,      "K"},
    {	SIGUSR_L  				,EVENT_QWERTY,      "L"},
    {	SIGUSR_N  				,EVENT_QWERTY,      "N"},
    {	SIGUSR_M  				,EVENT_QWERTY,      "M"},
    {	SIGUSR_FONDO  			,EVENT_QWERTY,      "1"},
    {	SIGUSR_1  				,EVENT_QWERTY,      "1"},
    {	SIGUSR_2  				,EVENT_QWERTY,      "2"},
    {	SIGUSR_3  				,EVENT_QWERTY,      "3"},
    {	SIGUSR_SUB_TOTAL  		,EVENT_SUBTOTAL,    "" },
    {	SIGUSR_HOME  			,EVENT_HOME,        "" },
    {	SIGUSR_Z  				,EVENT_QWERTY,      "Z"},
    {	SIGUSR_X  				,EVENT_QWERTY,      "X"},
    {	SIGUSR_C  				,EVENT_QWERTY,      "C"},
    {	SIGUSR_V  				,EVENT_QWERTY,      "V"},
    {	SIGUSR_B  				,EVENT_QWERTY,      "B"},
    {	SIGUSR_SPACE  			,EVENT_QWERTY,      " "},
    {	SIGUSR_BORRAR_C  		,EVENT_BORRAR_C,    "" },
    {	SIGUSR_0  				,EVENT_QWERTY,      "0"},
    {	SIGUSR_00  				,EVENT_QWERTY,     "00"},
    {	SIGUSR_POINT  			,EVENT_QWERTY,      "."},
    {	SIGUSR_TOTAL  			,EVENT_TOTAL,       "" },
    {	SIGUSR_PRECIO  			,EVENT_PRICE,       "" },
    {	SIGUSR_RETIRO  			,EVENT_RETIRO,      "0"},
    {	SIGUSR_SURCHARGE  		,EVENT_SURCHARGE,   "" },
    {	SIGUSR_CANCELINVOID  	,EVENT_CANCEL_INVOID,""},
    {	SIGUSR_CASH 			,EVENT_CANCEL_INVOID,""},
    {	SIGUSR_DEBIT 			,EVENT_DEBIT,       "" },
    {	SIGUSR_CREDIT 			,EVENT_CREDIT,      "" },
    {	SIGUSR_CHEQ  			,EVENT_CHEQ,        "" },
    {	SIGUSR_MEANOFPAY  		,EVENT_MEANS_OF_PAY,"" },
    {	SIGUSR_NDC  			,EVENT_NDC,         "" }
};

EventControl::fsmEventControl fsmEvent[]=
{
    {EVENT_QWERTY       ,&Fk701sApp::pressKey_Qwerty},
    {EVENT_PLU          ,&Fk701sApp::pressKey_Plu},
    {EVENT_DEPARTMENT   ,&Fk701sApp::pressKey_Dept},
    {EVENT_DIRECT_DEPT  ,&Fk701sApp::pressKey_DirectDept},
    {EVENT_BORRAR_C     ,&Fk701sApp::pressKey_BorrarC},
    {EVENT__KEY_X       ,&Fk701sApp::pressKey_X},
    {EVENT_TOTAL        ,&Fk701sApp::pressKey_Total},
    {EVENT_PRICE        ,&Fk701sApp::pressKey_Price},
    {EVENT_TEST         ,&Fk701sApp::pressKey_ChangeView},
    {EVENT_UP           ,&Fk701sApp::pressKey_FocusUp},
    {EVENT_DOWN         ,&Fk701sApp::pressKey_FocusDown},
    {EVENT_CLIENT       ,&Fk701sApp::pressKey_Client},
    {EVENT_DRAWER       ,&Fk701sApp::pressKey_Drawer},
    {EVENT_SHOW_PLU     ,&Fk701sApp::pressKey_ShowPlu},
    {EVENT_CAJERO       ,&Fk701sApp::pressKey_Cashier},
    {EVENT_HOME         ,&Fk701sApp::pressKey_Home},
    {EVENT_FONDO        ,&Fk701sApp::pressKey_Drawer},
    {EVENT_RETIRO       ,&Fk701sApp::pressKey_Drawer},
    {EVENT_DISCOUNT     ,&Fk701sApp::pressKey_Discount},
    {EVENT_SURCHARGE    ,&Fk701sApp::pressKey_Surcharge},
    {EVENT_CANCEL_INVOID,&Fk701sApp::pressKey_CloseInvoid},
    {EVENT_CORREC       ,&Fk701sApp::pressKey_Correc},
    {EVENT_ANUL         ,&Fk701sApp::pressKey_Anul},
    {EVENT_CASH         ,&Fk701sApp::pressKey_Cash},
    {EVENT_DEBIT        ,&Fk701sApp::pressKey_Debit},
    {EVENT_CREDIT       ,&Fk701sApp::pressKey_Credit},
    {EVENT_CHEQ         ,&Fk701sApp::pressKey_Check},
    {EVENT_ENVASE       ,&Fk701sApp::pressKey_bottle},
    {EVENT_SUSPEND      ,&Fk701sApp::pressKey_suspend},
    {EVENT_SUBTOTAL     ,&Fk701sApp::pressKey_SubTotal},
    {EVENT_MEANS_OF_PAY ,&Fk701sApp::pressKey_meansOfPay},
    {EVENT_NDC          ,&Fk701sApp::pressKey_NDC},
    {EVENT_BALANZA      ,&Fk701sApp::pressKey_balance},
    {EVENT_ESC          ,&Fk701sApp::pressKey_Esc}
};

void EventControl::SignalListener(int nSignal){
    qDebug() << "nSignal signalListener" << nSignal;
    const int nSIZE = sizeof(st_listener)/sizeof(st_listener[0]);
    st_listenerSignal const * pEntry = &st_listener[0];
    st_listenerSignal const * pEnd = &st_listener[nSIZE];
    bool bStateFound = false;
    while ((!bStateFound) && (pEntry != pEnd)){
        if (pEntry->nSignal == nSignal){
                nodo n = {pEntry->events,pEntry->data};
                FIFO(n);
                bStateFound = true;
                break;
        }
        ++pEntry;
    }
    if(!bWakeUp){
        bWakeUp = true;
        event();
    }
}


void EventControl::unlockBlockError(int nSignal)
{
    if(nSignal == SIGUSR_TOTAL){
        qDebug()<<"[DEBUG][EventControl] Señal letra TotalEfect!";
        nodo n = {EVENT_TOTAL,""};
        FIFO(n);
        event();
        connect(kb,SIGNAL(sigMoas(int)),this,SLOT(ReciverSignal(int)));
        disconnect(kb,SIGNAL(sigMoas(int)),this,SLOT(unlockBlockError(int)));
    }
}

void EventControl::event()
{
    nodo cola;
    int ret = 0;
    while(fifo.count() != 0) {
        cola = fifo.dequeue();
        Valorfifo = cola.valor;
        qDebug()<< " Contenido del FIFO " << "Metodo writeOnWidget "<< Valorfifo;
        qDebug() << "fsmEvent " << cola.metodo;
        ret = (fk->*fsmEvent[cola.metodo].fptr)(Valorfifo);
        if(ret < 0) {
            FIFOkill();
            break;
        }
    }
    bWakeUp = false;

}

void EventControl::FIFO(nodo tecla)
{
    fifo.enqueue(tecla);
}

void EventControl::FIFOkill()
{
    fifo.clear();
    disconnect(kb,SIGNAL(sigMoas(int)),this,SLOT(ReciverSignal(int)));
    connect(kb,SIGNAL(sigMoas(int)),this,SLOT(unlockBlockError(int)));
    qDebug()<< "Fifo clear!!!";
}













