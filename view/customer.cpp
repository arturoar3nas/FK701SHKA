#include "customer.h"

Customer::Customer(QWidget *parent)
    :QWidget(parent)
{
    int nWidth = 350;

    lineEditId = new QLineEdit;
    lineEditRSocial = new QLineEdit;
    lineEditnDocument = new QLineEdit;
    lineEditResp = new QLineEdit;
    textEditAdress = new QTextEdit;

    lineEditId->setMaxLength(1);
    lineEditRSocial->setMaxLength(20);
    lineEditnDocument->setMaxLength(11);
    lineEditResp->setMaxLength(1);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    lineEditId->setFixedWidth(nWidth);
    lineEditRSocial->setFixedWidth(nWidth);
    lineEditnDocument->setFixedWidth(nWidth);
    lineEditResp->setFixedWidth(nWidth);
    textEditAdress->setFixedWidth(400);

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("")),0 ,0);
    layout->addWidget(createLabel(tr("REGISTRO DE CLIENTE")),1 ,0 ,1 ,2);
    layout->addWidget(line,2,0,1,2);
    layout->addWidget(createLabel(tr("")),3 ,0);
    layout->addWidget(createLabel(tr("Tipo de ID")),6 ,0);
    layout->addWidget(lineEditId,7 ,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Numero de Documento")),6 ,1);
    layout->addWidget(lineEditnDocument,7 ,1,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Razon Social")),8 ,0);
    layout->addWidget(lineEditRSocial,9,0,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Responsabilidad")),8 ,1);
    layout->addWidget(lineEditResp,9,1,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("Direccion")),11 ,1);
    layout->addWidget(textEditAdress,12,1,Qt::AlignHCenter);
    layout->addWidget(createLabel(tr("")),13 ,0);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),14 ,0,1,2);//8
    setLayout(layout);

#ifdef DEBUG
    lineEditId->setText("T");
    lineEditnDocument->setText("27257688122");
    lineEditResp->setText("I");
    lineEditRSocial->setText("ISA");
    textEditAdress->setText("ccs");
#endif
    resize(800, 480);//550, 370
}

void Customer::writeOnWidget(QString qstr)
{
    if(lineEditId->hasFocus())
        lineEditId->insert(qstr);
    else if(lineEditnDocument->hasFocus())
        lineEditnDocument->insert(qstr);
    else if(lineEditResp->hasFocus())
        lineEditResp->insert(qstr);
    else if(lineEditRSocial->hasFocus())
        lineEditRSocial->insert(qstr);
    else if(textEditAdress->hasFocus())
        textEditAdress->insertPlainText(qstr);
    else
        return;
}

void Customer::backSpaceLineEdit()
{
    if(lineEditId->hasFocus())
        lineEditId->backspace();
    else if(lineEditnDocument->hasFocus())
        lineEditnDocument->backspace();
    else if(lineEditResp->hasFocus())
        lineEditResp->backspace();
    else if(lineEditRSocial->hasFocus())
        lineEditRSocial->backspace();
    else if(textEditAdress->hasFocus())
        textEditAdress->undo();
    else
        return;
}

QLabel *Customer::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}

int Customer::getFromWidgets(QString &qstr)
{
    qstr.append(lineEditId->text()+";");
    qstr.append(lineEditnDocument->text()+";");
    qstr.append(lineEditResp->text()+";");
    qstr.append(lineEditRSocial->text()+";");
    qstr.append(textEditAdress->toPlainText()+";");
    return 0;
}
int Customer::getFromWidgets(QStringList &qstrl)
{
    qstrl.append(lineEditId->text());
    qstrl.append(lineEditnDocument->text());
    qstrl.append(lineEditResp->text());
    qstrl.append(lineEditRSocial->text());
    qstrl.append(textEditAdress->toPlainText());
    return 0;
}

int Customer::focusUp()
{
    if(lineEditId->hasFocus())
        textEditAdress->setFocus();
    else if(lineEditnDocument->hasFocus())
        lineEditId->setFocus();
    else if(lineEditResp->hasFocus())
        lineEditnDocument->setFocus();
    else if(lineEditRSocial->hasFocus())
        lineEditResp->setFocus();
    else if(textEditAdress->hasFocus())
        lineEditRSocial->setFocus();
    else
        return -1;
}

int Customer::focusDown()
{
    if(lineEditId->hasFocus())
        lineEditnDocument->setFocus();
    else if(lineEditnDocument->hasFocus())
        lineEditRSocial->setFocus();
    else if(lineEditRSocial->hasFocus())
        lineEditResp->setFocus();
    else if(lineEditResp->hasFocus())
        textEditAdress->setFocus();
    else if(textEditAdress->hasFocus())
        lineEditId->setFocus();
    else
        return -1;
}

void Customer::clear()
{
    lineEditId->clear();
    lineEditnDocument->clear();
    lineEditResp->clear();
    lineEditRSocial->clear();
    textEditAdress->clear();
}
