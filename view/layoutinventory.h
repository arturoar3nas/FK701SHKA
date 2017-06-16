#ifndef LAYOUTINVENTORY_H
#define LAYOUTINVENTORY_H

#include <QtGui>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QDebug>
#include "view/IGui.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QTextEdit;
QT_END_NAMESPACE

class LayoutInventory : public QWidget  , public IGui
{
    Q_OBJECT
public:
    explicit LayoutInventory(QWidget *parent = 0);

private:
    QLabel      *label_title;
    QLabel      *label_help;
    QLabel      *label_espacio;
    QLabel      *label_alignRigt;
    QLineEdit   *lineEdit_1;
    QPlainTextEdit   *textEdit;
    QLabel      *label_accept;
    QLabel      *label_return;
    QGridLayout *layout_inventory;
    QVBoxLayout *colunm_1;
    QHBoxLayout *row_box_top;
    QHBoxLayout *row_box_mid;
    QHBoxLayout *row_box_bot;
    QHBoxLayout *row_box_space;

    void updateWidgets(void *sender);
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr);
    int getFromWidgets(QStringList &qstrl);
    int focusUp();
    int focusDown();
    void clear();
    void release(){delete this;}
    void showWidget(){lineEdit_1->setFocus();show();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus();
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}

};

#endif // LAYOUTINVENTORY_H
