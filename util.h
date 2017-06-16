#ifndef UTIL
#define UTIL

#include <QThread>

#define SUCESS              0
#define FAIL               -1
#define nullptr NULL

#define _arrow_up           0
#define _arrow_down         1

#define DEPARTAMENTO        0
#define PLU                 1

#define PRINTER             1
#define ARM                 1
//!#define DEBUG           1

#define BUFFER_MID_SIZE     64
#define BUFFER_MAX_SIZE     256

#define STX                 0X02
#define ETX                 0X03
#define ENQ                 0X05
#define ACK                 0X06
#define EOT                 0X04
#define NAK                 0X15
#define ETB                 0X17
#define MASK                0X7F
#define NUL                 0X00
#define LF                  0X0A
#define CR                  0X0D
#define FLD                 0X1C

#define UNIC_ITEM_SUCESS         5
#define CLEAR_UI                 4
#define PAY_INCOMPLETE           3
#define PAY_COMPLETE             2
#define NO_ACTION                1
#define PRINTER_NO_FOUND       (-1)
#define PRINTER_ERROR          (-2)
#define PLU_NO_FOUND           (-3)
#define PLU_SEND_ERROR         (-4)
#define PAY_ERROR              (-5)
#define PAY_FOUL_PAY           (-6)
#define PAY_NOT_PERFORMED      (-7)
#define CLIENT_ID_RESP_ERROR   (-8)
#define OPTION_INVALID         (-9)
#define NO_PRIVILEGE          (-10)
#define PASSWORD_EMPTY        (-11)
#define CASHIER_EMPTY         (-12)
#define PASSWORR_INVALID      (-13)
#define INPUT_INVALID         (-14)
#define CASHIER_OPEN_ERROR    (-15)
#define CANT_OPEN_TICKET      (-16)
#define CANT_DO_DISCOUNT      (-17)
#define CLOSE_TICKET_ERROR    (-18)
#define INPUT_ERROR           (-19)
#define LOW_SOTCK             (-20)
#define INPUT_EMPTY           (-21)
#define DISCNT_SURCHRG_ERROR  (-22)
#define MEAN_OF_PAY_ERROR     (-23)
#define CLIENT_EMPTY_ID       (-24)
#define CLIENT_EMPTY_NDOCUMENT (-25)
#define CLIENT_EMPTY_RESP     (-26)
#define CLIENT_EMPTY_RSOCIAL  (-27)
#define CLIENT_EMPTY_ADDRESS  (-28)
#define CLIENT_EMPTY          (-29)
#define NO_LETTER_ERROR       (-30)
#define LENGTH_NDOC_ERROR     (-31)
#define LENGTH_RSOC_ERROR     (-32)
#define CLIENT_ID_ERROR       (-33)
#define LENGHT_ID_ERROR       (-34)
#define CLIENT_RESP_ERROR     (-35)
#define LENGTH_RESP           (-36)
#define LENGHT_ADDRESS        (-37)
#define DRAWER_OPEN_ERROR     (-38)
#define DRAWER_EMPTY          (-39)
#define DRAWER_CAN            (-40)
#define MSR_ERROR             (-41)
#define INVOID_NO_FOUND       (-42)

#define STATE_GUI_LOGIN     0
#define STATE_GUI_SALE      1
#define STATE_GUI_CLIENT    2
#define STATE_GUI_DRAW      3
#define STATE_GUI_SHOW_PLU  4
#define STATE_GUI_INDEX_H   5
#define STATE_GUI_DIALOG    6
#define STATE_GUI_REPORT    7
#define STATE_GUI_INVENTORY 8
#define STATE_GUI_CASH_S    9

#define STATE_NUMBER    7


#define STATE_FSM_LOGIN     0
#define STATE_FSM_SALE      1
#define STATE_FSM_CLIENT    2
#define STATE_FSM_INDEX_H   3
#define STATE_FSM_INDEX_X   4
#define STATE_FSM_INDEX_RX  5
#define STATE_FSM_INVENTORY 6

