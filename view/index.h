#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
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

#define OPTION_LAYOUT_HOME     0
#define OPTION_LAYOUT_X_MODE   1
#define OPTION_LAYOUT_Z_MODE   2
#define OPTION_LAYOUT_X1       3
#define OPTION_LAYOUT_Z1       4
#define OPTION_LAYOUT_Z2       5

class Index : public QWidget, public IGui
{
    Q_OBJECT
public:
    Index(QWidget *parent = 0);
    void updateWidgets(void *sender){Q_UNUSED(sender);}
    void writeOnWidget(QString qstr){Q_UNUSED(qstr);}
    void backSpaceLineEdit(){}
    int getFromWidgets(QString &qstr){Q_UNUSED(qstr);return 0;}
    int getFromWidgets(QStringList &qstrl){Q_UNUSED(qstrl);return 0;}
    int focusUp();
    int focusDown();
    void clear(){}
    void release(){delete this;}
    void showWidget(){label_1->setFocus();show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus();
    int  modifyLayout(int nOption);
private:
    QGridLayout *layout;
    QTimer      *timer;
    QLabel      *label_1;
    QLabel      *label_2;
    QLabel      *label_3;
    QLabel      *label_4;
    QLabel      *label_5;
    QLabel      *label_6;
    QLabel      *label_7;

    QLabel      *createLabel(const QString &text);
    QLabel *createLabelwithFrame(const QString &text);
    void setTexttoLabel(QString qstr, int nLabel) const;
};

#endif // INDEX_H
