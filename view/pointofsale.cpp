#include "pointofsale.h"

PointOfSale::PointOfSale(QWidget *parent)
    :QWidget(parent)
{
    int nwidth = 200;//200

    /*! Fila 1*/
    {
    label = new QLabel;
    label->setFixedWidth(nwidth);
    label_2 = new QLabel;
    label_2->setFixedWidth(nwidth);
    label_3 = new QLabel;
    label_3->setFixedWidth(nwidth);
    label_4 = new QLabel;
    label_4->setFixedWidth(nwidth);
    label_5 = new QLabel;
    label_5->setFixedWidth(nwidth);
    }
    /*! Fila 2*/
    {
    label_12 = new QLabel;
    label_12->setFixedWidth(nwidth);
    label_22 = new QLabel;
    label_22->setFixedWidth(nwidth);
    label_32 = new QLabel;
    label_32->setFixedWidth(nwidth);
    label_42 = new QLabel;
    label_42->setFixedWidth(nwidth);
    label_52 = new QLabel;
    label_52->setFixedWidth(nwidth);
    }
    /*! Fila 3*/
    {
    label_13 = new QLabel;
    label_13->setFixedWidth(nwidth);
    label_23 = new QLabel;
    label_23->setFixedWidth(nwidth);
    label_33 = new QLabel;
    label_33->setFixedWidth(nwidth);
    label_43 = new QLabel;
    label_43->setFixedWidth(nwidth);
    label_53 = new QLabel;
    label_53->setFixedWidth(nwidth);
    }
    /*! Fila 4*/
    {
    label_14 = new QLabel;
    label_14->setFixedWidth(nwidth);
    label_24 = new QLabel;
    label_24->setFixedWidth(nwidth);
    label_34 = new QLabel;
    label_34->setFixedWidth(nwidth);
    label_44 = new QLabel;
    label_44->setFixedWidth(nwidth);
    label_54 = new QLabel;
    label_54->setFixedWidth(nwidth);
    }
    /*! Fila 5*/
    {
    label_15 = new QLabel;
    label_15->setFixedWidth(nwidth);
    label_25 = new QLabel;
    label_25->setFixedWidth(nwidth);
    label_35 = new QLabel;
    label_35->setFixedWidth(nwidth);
    label_45 = new QLabel;
    label_45->setFixedWidth(nwidth);
    label_55 = new QLabel;
    label_55->setFixedWidth(nwidth);
    }
    /*! Line edit*/
    {
    lineEdit = new QLineEdit;
    lineEdit->setFixedWidth(400);
    lineEdit->setMaxLength(15);
    lineEdit->setValidator(new QDoubleValidator((long long) 0,(long long) 999999999999999.99,2,this));/*solo numeros*/
    }
    /*! Label total*/
    {
    label_total = new QLabel;
    label_total->setFrameShadow(QFrame::Plain);
    label_total->setFrameShape(QFrame::Box);
    label_total->setAlignment(Qt::AlignHCenter | Qt::AlignRight);
    label_total->setLineWidth(1);
    label_total->setFixedWidth(250);
    label_total->setFixedHeight(50);
    }
    /*! Foot*/
    {
    label_foot = new QLabel(" Casa Trasorras - Argentina  Cajero: ");
    label_foot->setFrameShadow(QFrame::Plain);
    label_foot->setFrameShape(QFrame::Box);
    label_foot->setLineWidth(1);
    }
    /*! Grid layout*/
    {
    layout = new QGridLayout;
    layout->addWidget(createLabel(tr(""))  ,0  ,0);
    layout->addWidget(createLabel(tr(" Operacion: ")),1 ,0,Qt::AlignHCenter);
    layout->addWidget(lineEdit  ,1  ,1,Qt::AlignLeft);
    layout->addWidget(createLabel(tr(""))  ,2  ,0);
    layout->addWidget(createLabel(tr("ARTICULO "))  ,3  ,0);
    layout->addWidget(createLabel(tr("PRECIO"))     ,3  ,1);
    layout->addWidget(createLabel(tr("CANTIDAD"))   ,3  ,2);
    layout->addWidget(createLabel(tr("IVA(%)"))     ,3  ,3);
    layout->addWidget(createLabel(tr("TOTAL"))      ,3  ,4);
    layout->addWidget(label     ,4  ,0);
    layout->addWidget(label_2   ,5  ,0);
    layout->addWidget(label_3   ,6  ,0);
    layout->addWidget(label_4   ,7  ,0);
    layout->addWidget(label_5   ,8  ,0);

    layout->addWidget(label_12  ,4  ,1);
    layout->addWidget(label_22  ,5  ,1);
    layout->addWidget(label_32  ,6  ,1);
    layout->addWidget(label_42  ,7  ,1);
    layout->addWidget(label_52  ,8  ,1);

    layout->addWidget(label_13  ,4  ,2);
    layout->addWidget(label_23  ,5  ,2);
    layout->addWidget(label_33  ,6  ,2);
    layout->addWidget(label_43  ,7  ,2);
    layout->addWidget(label_53  ,8  ,2);

    layout->addWidget(label_14  ,4  ,3);
    layout->addWidget(label_24  ,5  ,3);
    layout->addWidget(label_34  ,6  ,3);
    layout->addWidget(label_44  ,7  ,3);
    layout->addWidget(label_54  ,8  ,3);

    layout->addWidget(label_15  ,4  ,4);
    layout->addWidget(label_25  ,5  ,4);
    layout->addWidget(label_35  ,6  ,4);
    layout->addWidget(label_45  ,7  ,4);
    layout->addWidget(label_55  ,8  ,4);
    layout->addWidget(createLabel(tr(""))  ,9  ,0);
    layout->addWidget(createLabel(tr("TOTAL: ")),10  ,2);
    layout->addWidget(label_total  ,10 ,3);
    layout->addWidget(label_foot   ,11 ,0 ,1 ,5);
    setLayout(layout);
    }

    resize(800, 480);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateLabels()),Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    timer->start(150);

    bflag = false;
    nSheets = 1;
    nCurrentSheet = 1;
    m_nlines = 0;
}

