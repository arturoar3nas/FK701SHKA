#ifndef LAYOUTREPORT_H
#define LAYOUTREPORT_H

#include <QtGui>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QPlainTextEdit>
#include "view/IGui.h"
#include "util.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTextEdit;
class QGridLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class LayoutReport : public QWidget , public IGui
{
    Q_OBJECT
public:
    explicit LayoutReport(QWidget *parent = 0);
    void updateWidgets(void *sender);
    void writeOnWidget(QString qstr){Q_UNUSED(qstr);}
    void backSpaceLineEdit(){}
    int getFromWidgets(QString &qstr){Q_UNUSED(qstr);return 0;}
    int getFromWidgets(QStringList &qstrl){Q_UNUSED(qstrl);return 0;}
    int focusUp();
    int focusDown();
    void clear(){textEdit->clear();}
    void release(){delete this;}
    void showWidget(){show();textEdit->clear();qApp->processEvents();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}

private:
    QLabel      *label_title;
    QLabel      *label_espacio;
    QLabel      *label_alignRigt;
    QPlainTextEdit *textEdit;
    QLabel      *label_accept;
    QLabel      *label_return;
    QGridLayout *layout_report;
    QHBoxLayout *row_box_mid;
    QHBoxLayout *row_box_bot;
};

#endif // LAYOUTREPORT_H
