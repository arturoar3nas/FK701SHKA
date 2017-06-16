#include "index.h"

Index::Index(QWidget *parent)
    :QWidget(parent)
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    QWidget::setStyleSheet("QLabel:focus   { color : #960000; border-color: #960000;}");

    label_1 = new QLabel("Configuracion");
    label_1->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_1->setFrameShadow(QFrame::Plain);
    label_1->setFrameShape(QFrame::Box);
    label_1->setLineWidth(1);
    label_1->setFixedWidth(250);
    label_1->setFixedHeight(50);

    label_2 = new QLabel("Punto de Venta");
    label_2->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_2->setFrameShadow(QFrame::Plain);
    label_2->setFrameShape(QFrame::Box);
    label_2->setLineWidth(1);
    label_2->setFixedWidth(250);
    label_2->setFixedHeight(50);

    label_3 = new QLabel("Modo X");
    label_3->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_3->setFrameShadow(QFrame::Plain);
    label_3->setFrameShape(QFrame::Box);
    label_3->setLineWidth(1);
    label_3->setFixedWidth(250);
    label_3->setFixedHeight(50);

    label_4 = new QLabel("Modo Z");
    label_4->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_4->setFrameShadow(QFrame::Plain);
    label_4->setFrameShape(QFrame::Box);
    label_4->setLineWidth(1);
    label_4->setFixedWidth(250);
    label_4->setFixedHeight(50);

    label_5 = new QLabel;
    label_5->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_5->setFrameShadow(QFrame::Plain);
    label_5->setFrameShape(QFrame::Box);
    label_5->setLineWidth(1);
    label_5->setFixedWidth(250);
    label_5->setFixedHeight(50);
    label_5->hide();

    label_6 = new QLabel;
    label_6->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_6->setFrameShadow(QFrame::Plain);
    label_6->setFrameShape(QFrame::Box);
    label_6->setLineWidth(1);
    label_6->setFixedWidth(250);
    label_6->setFixedHeight(50);
    label_6->hide();

    label_7 = new QLabel;
    label_7->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    label_7->setFrameShadow(QFrame::Plain);
    label_7->setFrameShape(QFrame::Box);
    label_7->setLineWidth(1);
    label_7->setFixedWidth(250);
    label_7->setFixedHeight(50);
    label_7->hide();

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("INICIO")),0 ,0);
    layout->addWidget(line,1,0);
    layout->addWidget(createLabel(tr("")),2 ,0,Qt::AlignHCenter);
    layout->addWidget(label_1,3 ,0,Qt::AlignHCenter);
    layout->addWidget(label_2,4 ,0,Qt::AlignHCenter);
    layout->addWidget(label_3,5,0,Qt::AlignHCenter);
    layout->addWidget(label_4,6 ,0,Qt::AlignHCenter);
    layout->addWidget(label_5,7 ,0,Qt::AlignHCenter);
    layout->addWidget(label_6,8 ,0,Qt::AlignHCenter);
    layout->addWidget(label_7,9 ,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("")),10 ,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),11 ,0);
    setLayout(layout);

    resize(800, 480);//550, 370
}

int Index::focusUp()
{
    if(label_1->hasFocus()){
        if(label_5->isVisible())
            label_5->setFocus();
        else if(label_6->isVisible())
            label_6->setFocus();
        else if(label_7->isVisible())
            label_7->setFocus();
        else
            label_4->setFocus();
        return 0;
    }
    else if(label_2->hasFocus()){
        label_1->setFocus();
        return 0;
    }
    else if(label_3->hasFocus()){
        label_2->setFocus();
        return 0;
    }
    else if(label_4->hasFocus()){
        label_3->setFocus();
        return 0;
    }
    else if(label_5->hasFocus()){
        label_4->setFocus();
        return 0;
    }
    else if(label_6->hasFocus()){
        label_5->setFocus();
        return 0;
    }
    else if(label_7->hasFocus()){
        label_6->setFocus();
        return 0;
    }
    else
    {
        label_1->setFocus();
        return -1;
    }
}

int Index::focusDown()
{
    if(label_1->hasFocus()){
        label_2->setFocus();
        return 0;
    }
    else if(label_2->hasFocus()){
        label_3->setFocus();
        return 0;
    }
    else if(label_3->hasFocus()){
        label_4->setFocus();
        return 0;
    }
    else if(label_4->hasFocus()){
        if(label_5->isVisible()){
            label_5->setFocus();
            return 0;
        }
        else if(label_6->isVisible()){
            label_6->setFocus();
            return 0;
        }
        else if(label_7->isVisible()){
            label_7->setFocus();
            return 0;
        }
        else
            label_1->setFocus();
        return 0;
    }
    else if(label_5->hasFocus()){
        if(label_6->isVisible()){
            label_6->setFocus();
            return 0;
        }
        else if(label_7->isVisible()){
            label_7->setFocus();
            return 0;
        }
        else
            label_1->setFocus();
        return 0;
    }
    else if(label_6->hasFocus()){
        if(label_7->isVisible()){
            label_7->setFocus();
            return 0;
        }
        else
            label_1->setFocus();
        return 0;
    }
    else if(label_7->hasFocus()){
        label_1->setFocus();
        return 0;
    }
    else
        return -1;
}

