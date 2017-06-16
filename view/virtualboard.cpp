#include "virtualboard.h"

virtualBoard::virtualBoard(QWidget *parent)
    :QWidget(parent)
{
    fk = new Fk701sApp();
        fk->start();

        bShift = false;

        button_total = new QPushButton("Total Efect");
        button_total->setStyleSheet("background-color: green");
        button_one = new QPushButton("1");
        button_one->setStyleSheet("color: white; background-color: black");
        button_two = new QPushButton("2");
        button_two->setStyleSheet("color: white; background-color: black");
        button_three = new QPushButton("3");
        button_three->setStyleSheet("color: white; background-color: black");
        button_four = new QPushButton("4");
        button_four->setStyleSheet("color: white; background-color: black");
        button_five = new QPushButton("5");
        button_five->setStyleSheet("color: white; background-color: black");
        button_six = new QPushButton("6");
        button_six->setStyleSheet("color: white; background-color: black");
        button_seven = new QPushButton("7");
        button_seven->setStyleSheet("color: white; background-color: black");
        button_eight = new QPushButton("8");
        button_eight->setStyleSheet("color: white; background-color: black");
        button_nine = new QPushButton("9");
        button_nine->setStyleSheet("color: white; background-color: black");
        button_zero = new QPushButton("0");
        button_zero->setStyleSheet("color: white; background-color: black");
        button_delete = new QPushButton("Borrar C");
        button_delete->setStyleSheet("background-color: red");
        button_point = new QPushButton(".");
        button_point->setStyleSheet("color: white; background-color: black");
        button_client = new QPushButton("Cliente");
        button_price = new QPushButton("Price");
        button_showPlu = new QPushButton("Show");
        button_arrowUp = new QPushButton("^");
        button_arrowUp->setStyleSheet("background-color: yellow");
        button_arrowDown = new QPushButton("v");
        button_arrowDown->setStyleSheet("background-color: yellow");
        button_cashier = new QPushButton("Cajero");
        button_drawer = new QPushButton("Cajon");
        button_index = new QPushButton("Home");
        button_index->setStyleSheet("background-color: yellow");
        button_x = new QPushButton("X");
        button_shift = new QPushButton("Shift");
        button_surcharge = new QPushButton("Rec");
        button_discount = new QPushButton("Desc");
        button_PLU = new QPushButton("Plu");
        button_Dept_one = new QPushButton("Dept# 1");
        button_cash = new QPushButton("Cash");
        button_debit = new QPushButton("Debit");
        button_cheq = new QPushButton("Cheq");
        button_credit = new QPushButton("Credit");
        button_anul = new QPushButton("Anul");
        button_anul->setStyleSheet("background-color: red");
        button_correc = new QPushButton("Correc");
        button_correc->setStyleSheet("background-color: red");
        button_susp = new QPushButton("Susp");
        button_susp->setStyleSheet("background-color: green");
        button_dept = new QPushButton("Dept#");
        button_esc = new QPushButton("Esc");
        button_susp->setStyleSheet("background-color: green");

        layout = new QGridLayout;
        layout->addWidget(button_anul       ,0 ,1);
        layout->addWidget(button_price      ,0 ,2);
        layout->addWidget(button_showPlu    ,0 ,3);
        layout->addWidget(button_client     ,0 ,4);
        layout->addWidget(button_arrowUp    ,0 ,5);
        layout->addWidget(button_arrowDown  ,0 ,6);

        layout->addWidget(button_correc     ,1 ,1);
        layout->addWidget(button_x          ,1 ,2);
        layout->addWidget(button_PLU        ,1 ,3);
        layout->addWidget(button_dept       ,1 ,4);
        layout->addWidget(button_shift      ,1 ,5);
        layout->addWidget(button_cashier    ,1 ,6);

        layout->addWidget(button_surcharge  ,2 ,1);
        layout->addWidget(button_seven      ,2 ,2);
        layout->addWidget(button_eight      ,2 ,3);
        layout->addWidget(button_nine       ,2 ,4);
        layout->addWidget(button_cash       ,2 ,5);
        layout->addWidget(button_debit      ,2 ,6);

        layout->addWidget(button_discount   ,3 ,1);
        layout->addWidget(button_four       ,3 ,2);
        layout->addWidget(button_five       ,3 ,3);
        layout->addWidget(button_six        ,3 ,4);
        layout->addWidget(button_credit     ,3 ,5);
        layout->addWidget(button_cheq       ,3 ,6);

        layout->addWidget(button_drawer     ,4 ,1);
        layout->addWidget(button_one        ,4 ,2);
        layout->addWidget(button_two        ,4 ,3);
        layout->addWidget(button_three      ,4 ,4);
        layout->addWidget(button_Dept_one   ,4 ,5);
        layout->addWidget(button_susp       ,4 ,6);

        layout->addWidget(button_index      ,5 ,1);
        layout->addWidget(button_delete     ,5 ,2);
        layout->addWidget(button_zero       ,5 ,3);
        layout->addWidget(button_point      ,5 ,4);
        layout->addWidget(button_total      ,5 ,5,1,6);

        layout->addWidget(button_esc      ,6 ,1);

        setLayout(layout);

        connect(button_total,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Total()));
        connect(button_client,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Client()));
        connect(button_price,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Price()));
        connect(button_showPlu,SIGNAL(clicked(bool)),fk,SLOT(pressKey_ShowPlu()));
        connect(button_arrowUp,SIGNAL(clicked(bool)),fk,SLOT(pressKey_FocusUp()));
        connect(button_arrowDown,SIGNAL(clicked(bool)),fk,SLOT(pressKey_FocusDown()));
        connect(button_cashier,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Cashier()));
        connect(button_one,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_two,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_three,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_four,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_five,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_six,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_seven,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_eight,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_nine,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_zero,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(button_point,SIGNAL(clicked(bool)),this,SLOT(qwerty()));
        connect(this,SIGNAL(signal(char*)),fk,SLOT(pressKey_Qwerty(char*)));
        connect(button_PLU,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Plu()));
        connect(button_Dept_one,SIGNAL(clicked(bool)),this,SLOT(deptDirect()));
        connect(this,SIGNAL(signaldept(QString)),fk,SLOT(pressKey_DirectDept(QString)));
        connect(button_index,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Home()));
        connect(button_discount,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Discount()));
        connect(button_cash,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Cash()));
        connect(button_debit,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Debit()));
        connect(button_cheq,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Check()));
        connect(button_credit,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Credit()));
        connect(button_surcharge,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Surcharge()));
        connect(button_anul,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Anul()));
        connect(button_correc,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Correc()));
        connect(button_shift,SIGNAL(clicked(bool)),this,SLOT(shift()));
        connect(button_drawer,SIGNAL(clicked(bool)),this,SLOT(drawer()));
        connect(this,SIGNAL(signalDrawer(int)),fk,SLOT(pressKey_Drawer(int)));
        connect(button_susp,SIGNAL(clicked(bool)),fk,SLOT(pressKey_suspend()));
        connect(button_dept,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Dept()));
        connect(button_esc,SIGNAL(clicked(bool)),fk,SLOT(pressKey_Esc()));


}

