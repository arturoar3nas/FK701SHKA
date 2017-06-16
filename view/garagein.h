#ifndef GARAGEIN_H
#define GARAGEIN_H
#include <QWidget>
#include <QLineEdit>
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

class GarageIn : public QWidget , public IGui
{
    Q_OBJECT
public:
    GarageIn(QWidget *parent = 0);
    void updateWidgets(void *vObj){Q_UNUSED(vObj);}
    void writeOnWidget(QString qstr);
    void backSpaceLineEdit();
    int getFromWidgets(QString &qstr){Q_UNUSED(qstr);return 0;}
    int getFromWidgets(QStringList &qstrl);
    int focusUp();
    int focusDown();
    void clear();
    void release(){delete this;}
    void showWidget(){show();}
    void hideWidget(){hide();}
    bool isVisibleWidget(){return isVisible();}
    int isFocus(){return 0;}
    int  modifyLayout(int nOption){Q_UNUSED(nOption);return 0;}
private:
    QLineEdit *lineEditCategoria;
    QLineEdit *lineEditnPatente;
    QLineEdit *lineEdituPrecio;
    QLineEdit *lineEditdUnidad;
    QGridLayout *layout;
    QLabel      *createLabel(const QString &text);
};

#endif // GARAGE_H
