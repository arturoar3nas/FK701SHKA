#ifndef LOGUSER_H
#define LOGUSER_H

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QtGui>
#include "util.h"
#include "view/IGui.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class LogUsr  : public QWidget, public IGui
{
    Q_OBJECT
public:
    LogUsr(QWidget *parent = 0);
private:
    QLineEdit *lineEditUsr;
    QLineEdit *lineEditPswd;
    QGridLayout *layout;
    QTimer      *timer;
    QLabel      *createLabel(const QString &text);
public:
    void updateWidgets(void *sender){Q_UNUSED(sender);}
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr);
    int getFromWidgets(QStringList &qstrl);
    int focusUp();
    int focusDown();
    void clear();
    void release(){delete this;}
    void showWidget(){lineEditUsr->setFocus();show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}
};

#endif // LOGUSER_H
