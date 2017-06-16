#include "inputdialog.h"

InputDialog::InputDialog(QWidget *parent)
    :QWidget(parent)
{
    lineEditUsr = new QLineEdit;
    lineEditPswd = new QLineEdit;

    lineEditPswd->setEchoMode(QLineEdit::Password);

    lineEditPswd->setMaxLength(4);
    lineEditUsr->setMaxLength(1);

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("Llame a un Supervisor!")),0 ,0);
    layout->addWidget(createLabel(tr("Numero de Usuario")),1 ,0);
    layout->addWidget(lineEditUsr,2,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Contrasena")),3 ,0);
    layout->addWidget(lineEditPswd,4,0,Qt::AlignHCenter);
    setLayout(layout);

    resize(200, 200);//550, 370
    /*center*/
    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    /*change color*/
    QPalette p(this->palette());
    p.setColor(QPalette::Window, Qt::gray);
    setPalette(p);
}

QLabel *InputDialog::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}

void InputDialog::writeOnWidget(QString qstr)
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

void InputDialog::backSpaceLineEdit()
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

void InputDialog::clear()
{
    lineEditPswd->clear();
    lineEditUsr->clear();

}
int InputDialog::getFromWidgets(QString &qstr)
{
    qstr.append(lineEditUsr->text()+";");
    qstr.append(lineEditPswd->text()+";");
    return 0;
}
int InputDialog::getFromWidgets(QStringList &qstrl)
{
    qstrl.append(lineEditUsr->text());
    qstrl.append(lineEditPswd->text());
    return 0;
}

int InputDialog::focusUp()
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

int InputDialog::focusDown()
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
