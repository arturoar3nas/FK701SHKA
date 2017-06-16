#ifndef VIRTUALBOARD_H
#define VIRTUALBOARD_H

#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QPushButton>
#include "view/IGui.h"
#include "cntrlr/fk701sapp.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class virtualBoard : public QWidget, public IGui
{
    Q_OBJECT
public:
    virtualBoard(QWidget *parent = 0);
private:
    Fk701sApp *fk;
    bool bShift;
    QGridLayout *layout;
    QPushButton *button_total;
    QPushButton *button_one;
    QPushButton *button_two;
    QPushButton *button_three;
    QPushButton *button_four;
    QPushButton *button_five;
    QPushButton *button_six;
    QPushButton *button_seven;
    QPushButton *button_eight;
    QPushButton *button_nine;
    QPushButton *button_zero;
    QPushButton *button_client;
    QPushButton *button_price;
    QPushButton *button_showPlu;
    QPushButton *button_delete;
    QPushButton *button_arrowUp;
    QPushButton *button_arrowDown;
    QPushButton *button_cashier;
    QPushButton *button_drawer;
    QPushButton *button_index;
    QPushButton *button_x;
    QPushButton *button_point;
    QPushButton *button_shift;
    QPushButton *button_surcharge;
    QPushButton *button_discount;
    QPushButton *button_PLU;
    QPushButton *button_Dept_one;
    QPushButton *button_cash;
    QPushButton *button_cheq;
    QPushButton *button_debit;
    QPushButton *button_credit;
    QPushButton *button_anul;
    QPushButton *button_correc;
    QPushButton *button_susp;
    QPushButton *button_dept;
    QPushButton *button_esc;

public:
    void updateWidgets(void *sender){Q_UNUSED(sender);}
    void writeOnWidget(QString qstr){Q_UNUSED(qstr);}
    void backSpaceLineEdit(){}
    int getFromWidgets(QString &qstr){Q_UNUSED(qstr);return 0;}
    int getFromWidgets(QStringList &qstrl){Q_UNUSED(qstrl);return 0;}
    int focusUp(){return 0;}
    int focusDown(){return 0;}
    void clear(){}
    void release(){delete this;}
    void showWidget(){show();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}

signals:
    void signal(char*);
    void signaldept(QString);
    void signalDrawer(int);

public slots:
private slots:
    void qwerty();
    void deptDirect();
    void shift();
    void drawer();
};

#endif // VIRTUALBOARD_H