void virtualBoard::qwerty()
{
    QObject* callingButton = QObject::sender();
    if (callingButton == button_one){
        emit signal((char*)"1");return;}
    else if (callingButton == button_two){
        emit signal((char*)"2");return;}
    else if (callingButton == button_three){
        emit signal((char*)"3");return;}
    else if (callingButton == button_four){
        emit signal((char*)"4");return;}
    else if (callingButton == button_five){
        emit signal((char*)"5");return;}
    else if (callingButton == button_six){
        emit signal((char*)"6");return;}
    else if (callingButton == button_seven){
        emit signal((char*)"7");return;}
    else if (callingButton == button_eight){
        emit signal((char*)"8");return;}
    else if (callingButton == button_nine){
        emit signal((char*)"9");return;}
    else if (callingButton == button_zero){
        emit signal((char*)"0");return;}
    else if (callingButton == button_point){
        emit signal((char*)".");return;}
    else
        return;
}

void virtualBoard::deptDirect()
{
    QObject* callingButton = QObject::sender();
    if (callingButton == button_Dept_one)
        emit signaldept("1");
    else
        return;
}

void virtualBoard::shift()
{
    bShift=!bShift;
}

void virtualBoard::drawer()
{
    if(!bShift){
        emit signalDrawer(1);
        return;
    }
    if(bShift){
        emit signalDrawer(0);
        return;
    }
}
