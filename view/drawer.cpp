#include "drawer.h"

Drawer::Drawer(QWidget *parent)
    :QWidget(parent)
{
    int nWidth = 250;

    label = new QLabel;
    lineEdit = new QLineEdit;

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    lineEdit->setFixedWidth(nWidth);

    label->setText("0");
    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("FONDO & RETIRO")),0 ,0);
    layout->addWidget(line,1,0);
    layout->addWidget(createLabel(tr("")),2 ,0);
    layout->addWidget(createLabel(tr("Total en Gaveta")),3 ,0);
    layout->addWidget(label,4 ,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Ingrese una cantidad")),5 ,0);
    layout->addWidget(lineEdit,6,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("")),7 ,0);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),8 ,0);
    setLayout(layout);
    resize(800, 480);//550, 370
}

QLabel *Drawer::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}

int Drawer::getFromWidgets(QString &qstr)
{
    qstr = lineEdit->text();
    lineEdit->clear();
    return 0;
}

void Drawer::clear()
{
    label->clear();
    lineEdit->clear();
}


void Drawer::updateWidgets(void *sender)
{
    QString *qstr = static_cast<QString*>(sender);
    label->setText(qstr->toAscii());
}

void Drawer::writeOnWidget(QString qstr)
{
    lineEdit->insert(qstr);
}

void Drawer::backSpaceLineEdit()
{
    lineEdit->backspace();
}

