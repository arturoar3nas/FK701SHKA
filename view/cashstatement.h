#ifndef CASHSTATEMENT_H
#define CASHSTATEMENT_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QString>
#include <QtGui>
#include "view/IGui.h"
#include "util.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class CashStatement : public QWidget , public IGui
{
    Q_OBJECT
public:
    CashStatement(QWidget *parent = 0);
    void updateWidgets(void *sender);
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr){return 0;}
    int getFromWidgets(QStringList &qstrl){return 0;}
    int focusUp(){return 0;}
    int focusDown(){return 0;}
    void clear();
    void release(){delete this;}
    void showWidget(){lineEdit->setFocus();show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}

private:
    QLineEdit *lineEdit;
    QLabel    *label_cash;
    QLabel    *label_cards;
    QLabel    *label_cheq;
    QLabel    *label_total;
    QGridLayout *layout;
    QLabel      *createLabel(const QString &text);
    QLabel *createLabelwithFrame(const QString &text);
};








#endif // CASHSTATEMENT_H
