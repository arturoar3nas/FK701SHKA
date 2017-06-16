#include "logusr.h"

LogUsr::LogUsr(QWidget *parent)
    :QWidget(parent)
{
    int nWidth = 250;
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    lineEditUsr = new QLineEdit;
    lineEditPswd = new QLineEdit;

    lineEditUsr->setFixedWidth(nWidth);
    lineEditPswd->setFixedWidth(nWidth);

    lineEditPswd->setEchoMode(QLineEdit::Password);

    lineEditPswd->setMaxLength(4);
    lineEditUsr->setMaxLength(1);

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("BIENVENIDO")),0 ,0);
    layout->addWidget(line,1,0);
    layout->addWidget(createLabel(tr("Ingrese el numero de Usuario y Contrasena")),2 ,0);
    layout->addWidget(createLabel(tr("Numero de Usuario")),3 ,0);
    layout->addWidget(lineEditUsr,4,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Contrasena")),5 ,0);
    layout->addWidget(lineEditPswd,6,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),7 ,0);
    setLayout(layout);

    resize(800, 480);//550, 370

#ifdef DEBUG
    lineEditUsr->setText("1");
    lineEditPswd->setText("0000");
#endif

}

QLabel *LogUsr::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}

void LogUsr::writeOnWidget(QString qstr)
{
    if(lineEditPswd->hasFocus()){
        lineEditPswd->insert(qstr);
        return;
    }
    if(lineEditUsr->hasFocus()){
        lineEditUsr->insert(qstr);
        return;
    }
}

void LogUsr::backSpaceLineEdit()
{
    if(lineEditPswd->hasFocus()){
        lineEditPswd->backspace();
        return;
    }
    if(lineEditUsr->hasFocus()){
        lineEditUsr->backspace();
        return;
    }
}

void LogUsr::clear()
{
    lineEditPswd->clear();
    lineEditUsr->clear();

}
int LogUsr::getFromWidgets(QString &qstr)
{
    qstr.append(lineEditUsr->text()+";");
    qstr.append(lineEditPswd->text()+";");
    return 0;
}
int LogUsr::getFromWidgets(QStringList &qstrl)
{
    qstrl.append(lineEditUsr->text());
    qstrl.append(lineEditPswd->text());
    return 0;
}

int LogUsr::focusUp()
{
    if(lineEditPswd->hasFocus()){
        lineEditUsr->setFocus();
        return 0;
    }
    if(lineEditUsr->hasFocus()){
        lineEditPswd->setFocus();
        return 0;
    }
}

int LogUsr::focusDown()
{
    if(lineEditPswd->hasFocus()){
        lineEditUsr->setFocus();
        return 0;
    }
    if(lineEditUsr->hasFocus()){
        lineEditPswd->setFocus();
        return 0;
    }
}









