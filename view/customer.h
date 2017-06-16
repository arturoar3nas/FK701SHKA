#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QtGui>
#include "view/IGui.h"
#include "util.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE


class Customer : public QWidget , public IGui
{
    Q_OBJECT
public:
    Customer(QWidget *parent = 0);
    void updateWidgets(void *sender){Q_UNUSED(sender);}
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr);
    int getFromWidgets(QStringList &qstrl);
    int focusUp();
    int focusDown();
    void clear();
    void release(){delete this;}
    void showWidget(){lineEditId->setFocus();show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}
private:
    QLineEdit *lineEditId;
    QLineEdit *lineEditRSocial;
    QLineEdit *lineEditnDocument;
    QLineEdit *lineEditResp;
    QTextEdit *textEditAdress;
    QGridLayout *layout;
    QTimer      *timer;
    QLabel      *createLabel(const QString &text);
};

#endif // CUSTOMER_H