#define SIGUSR_ESC 	 			0
#define	SIGUSR_CALCULADORA  	1
#define SIGUSR_SUSPENDER  		2
#define SIGUSR_BALANZA  		3
#define SIGUSR_ENVASE  			4
#define SIGUSR_GARAGE_IN  		5
#define SIGUSR_GARAGE_OUT  		6
#define SIGUSR_ANUL  			7
#define SIGUSR_RECIBO		    8
#define SIGUSR_VER_ITEM  		9
#define SIGUSR_CLIENTE  		10
#define SIGUSR_UP   			11
#define SIGUSR_DOWN  			12
#define SIGUSR_DEPARTAMENTO_1  	13
#define SIGUSR_DEPARTAMENTO_2  	14
#define SIGUSR_DEPARTAMENTO_3  	15
#define SIGUSR_DEPARTAMENTO_4  	16
#define SIGUSR_DEPARTAMENTO_5  	17
#define SIGUSR_DEPARTAMENTO_6  	18
#define SIGUSR_DEPARTAMENTO_7  	19
#define SIGUSR_CORREC  			20
#define SIGUSR_EXCANTIDAD  		21
#define SIGUSR_PLU  			22
#define SIGUSR_DEPARTAMENTO  	23
#define SIGUSR_CAJERO  			25
#define SIGUSR_Q  				26
#define SIGUSR_W  				27
#define SIGUSR_E  				28
#define SIGUSR_R  				29
#define SIGUSR_T  				30
#define SIGUSR_Y  				31
#define SIGUSR_U  				32
#define SIGUSR_7  				34
#define SIGUSR_8  				35
#define SIGUSR_9  				36
#define SIGUSR_A  				39
#define SIGUSR_S  				40
#define SIGUSR_D  				41
#define SIGUSR_F  				42
#define SIGUSR_G  				43
#define SIGUSR_H  				44
#define SIGUSR_J  				45
#define SIGUSR_DISCOUNT  		46
#define SIGUSR_4  				47
#define SIGUSR_5  				48
#define SIGUSR_6  				49
#define SIGUSR_I  				52
#define SIGUSR_O  				53
#define SIGUSR_P  				54
#define SIGUSR_K  				55
#define SIGUSR_L  				56
#define SIGUSR_N  				57
#define SIGUSR_M  				58
#define SIGUSR_FONDO  			59
#define SIGUSR_1  				60
#define SIGUSR_2  				61
#define SIGUSR_3  				62
#define SIGUSR_SUB_TOTAL  		64
#define SIGUSR_HOME  			65
#define SIGUSR_Z  				66
#define SIGUSR_X  				67
#define SIGUSR_C  				68
#define SIGUSR_V  				69
#define SIGUSR_B  				70
#define SIGUSR_SPACE  			71
#define SIGUSR_BORRAR_C  		72
#define SIGUSR_0  				73
#define SIGUSR_00  				74
#define SIGUSR_POINT  			75
#define SIGUSR_TOTAL  			77
#define SIGUSR_PRECIO  			85
#define SIGUSR_RETIRO  			86
#define SIGUSR_SURCHARGE  		87
#define SIGUSR_CANCELINVOID  	88
#define SIGUSR_CASH 			89
#define SIGUSR_DEBIT 			90
#define SIGUSR_CREDIT 			91
#define SIGUSR_CHEQ  			92
#define SIGUSR_MEANOFPAY  		93
#define SIGUSR_NDC  			94

//!------------------------------------------
//!Base de Datos
/*!
╔═══════════════╤═══════════════════╤═════════════╤═════════════════╤════════════════╤═════════════════════╤════════════════╤═════════════════╤═════════════════════════╤═════════╤══════════╤═════╤══════════════╤══════════╤═════════╤══════════╤═══════╤═══════════╤══════════════╗
║               │ Key               │ 0           │ 1               │ 2              │ 3                   │ 4              │ 5               │ 6                       │ 7       │ 8        │ 9   │ 10           │ 11       │ 12      │ 13       │ 14    │ 15        │ 16           ║
╠═══════════════╪═══════════════════╪═════════════╪═════════════════╪════════════════╪═════════════════════╪════════════════╪═════════════════╪═════════════════════════╪═════════╪══════════╪═════╪══════════════╪══════════╪═════════╪══════════╪═══════╪═══════════╪══════════════╣
║ PLU           │ PLU_Número        │ Descripción │ Descripción #1  │ Descripción #2 │ Descripción #3      │ Descripción #4 │ Cantidad        │ Precio                  │ IVA     │ IIF      │ IIP │ Inventario   │ Tipo     │ Abierto │ Negativo │ Unico │ Vinculado │ Departamento ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Departamento  │ DEPT_Número       │ Descripción │ Descripción #1  │ Descripción #2 │ Descripción #3      │ Descripción #4 │ Cantidad        │ Precio                  │ IVA     │ IIF      │ IIP │ Monto Máximo │     0    │ Abierto │ Negativo │ Unico │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Medio de Pago │ PMethod_Número    │ Número      │ Descripción     │ Tasa de Cambio │ Monto Máximo        │ Cambio         │ Pago Parcial    │ Imprimir Identificación │ Cajón   │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Cliente       │ CLIENT_Número     │ Número      │ Responsabilidad │ Tipo ID        │ Dirección           │ Razón Social   │                 │                         │         │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Cajero        │ CASHIER_Número    │ Número      │ Contraseña      │ Nombre         │ Nivel de Privilegio │                │                 │                         │         │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Mozo          │ MOZO_Número       │ Número      │ Nombre          │ Contraseña     │                     │                │                 │                         │         │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Categoría     │ GarageC_Número    │ Número      │ Nombre          │                │                     │                │                 │                         │         │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Unidad        │ GarageU_Número    │ Número      │ Nombre          │ Fracción       │ Tolerancia          │ Tiempo Máximo  │ Unidad Fracción │ Minimo                  │ Máximo  │          │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Precio        │ GarageP_Categoria │ Categoria   │ Unidad Tiempo   │ Nombre         │ Precio              │ IVA            │ Departamento    │ Impuesto Interno        │ Abierto │ Negativo │     │              │          │         │          │       │           │              ║
╟───────────────┼───────────────────┼─────────────┼─────────────────┼────────────────┼─────────────────────┼────────────────┼─────────────────┼─────────────────────────┼─────────┼──────────┼─────┼──────────────┼──────────┼─────────┼──────────┼───────┼───────────┼──────────────╢
║ Vinculados    │ Vncld_Grupo       │ PLU_1       │ PLU_2           │ PLU_3          │ PLU_n               │                │                 │                         │         │          │     │              │          │         │          │       │           │              ║
╚═══════════════╧═══════════════════╧═════════════╧═════════════════╧════════════════╧═════════════════════╧════════════════╧═════════════════╧═════════════════════════╧═════════╧══════════╧═════╧══════════════╧══════════╧═════════╧══════════╧═══════╧═══════════╧══════════════╝
*/


class Sleep : public QThread
{
public:
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
};
#endif // UTIL

