#include "operation.h"

Operation::Operation()
{
    _quanty = 0;
    _priceGroup = 0;
    _type = 0;
    _meanofpay = 0;
}

QString Operation::price() const
{
    return _price;
}

void Operation::setPrice(const QString &price)
{
    _price = price;
}

QString Operation::input() const
{
    return _input;
}

void Operation::setInput(const QString &input)
{
    _input = input;
}

float Operation::quanty() const
{
    return _quanty;
}

void Operation::setQuanty(float quanty)
{
    _quanty = quanty;
}

float Operation::priceGroup() const
{
    return _priceGroup;
}

void Operation::setPriceGroup(float priceGroup)
{
    _priceGroup = priceGroup;
}

int Operation::type() const
{
    return _type;
}

void Operation::setType(int type)
{
    _type = type;
}

void Operation::clear()
{
    _type = 0;
    _quanty = 0;
    _priceGroup = 0;
    _price.clear();
    _input.clear();
}

int Operation::meanofpay() const
{
    return _meanofpay;
}

void Operation::setMeanofpay(int meanofpay)
{
    _meanofpay = meanofpay;
}

QString Operation::plu() const
{
    return _plu;
}

void Operation::setPlu(const QString &plu)
{
    _plu = plu;
}

