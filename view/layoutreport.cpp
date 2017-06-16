#include "layoutreport.h"

LayoutReport::LayoutReport(QWidget *parent) : QWidget(parent)
{
    /*Label de Titulo*/
    {
        label_title = new QLabel("Reportes");
        label_title->setFixedWidth(800);
        label_title->setFixedHeight(40);
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
    int Qtexteditwidth = 770;
    int Qtexteditheight = 370;
    /*TextEdit de Inventario*/
    {
        textEdit = new QPlainTextEdit;
        textEdit->setFixedWidth(Qtexteditwidth);
        textEdit->setFixedHeight(Qtexteditheight);
        textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    /*Diseño de cuadrícula*/
    {
        layout_report = new QGridLayout;
        row_box_mid = new QHBoxLayout;
        row_box_bot = new QHBoxLayout;

        layout_report->addWidget(label_title ,0 ,0 , Qt::AlignHCenter | Qt::AlignVCenter );
        layout_report->addLayout(row_box_mid ,1 ,0  , Qt::AlignHCenter | Qt::AlignVCenter );
        row_box_mid->addWidget(textEdit);

        layout_report->addLayout(row_box_bot ,2 ,0  ,Qt::AlignHCenter | Qt::AlignVCenter);
        row_box_bot->addWidget(label_accept);

    }
    setLayout(layout_report);
    resize(800, 480);//550, 370

    label_accept->setFocus();
}

void LayoutReport::updateWidgets(void *sender)
{
    QStringList *strl = static_cast<QStringList*>(sender);
    qDebug () << "update";
    textEdit->appendPlainText(strl->join("\n"));
    qApp->processEvents();
}

int LayoutReport::focusUp()
{
    int nPos = textEdit->verticalScrollBar()->value();
    nPos -= 30;
    textEdit->verticalScrollBar()->setSliderPosition(nPos);
}

int LayoutReport::focusDown()
{
    int nPos = textEdit->verticalScrollBar()->value();
    nPos += 30;
    textEdit->verticalScrollBar()->setSliderPosition(nPos);
}



