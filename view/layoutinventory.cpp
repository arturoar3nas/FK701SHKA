#include "layoutinventory.h"

LayoutInventory::LayoutInventory(QWidget *parent) : QWidget(parent)
{
    const int Qtexteditwidth = 770;
    const int Qtexteditheight = 240;
    const int QLineEditwidth = 330;
    const int QlineEditheight = 40;
    /*Label de Titulo*/
    {
        label_title = new QLabel;
        label_title->setFixedWidth(800);
        label_title->setFixedHeight(40);
        label_title->setText(QString::fromUtf8("INVENTARIO"));
        label_title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    /*Label de Espacio*/
    {
        label_espacio = new QLabel;
        label_espacio->setFixedWidth(800);
        label_espacio->setFixedHeight(10);
        label_espacio->setText(" ");
        label_espacio->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    /*Label de alineacion*/
    {
        label_alignRigt = new QLabel;
        label_alignRigt->setFixedWidth(15);
        label_alignRigt->setFixedHeight(15);
        label_alignRigt->setText(" ");
        label_alignRigt->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    /*Label de Ayuda*/
    {
        label_help = new QLabel;
        label_help->setFixedWidth(510);
        label_help->setFixedHeight(70);
        label_help->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label_help->setFrameShadow(QFrame::Plain);
        label_help->setFrameShape(QFrame::Box);
        label_help->setLineWidth(1);
        label_help->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding );
    }
    /*Label's de botones*/
    {
        label_accept = new QLabel;
        label_accept->setFixedWidth(180);
        label_accept->setFixedHeight(32);
        label_accept->setText("TOTAL EFECT");
        label_accept->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label_accept->setFrameShadow(QFrame::Plain);
        label_accept->setFrameShape(QFrame::Box);
        label_accept->setLineWidth(1);
    }
    /*LineEdit's de datos*/
    {
        lineEdit_1 = new QLineEdit;
        lineEdit_1->setFixedWidth(QLineEditwidth);
        lineEdit_1->setFixedHeight(QlineEditheight);
        lineEdit_1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    /*TextEdit de Inventario*/
    {
        textEdit = new QPlainTextEdit;
        //!textEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        textEdit->setFixedWidth(Qtexteditwidth);
        textEdit->setFixedHeight(Qtexteditheight);
        textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    /*Diseño de cuadrícula*/
    {
        layout_inventory = new QGridLayout;
        colunm_1 = new QVBoxLayout;
        row_box_top = new QHBoxLayout;
        row_box_mid = new QHBoxLayout;
        row_box_bot = new QHBoxLayout;
        row_box_space = new QHBoxLayout;

        layout_inventory->addWidget(label_title ,0  ,0 , Qt::AlignHCenter | Qt::AlignVCenter );
        layout_inventory->addLayout(colunm_1 ,1 ,0 , Qt::AlignHCenter | Qt::AlignTop );
        colunm_1->addLayout(row_box_top);
        colunm_1->addLayout(row_box_space);
        colunm_1->addLayout(row_box_mid);
        layout_inventory->addLayout(row_box_bot ,2 ,0 ,Qt::AlignHCenter | Qt::AlignVCenter);
        row_box_space->addWidget(label_espacio);
        row_box_top->addWidget(lineEdit_1,0,Qt::AlignLeft);
        row_box_mid->addWidget(textEdit);
        row_box_bot->addWidget(label_help);
        row_box_bot->addWidget(label_alignRigt);
        row_box_bot->addWidget(label_accept);
    }

    setLayout(layout_inventory);
    resize(800, 480);//550, 370

}

void LayoutInventory::updateWidgets(void *sender)
{
    QStringList *strl = static_cast<QStringList*>(sender);
    QStringList strlTmp;
    int nCount = strl->count();
    for(int i = 0; i < nCount; i++) {
        QString str = strl->at(i);
        if(!str.isEmpty()) {
            strlTmp = str.split(";");
            QString strLine;
            strLine.append("<table width=\"100%\">");
            strLine.append("<tr>");
            strLine.append("<td width=\"150\" >"+strlTmp.at(0)+"</td>");
            strLine.append("<td width=\"150\" >"+strlTmp.at(6)+"</td>");
            strLine.append("<td width=\"140\" >"+strlTmp.at(16)+"</td>");
            strLine.append("<td width=\"140\" >"+strlTmp.at(10)+"</td>");
            strLine.append("<td width=\"140\" >"+strlTmp.takeLast()+"</td>");
            strLine.append("</tr>");
            strLine.append("</table>");
            textEdit->appendHtml(strLine);
        }
    }
    qApp->processEvents();
}

void LayoutInventory::writeOnWidget(QString qstr)
{
    if(lineEdit_1->hasFocus()){
       lineEdit_1->insert(qstr);
    }
}

void LayoutInventory::backSpaceLineEdit()
{
    if(lineEdit_1->hasFocus()){
       lineEdit_1->backspace();
    }
}

int LayoutInventory::getFromWidgets(QString &qstr)
{
    qstr.append(lineEdit_1->text());
    lineEdit_1->clear();
    return 0;
}

int LayoutInventory::getFromWidgets(QStringList &qstrl)
{
    qstrl.append(lineEdit_1->text());
    return 0;
}

int LayoutInventory::focusUp()
{
    /*! []
//    if(lineEdit_1->hasFocus()){
//        lineEdit_2->setFocus();
//        return 0;
//    }
//    if(lineEdit_2->hasFocus()){
//        lineEdit_1->setFocus();
//        return 0;
//    }
*/
    int nPos = textEdit->verticalScrollBar()->value();
    nPos -= 10;
    textEdit->verticalScrollBar()->setSliderPosition(nPos);

}

int LayoutInventory::focusDown()
{
/*! []
//    if(lineEdit_1->hasFocus()){
//        lineEdit_2->setFocus();
//        return 0;
//    }
//    if(lineEdit_2->hasFocus()){
//        lineEdit_1->setFocus();
//        return 0;
//    }
*/
    int nPos = textEdit->verticalScrollBar()->value();
    nPos += 10;
    textEdit->verticalScrollBar()->setSliderPosition(nPos);
}

void LayoutInventory::clear()
{
    lineEdit_1->clear();
    textEdit->clear();
}

int LayoutInventory::isFocus()
{
    if(lineEdit_1->hasFocus()){
        return 0;
    }
    else
        return -1;
}




