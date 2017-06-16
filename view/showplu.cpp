#include "showplu.h"

ShowPlu::ShowPlu(QWidget *parent)
    :QWidget(parent)
{
    QFont f_14("helvetica",14, QFont::Bold);

    textEdit = new QTextEdit;
    textEdit->setFont(f_14);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);

    layout = new QGridLayout;
    layout->addWidget(createLabel(tr("VER PLU")),1 ,0);
    layout->addWidget(line,3,0);
    layout->addWidget(createLabel(tr("")),4 ,0);
    layout->addWidget(textEdit,5,0);
    //layout->addWidget(createLabel(tr("")),6 ,0);
    layout->addWidget(createLabel(tr("Presione [VER PLU] para salir")),7 ,0);
    setLayout(layout);

    resize(800, 480);//550, 370
}

void ShowPlu::updateWidgets(void *sender)
{
    QStringList *qstr = static_cast<QStringList*>(sender);
    textEdit->clear();/*Borramos la Pantalla*/

    QString precio = qstr->at(6);
    int long_precio = precio.length()-2;
    precio.insert(long_precio, QString("."));

    QString iva = qstr->at(7);
    int long_iva = iva.length()-2;
    iva.insert(long_iva, QString("."));

    QString impuesto_interno = qstr->at(8);
    int long_impuesto_interno = impuesto_interno.length()-2;
    impuesto_interno.insert(long_impuesto_interno, QString("."));

    QString qstrOpen;
    if(qstr->at(12).compare("Yes") == 0)
        qstrOpen = "Si";
    else
        qstrOpen = "No";

    QString qstrNeg;
    if(qstr->at(13).compare("Yes") == 0)
        qstrNeg = "Si";
    else
        qstrNeg = "No";

    QString qstrUn;
    if(qstr->at(14).compare("Yes") == 0)
        qstrUn = "Si";
    else
        qstrUn = "No";

    QString text;
    text.append("<table width=\"100%\">");
    text.append("<tr>");
    text.append("<td width=\"200\" >Nombre:</td>");
    text.append("<td width=\"200\" >"+qstr->at(0)+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Precio:</td>");
    text.append("<td width=\"200\" >"+precio+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Stock:</td>");
    text.append("<td width=\"200\" >"+qstr->at(10)+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >IVA:</td>");
    text.append("<td width=\"200\" >"+iva+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Monto impuesto interno:</td>");
    text.append("<td width=\"200\" >"+impuesto_interno+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Tipo:</td>");
    text.append("<td width=\"200\" >"+qstr->at(11)+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Abierto:</td>");
    text.append("<td width=\"200\" >"+qstrOpen+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Negativo:</td>");
    text.append("<td width=\"200\" >"+qstrNeg+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Unico:</td>");
    text.append("<td width=\"200\" >"+qstrUn+"</td>");
    text.append("</tr>");
    text.append("<tr>");
    text.append("<td width=\"200\" >Departamento:</td>");
    text.append("<td width=\"200\" >"+qstr->at(16)+"</td>");
    text.append("</tr>");
    text.append("</table>");
    textEdit->append(text);
    return;
}


void ShowPlu::clear()
{
    textEdit->clear();
}

QLabel *ShowPlu::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
