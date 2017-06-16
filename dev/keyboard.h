#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "dev/barcode.h"
#include "util.h"
#ifdef ARM
#include <linux/input.h>
#endif

/* Key state */
#define MAKE					1
#define BREAK					2
#define TYPEMATIC				3
#define INDEX_NO_KEY			0xFF
#define INDEX_GHOST				0xFE
#define INDEX_REPEATING			0xFD
#define IOCTL_KEY_MAGIC         0xF8
#define IOCTL_KEY_GET           _IOR(IOCTL_KEY_MAGIC, 0x00, int) /* Get key index and key status */


class KeyBoard : public QObject
{
    Q_OBJECT

public:
    KeyBoard(QObject * parent = NULL);
    void Start();
    void Stop();
    void Close();
    void Open();
    void Killme();
private slots:
    void MsgLoop_KeyBoard();
signals:
    void sigMoas(int);/*Mother of all signals*/
    void sigBarCode(QString);
private:
    QThread thread;
    bool stopThread;
    int  fd;
    bool Qwerty;
    bool Calc;
    bool Shift;

};

#endif // KEYBOARD_H
