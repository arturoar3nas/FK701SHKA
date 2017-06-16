#include "cashier.h"

Cashier::Cashier()
{

}

QString Cashier::getQstrName() const
{
    return qstrName;
}

void Cashier::setQstrName(const QString &value)
{
    qstrName = value;
}

int Cashier::getNNumber() const
{
    return nNumber;
}

void Cashier::setNNumber(int value)
{
    nNumber = value;
}

int Cashier::getNpswd() const
{
    return npswd;
}

void Cashier::setNpswd(int value)
{
    npswd = value;
}

int Cashier::getNlevel() const
{
    return nlevel;
}

void Cashier::setNlevel(int value)
{
    nlevel = value;
}