QLabel *PointOfSale::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    //lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}

void PointOfSale::updateLabels()
{
    if(qstrlItem.isEmpty() == false){
        QStringList qstrl;
        if(nCurrentSheet!=nSheets)restore();
        foreach (const QString &qstr, qstrlItem){
            qstrl = qstr.split(";");
            if(m_nlines > 4){
                m_nlines = 0;
                nSheets++;
                nCurrentSheet++;
                cleanLabels();
            }
            switch(m_nlines){
            case 0:label  ->setText(qstrl.at(0));label_12->setText(qstrl.at(1));label_13->setText(qstrl.at(2));label_14->setText(qstrl.at(3));label_15->setText(qstrl.at(4));break;
            case 1:label_2->setText(qstrl.at(0));label_22->setText(qstrl.at(1));label_23->setText(qstrl.at(2));label_24->setText(qstrl.at(3));label_25->setText(qstrl.at(4));break;
            case 2:label_3->setText(qstrl.at(0));label_32->setText(qstrl.at(1));label_33->setText(qstrl.at(2));label_34->setText(qstrl.at(3));label_35->setText(qstrl.at(4));break;
            case 3:label_4->setText(qstrl.at(0));label_42->setText(qstrl.at(1));label_43->setText(qstrl.at(2));label_44->setText(qstrl.at(3));label_45->setText(qstrl.at(4));break;
            case 4:label_5->setText(qstrl.at(0));label_52->setText(qstrl.at(1));label_53->setText(qstrl.at(2));label_54->setText(qstrl.at(3));label_55->setText(qstrl.at(4));break;
            default:break;
            }
            sheets.append(qstr);
            m_nlines++;
        }
        qstrlItem.clear();
        label_total->setText(qstrLcdTotal);
        qApp->processEvents();
    }
}

