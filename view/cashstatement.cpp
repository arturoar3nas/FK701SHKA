#include "cashstatement.h"

CashStatement::CashStatement(QWidget *parent)
    :QWidget(parent)
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    label_cards = new QLabel;
    label_cash = new QLabel;
    label_cheq = new QLabel;
    label_total = new QLabel;
    lineEdit = new QLineEdit;

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("")),0 ,0);
    layout->addWidget(createLabel(tr("DECLARACION DE CAJA")),1 ,0 ,1 ,2);
    layout->addWidget(line,2,0,1,2);
    layout->addWidget(createLabel(tr("")),3 ,0);
    layout->addWidget(createLabelwithFrame(tr("EFECTIVO")),4 ,0);
    layout->addWidget(label_cash,4 ,1);
    layout->addWidget(createLabelwithFrame(tr("TARJETAS")),5 ,0);
    layout->addWidget(label_cards,5 ,1);
    layout->addWidget(createLabelwithFrame(tr("CHEQUE")),6 ,0);
    layout->addWidget(label_cheq,6 ,1);
    layout->addWidget(createLabelwithFrame(tr("TOTAL")),7 ,0);
    layout->addWidget(label_total,7 ,1);
    layout->addWidget(lineEdit,8 ,0);
    layout->addWidget(createLabel(tr("")),9 ,0);
    layout->addWidget(createLabel(tr("Presione [TOTALEFECT] para continuar")),10 ,0,1,2);//8
    setLayout(layout);

    resize(800, 480);//550, 370
}

void CashStatement::updateWidgets(void *sender)
{
    QStringList *strl = static_cast<QStringList*>(sender);
    label_cash->setText(strl->at(0).toAscii());
    label_cards->setText(strl->at(1).toAscii());
    label_cheq->setText(strl->at(2).toAscii());
    label_total->setText(strl->at(3).toAscii());
}

void CashStatement::writeOnWidget(QString qstr)
{
    lineEdit->insert(qstr);
}

void CashStatement::backSpaceLineEdit()
{
    lineEdit->backspace();
}

void CashStatement::clear()
{
    lineEdit->clear();
}

QLabel *CashStatement::createLabelwithFrame(const QString &text)
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

QLabel *CashStatement::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    return lbl;
}
