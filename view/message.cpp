#include "message.h"

Message::Message(QWidget *parent)
        :QWidget(parent)
{
    qmapError.insert(-1,"No Se encuentra la impresora!");
    qmapError.insert(-2,"Error de Impresora!");
    qmapError.insert(-3,"No se encontro el PLU!");
    qmapError.insert(-4,"Error al Enviar el PLU!");
    qmapError.insert(-5,"Error al realizar pago!");
    qmapError.insert(-6,"No se puede realizar mas pagos!");
    qmapError.insert(-7,"Pago no realizado!");
    qmapError.insert(-8,"Error en el ID o Responsabilidad del Cliente!");
    qmapError.insert(-9,"Opcion invalida!");
    qmapError.insert(-10,"Privilegios insuficientes!");
    qmapError.insert(-11,"Ingrese una clave!");
    qmapError.insert(-12,"Ingrese un numero de cajero!");
    qmapError.insert(-13,"Error clave invalida!");
    qmapError.insert(-14,"Ingreso Invalido!");
    qmapError.insert(-15,"Error al abrir cajero!");
    qmapError.insert(-16,"No se puedo abrir la factura!");
    qmapError.insert(-17,"No se pudo realizar el descuento!");
    qmapError.insert(-18,"Error al cerrar la factura!");
    qmapError.insert(-19,"Error al ingresar datos!");
    qmapError.insert(-20,"Inventario Insuficiente!");
    qmapError.insert(-21,"Error entrada vacia!");
    qmapError.insert(-22,"Error al realizar el descuento o el recargo!");
    qmapError.insert(-23,"Error en el medio de pago!");
    qmapError.insert(-24,"Tipo de ID vacio ");
    qmapError.insert(-25,"Numero de Documento Vacio");
    qmapError.insert(-26,"Ingrese Responsabilidad");
    qmapError.insert(-27,"Ingrese razon social");
    qmapError.insert(-28,"Ingrese una direccion");
    qmapError.insert(-29,"Cliente vacio");
    qmapError.insert(-30,"No se admiten letras");
    qmapError.insert(-31,"Error longitud numero de documento");
    qmapError.insert(-32,"Error longitud razon social");
    qmapError.insert(-33,"Error tipo de ID");
    qmapError.insert(-34,"Error longitud tipo de ID");
    qmapError.insert(-35,"Error tipo de responsabilidad");
    qmapError.insert(-36,"Error longitud respuesta");
    qmapError.insert(-37,"Error longitud direccion");
    qmapError.insert(-38,"Error apertura de gaveta");
    qmapError.insert(-39,"Gaveta vacia");
    qmapError.insert(-40,"No se pudo abrir la gaveta");
    qmapError.insert(-41,"Error de lectura MSR");
    qmapError.insert(-42,"No se encontro Factura");

}

void Message::openMsgBox(QString qstrMsg)
{
    /*!
    !    if(__fMsgBoxIsOpen == true)
    !    {
    !        qDebug("[DEBUG][MainWindow] msgbox is open");
    !        return;
    !    }
    */
        timer = new QTimer(this);
        QTimer::singleShot(MAX_TIMER, this, SLOT(closeMessageBox()));
        msgBox  = new QMessageBox();
        msgBox->setWindowTitle("INFORMACION");
        msgBox->setText(QString::fromUtf8(qstrMsg.toUtf8()));
        msgBox->setStandardButtons(0);
    /*!
        QPalette plt;
        plt.setColor(QPalette::Background, Qt::darkGray);
        msgBox->setPalette(plt);
    */
        QFont f_24("helvetica",24, QFont::Bold);
        msgBox->setFont(f_24);
        msgBox->show();

        qApp->processEvents();
        timer->start(3000);
}

void Message::openMsgBoxInfo(QString qstrMsg)
{
    msgBox  = new QMessageBox();
    msgBox->setWindowTitle("INFORMACION");
    msgBox->setText(QString::fromUtf8(qstrMsg.toUtf8()));

    QFont f_24("helvetica",24, QFont::Bold);
    msgBox->setFont(f_24);
    msgBox->show();

    qApp->processEvents();
}

void Message::openMsgBoxError(int nError)
{
    msgBox  = new QMessageBox();
    msgBox->setWindowTitle("ERROR");
    QString qstrMsg = qmapError.value(nError);
    msgBox->setText(QString::fromUtf8(qstrMsg.toUtf8()));
/*!
    QPalette plt;
    plt.setColor(QPalette::Background, Qt::red);
    msgBox->setPalette(plt);
*/
    QFont f_24("helvetica",24, QFont::Bold);
    msgBox->setFont(f_24);
    msgBox->show();
    qApp->processEvents();
}

void Message::closeMsgBoxError()
{
    msgBox->close();
    delete msgBox;
}

void Message::closeMessageBox()
{
    msgBox->close();
    delete msgBox;
}