int Index::isFocus()
{
    if(label_1->hasFocus())
        return 0;
    else if(label_2->hasFocus())
        return 1;
    else if(label_3->hasFocus())
        return 2;
    else if(label_4->hasFocus())
        return 3;
    else if(label_5->hasFocus())
        return 4;
    else if(label_6->hasFocus())
        return 5;
    else if(label_7->hasFocus())
        return 6;
    else
        return -1;
}

int Index::modifyLayout(int nOption)
{
    if(nOption == OPTION_LAYOUT_HOME){
        setTexttoLabel("Configuracion",1);
        setTexttoLabel("Punto de Venta",2);
        setTexttoLabel("Modo X",3);
        setTexttoLabel("Modo Z",4);
        label_5->hide();
        label_6->hide();
        label_7->hide();
        label_1->setFocus();/*! reset Focus*/
        return 0;
    }
    else if(nOption == OPTION_LAYOUT_X_MODE){
        setTexttoLabel("Cierre X",1);
        setTexttoLabel("Reporte X1",2);
        setTexttoLabel("Declaracion de Caja",3);
        setTexttoLabel("Inventario",4);
        setTexttoLabel("Garage",5);
        setTexttoLabel("Borrar Garage",6);
        setTexttoLabel("Libro de Venta",7);
        label_1->setFocus();/*! reset Focus*/
        label_5->show();
        label_6->show();
        label_7->show();
        return 0;
    }
    else if(nOption == OPTION_LAYOUT_Z_MODE){
        setTexttoLabel("Cierre Z",1);
        setTexttoLabel("Reporte Z1",2);
        setTexttoLabel("Reporte Z2",3);
        setTexttoLabel("a",4);
        setTexttoLabel("b",5);
        setTexttoLabel("c",6);
        setTexttoLabel("d",7);
        label_1->setFocus();/*! reset Focus*/
        label_5->show();
        label_6->show();
        label_7->show();
        return 0;
    }
    else if(nOption == OPTION_LAYOUT_X1){
        setTexttoLabel("Departamento",1);
        setTexttoLabel("Cajero",2);
        setTexttoLabel("Mozo",3);
        setTexttoLabel("PLU",4);
        setTexttoLabel("Venta Periodo",5);
        label_1->setFocus();/*! reset Focus*/
        label_5->show();
        label_6->hide();
        label_7->hide();
        return 0;
    }
    else if(nOption == OPTION_LAYOUT_Z1){
        setTexttoLabel("Departamento",1);
        setTexttoLabel("Cajero",2);
        setTexttoLabel("Mozo",3);
        setTexttoLabel("PLU",4);
        setTexttoLabel("Venta Periodo",5);
        label_1->setFocus();/*! reset Focus*/
        label_5->show();
        label_6->hide();
        label_7->hide();
        return 0;
    }
    else if(nOption == OPTION_LAYOUT_Z2){
        setTexttoLabel("Financiero",1);
        setTexttoLabel("Departamento",2);
        setTexttoLabel("Cajero",3);
        setTexttoLabel("Mozo",4);
        setTexttoLabel("PLU",5);
        setTexttoLabel("Venta Periodo",6);
        label_1->setFocus();/*! reset Focus*/
        label_5->show();
        label_6->show();
        label_7->hide();
        return 0;
    }
    else
        return -1;
}

void Index::setTexttoLabel(QString qstr, int nLabel) const
{
    switch(nLabel){
    case 1: label_1->setText(qstr);break;
    case 2: label_2->setText(qstr);break;
    case 3: label_3->setText(qstr);break;
    case 4: label_4->setText(qstr);break;
    case 5: label_5->setText(qstr);break;
    case 6: label_6->setText(qstr);break;
    case 7: label_7->setText(qstr);break;
    default:break;
    }
}

QLabel *Index::createLabelwithFrame(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    lbl->setFrameShadow(QFrame::Plain);
    lbl->setFrameShape(QFrame::Box);
    lbl->setLineWidth(1);
    lbl->setFixedWidth(250);
    lbl->setFixedHeight(50);
    return lbl;
}
QLabel *Index::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}
