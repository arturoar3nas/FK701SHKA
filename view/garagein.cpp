#include "garagein.h"

GarageIn::GarageIn(QWidget *parent)
    :QWidget(parent)
{
    int nWidth = 350;

    lineEditCategoria = new QLineEdit;
    lineEditnPatente = new QLineEdit;
    lineEdituPrecio = new QLineEdit;
    lineEditdUnidad = new QLineEdit;

    lineEditCategoria->setMaxLength(1);
    lineEditnPatente->setMaxLength(20);
    lineEdituPrecio->setMaxLength(13);
    lineEditdUnidad->setMaxLength(1);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    lineEditCategoria->setFixedWidth(nWidth);
    lineEditnPatente->setFixedWidth(nWidth);
    lineEdituPrecio->setFixedWidth(nWidth);
    lineEditdUnidad->setFixedWidth(nWidth);

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("")),0 ,0);
    layout->addWidget(createLabel(tr("Garage Entrada")),1 ,0 ,1 ,2);
    layout->addWidget(line,2,0,1,2);
    layout->addWidget(createLabel(tr("")),3 ,0);
    layout->addWidget(createLabel(tr("CATEGORIA")),6 ,0);
    layout->addWidget(lineEditCategoria,7 ,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("CANTIDAD DE UNIDAD DE PRECIO  ")),6 ,1);
    layout->addWidget(lineEdituPrecio,7 ,1,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("NUMERO DE PATENTE")),8 ,0);
    layout->addWidget(lineEditnPatente,9,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("DIGITO CORRESPONDIENTE A LA UNIDAD")),8 ,1);
    layout->addWidget(lineEditdUnidad,9,1,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("")),10 ,0);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),11 ,0,1,2);//8
    setLayout(layout);

    resize(800, 480);//550, 370
}

QLabel *GarageIn::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}


void GarageIn::writeOnWidget(QString qstr)
{
    if(lineEditCategoria->hasFocus())
        lineEditCategoria->insert(qstr);
    else if(lineEdituPrecio->hasFocus())
        lineEdituPrecio->insert(qstr);
    else if(lineEditnPatente->hasFocus())
        lineEditnPatente->insert(qstr);
    else if(lineEditdUnidad->hasFocus())
        lineEditdUnidad->insert(qstr);
    else
        return;
}

void GarageIn::backSpaceLineEdit()
{
    if(lineEditCategoria->hasFocus())
        lineEditCategoria->backspace();
    else if(lineEdituPrecio->hasFocus())
        lineEdituPrecio->backspace();
    else if(lineEditnPatente->hasFocus())
        lineEditnPatente->backspace();
    else if(lineEditdUnidad->hasFocus())
        lineEditdUnidad->backspace();
    else
        return;
}

int GarageIn::getFromWidgets(QStringList &qstrl)
{
    qstrl.append(lineEditCategoria->text());
    qstrl.append(lineEdituPrecio->text());
    qstrl.append(lineEditnPatente->text());
    qstrl.append(lineEditdUnidad->text());
    return 0;
}

int GarageIn::focusUp()
{
    if(lineEditCategoria->hasFocus())
        lineEditdUnidad->setFocus();
    else if(lineEdituPrecio->hasFocus())
        lineEditCategoria->setFocus();
    else if(lineEditnPatente->hasFocus())
        lineEdituPrecio->setFocus();
    else if(lineEditdUnidad->hasFocus())
        lineEditnPatente->setFocus();
    else
        return -1;
    return 0;
}

int GarageIn::focusDown()
{
    if(lineEditCategoria->hasFocus())
        lineEdituPrecio->setFocus();
    else if(lineEdituPrecio->hasFocus())
        lineEditdUnidad->setFocus();
    else if(lineEditdUnidad->hasFocus())
        lineEditnPatente->setFocus();
    else if(lineEditnPatente->hasFocus())
        lineEditCategoria->setFocus();
    else
        return -1;
    return 0;
}

void GarageIn::clear()
{
    lineEditCategoria->clear();
    lineEdituPrecio->clear();
    lineEditnPatente->clear();
    lineEditdUnidad->clear();
}
