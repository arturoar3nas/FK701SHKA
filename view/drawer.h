#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QtGui>
#include "view/IGui.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Drawer  : public QWidget, public IGui
{
    Q_OBJECT
public:
    Drawer(QWidget *parent = 0);
    void updateWidgets(void *sender);
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr);
    int getFromWidgets(QStringList &qstrl){Q_UNUSED(qstrl);return 0;}
    int focusUp(){return 0;}
    int focusDown(){return 0;}
    void clear();
    void release(){delete this;}
    void showWidget(){show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}
private:
    QLineEdit *lineEdit;
    QLabel *label;
    QGridLayout *layout;
    QLabel      *createLabel(const QString &text);

};

#endif // DRAWER_H
