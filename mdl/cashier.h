#ifndef CASHIER_H
#define CASHIER_H

#include <QString>

class Cashier
{
public:
    Cashier();
    QString getQstrName() const;
    void setQstrName(const QString &value);
    int getNNumber() const;
    void setNNumber(int value);
    int getNpswd() const;
    void setNpswd(int value);
    int getNlevel() const;
    void setNlevel(int value);

private:
    QString qstrName;
    int     nNumber;
    int     npswd;
    int     nlevel;
};

#endif // CASHIER_H
