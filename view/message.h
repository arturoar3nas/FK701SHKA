#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QtGui>
#include <QMessageBox>

#define MAX_TIMER   3000


class Message : public QWidget
{
    Q_OBJECT
public:
    Message(QWidget *parent = 0);
    void openMsgBoxError(int nError);
    void closeMsgBoxError();
    void openMsgBoxInfo(QString qstrMsg);
public slots:
    void openMsgBox(QString qstrMsg);
private slots:
    void closeMessageBox();
private:
    QMessageBox *msgBox;
    QTimer *timer;
    QMap<int, QString> qmapError;
};

#endif // MESSAGE_H