int PointOfSale::restore()
{
    nCurrentSheet = nSheets - 1;
    int nElements = (nCurrentSheet*5);
    int nCount = 0;
    QStringList qstrlstrl;
    for(nElements;nElements < sheets.count();nElements++){
        qstrlstrl.append(sheets.at(nElements));
        if(nCount == 4)break;
        nCount++;
    }

    /*limpiamos la pantalla*/
    cleanLabels();
    m_nlines = 0; /*empezamos de la primera linea*/

    /*cargamos la informacion en la gui*/
    QStringList qstrl;
    foreach (const QString &qstr, qstrlstrl){
        qstrl = qstr.split(";");
        switch(m_nlines){
        case 0:label  ->setText(qstrl.at(0));label_12->setText(qstrl.at(1));label_13->setText(qstrl.at(2));label_14->setText(qstrl.at(3));label_15->setText(qstrl.at(4));break;
        case 1:label_2->setText(qstrl.at(0));label_22->setText(qstrl.at(1));label_23->setText(qstrl.at(2));label_24->setText(qstrl.at(3));label_25->setText(qstrl.at(4));break;
        case 2:label_3->setText(qstrl.at(0));label_32->setText(qstrl.at(1));label_33->setText(qstrl.at(2));label_34->setText(qstrl.at(3));label_35->setText(qstrl.at(4));break;
        case 3:label_4->setText(qstrl.at(0));label_42->setText(qstrl.at(1));label_43->setText(qstrl.at(2));label_44->setText(qstrl.at(3));label_45->setText(qstrl.at(4));break;
        case 4:label_5->setText(qstrl.at(0));label_52->setText(qstrl.at(1));label_53->setText(qstrl.at(2));label_54->setText(qstrl.at(3));label_55->setText(qstrl.at(4));break;
        default:break;
        }
        m_nlines++;
    }

    return 0;
}

void PointOfSale::cleanLabels()
{
    label  ->clear();
    label_2->clear();
    label_3->clear();
    label_4->clear();
    label_5->clear();
    label_12->clear();
    label_22->clear();
    label_32->clear();
    label_42->clear();
    label_52->clear();
    label_13->clear();
    label_23->clear();
    label_33->clear();
    label_43->clear();
    label_53->clear();
    label_14->clear();
    label_24->clear();
    label_34->clear();
    label_44->clear();
    label_54->clear();
    label_15->clear();
    label_25->clear();
    label_35->clear();
    label_45->clear();
    label_55->clear();

}

void PointOfSale::updateWidgets(void *sender)
{
    UpdateSale *us = static_cast<UpdateSale*>(sender);
    QStringList qstrl = us->qstrlItem();
    QStringList qstrlCalc = us->qstrCalcs();
    QStringList qstrlTmp;
    QString qstrItem;
    int n = 0;
    foreach(const QString &str, qstrl){
        qstrlTmp = str.split(";");
        qstrItem.append(qstrlTmp.at(0));/*Descripcion*/
        qstrItem.append(";");
        qstrItem.append(qstrlTmp.at(6));/*Precio*/
        qstrItem.append(";");
        qstrItem.append(qstrlTmp.at(5));/*Cantidad*/
        qstrItem.append(";");
        qstrlTmp[7].insert(2,".");
        qstrItem.append(qstrlTmp.at(7));/*Iva*/
        qstrItem.append(";");
        qstrItem.append(qstrlCalc.at(n));
        qstrItem.append(";");
        qstrlItem.append(qstrItem);
        n++;
        qstrItem.clear();
        qstrlTmp.clear();
    }
    qstrLcdTotal = us->qstrTotal();
}

void PointOfSale::writeOnWidget(QString qstr)
{
    lineEdit->insert(qstr);
}

int PointOfSale::getFromWidgets(QString &qstr)
{
    qstr = lineEdit->text();
    lineEdit->clear();
    if(!qstr.isEmpty())
        return 0;
    else
        return -1;
}

void PointOfSale::backSpaceLineEdit()
{
    lineEdit->backspace();
}
/**
 * @brief PointOfSale::focusUp
 * @return
 * Cambio de pagina
 */
