#ifndef POINTOFSALE_H
#define POINTOFSALE_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QString>
#include <QTimer>
#include <QtGui>
#include "mdl/updatesale.h"
#include "view/IGui.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class PointOfSale : public QWidget, public IGui
{
    Q_OBJECT
public:
    PointOfSale(QWidget *parent = 0);
    int getFromWidgets(QString &qstr);
    int getFromWidgets(QStringList &qstrl){Q_UNUSED(qstrl);return 0;}
    void clear();
    void updateWidgets(void *sender);
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int focusUp();
    int focusDown();
    void release(){delete this;}
    void showWidget(){show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}
private:
    int         m_nlines;
    QStringList sheets;
    int         nSheets;
    int         nCurrentSheet;
    QStringList qstrlItem;
    QString     qstrLcdTotal;
    QTimer      *timer;
    QLabel      *label;
    QLabel      *label_2;
    QLabel      *label_3;
    QLabel      *label_4;
    QLabel      *label_5;
    QLabel      *label_12;
    QLabel      *label_22;
    QLabel      *label_32;
    QLabel      *label_42;
    QLabel      *label_52;
    QLabel      *label_13;
    QLabel      *label_23;
    QLabel      *label_33;
    QLabel      *label_43;
    QLabel      *label_53;
    QLabel      *label_14;
    QLabel      *label_24;
    QLabel      *label_34;
    QLabel      *label_44;
    QLabel      *label_54;
    QLabel      *label_15;
    QLabel      *label_25;
    QLabel      *label_35;
    QLabel      *label_45;
    QLabel      *label_55;
    QLabel      *label_total;
    QLabel      *label_foot;
    QLineEdit   *lineEdit;
    QGridLayout *layout;
    bool         bflag;
    QLabel      *createLabel(const QString &text);
    void cleanLabels();
    int restore();
signals:
    void signal();
private slots:
    void updateLabels();
};



#endif // POINTOFSALE_H