int PointOfSale::focusUp()
{
    /*si presionamos arriba es atras*/
    /*si presionamos atras entonces
      preguntamos en que numero de pagina estamos*/
    /*si es mayor a uno podemos retroceder*/
    /*si es mayor a 1 mostramos los 0-4 primeros*/
    /*si es mayor a 2 mostramos del 5 al 9*/
    /*si es mayor a 3 mostramos del 10 al 14*/
    /*si es mayor a 4 mostramos del 15 al 19*/
    /*si es mayor a 5 mostramos del 20 al 24*/
    if(nCurrentSheet <= 1){
        return -1;
    }

    /*calculamos el retroceso que es la pagina actual
      por 5 items que tiene la pantalla
      menos 1 de ajuste
      menos 5 de retroceso
      menos 4 para posicionarnos en el primer item*/
    int nElements = (nCurrentSheet*5)-10;

    /*actualizamos el numero de la pantalla*/
    nCurrentSheet = nCurrentSheet -1;
    qstrlItem.append(sheets.at(nElements++));
    qstrlItem.append(sheets.at(nElements++));
    qstrlItem.append(sheets.at(nElements++));
    qstrlItem.append(sheets.at(nElements++));
    qstrlItem.append(sheets.at(nElements++));

    /*limpiamos la pantalla*/
    cleanLabels();
    m_nlines = 0; /*empezamos de la primera linea*/

    /*cargamos la informacion en la gui*/
    QStringList qstrl;
    foreach (const QString &qstr, qstrlItem){
        qstrl = qstr.split(";");
        switch(m_nlines){
        case 0:label  ->setText(qstrl.at(0));label_12->setText(qstrl.at(1));label_13->setText(qstrl.at(2));label_14->setText(qstrl.at(3));label_15->setText(qstrl.at(4));break;
        case 1:label_2->setText(qstrl.at(0));label_22->setText(qstrl.at(1));label_23->setText(qstrl.at(2));label_24->setText(qstrl.at(3));label_25->setText(qstrl.at(4));break;
        case 2:label_3->setText(qstrl.at(0));label_32->setText(qstrl.at(1));label_33->setText(qstrl.at(2));label_34->setText(qstrl.at(3));label_35->setText(qstrl.at(4));break;
        case 3:label_4->setText(qstrl.at(0));label_42->setText(qstrl.at(1));label_43->setText(qstrl.at(2));label_44->setText(qstrl.at(3));label_45->setText(qstrl.at(4));break;
        case 4:label_5->setText(qstrl.at(0));label_52->setText(qstrl.at(1));label_53->setText(qstrl.at(2));label_54->setText(qstrl.at(3));label_55->setText(qstrl.at(4));break;
        default:break;
        }
        m_nlines++;
    }
    qstrlItem.clear();
    qApp->processEvents();

    return 0;
}

/**
 * @brief PointOfSale::focusDown
 * @return
 * Cambio de pagina
 */
int PointOfSale::focusDown()
{
    /*si presionamos abajo es adelante*/
    /*si presionamos adelante entonces
      preguntamos en que numero de pagina estamos*/
    /*si la pagina actual es igual al numero
     *de hojas no podemos avanzar*/
    if(nCurrentSheet == nSheets){
        return -1;
    }
    int nCount = 0;
    /*calculamos el adelanto de la pagina
      multiplicanco la pagina actual por 5*/
    int nElements = (nCurrentSheet*5);
    nCurrentSheet++;/*actualizamos el numero de la pantalla*/
    for(nElements;nElements < sheets.count();nElements++){
        qstrlItem.append(sheets.at(nElements));
        if(nCount == 4)break;
        nCount++;
    }

    /*limpiamos la pantalla*/
    cleanLabels();
    m_nlines = 0; /*empezamos de la primera linea*/

    /*cargamos la informacion en la gui*/
    QStringList qstrl;
    foreach (const QString &qstr, qstrlItem){
        qstrl = qstr.split(";");
        switch(m_nlines){
        case 0:label  ->setText(qstrl.at(0));label_12->setText(qstrl.at(1));label_13->setText(qstrl.at(2));label_14->setText(qstrl.at(3));label_15->setText(qstrl.at(4));break;
        case 1:label_2->setText(qstrl.at(0));label_22->setText(qstrl.at(1));label_23->setText(qstrl.at(2));label_24->setText(qstrl.at(3));label_25->setText(qstrl.at(4));break;
        case 2:label_3->setText(qstrl.at(0));label_32->setText(qstrl.at(1));label_33->setText(qstrl.at(2));label_34->setText(qstrl.at(3));label_35->setText(qstrl.at(4));break;
        case 3:label_4->setText(qstrl.at(0));label_42->setText(qstrl.at(1));label_43->setText(qstrl.at(2));label_44->setText(qstrl.at(3));label_45->setText(qstrl.at(4));break;
        case 4:label_5->setText(qstrl.at(0));label_52->setText(qstrl.at(1));label_53->setText(qstrl.at(2));label_54->setText(qstrl.at(3));label_55->setText(qstrl.at(4));break;
        default:break;
        }
        m_nlines++;
    }
    qstrlItem.clear();
    qApp->processEvents();

    return 0;
}

void PointOfSale::clear()
{
    cleanLabels();
    m_nlines = 0;
    label_total->clear();
    lineEdit->clear();
}
