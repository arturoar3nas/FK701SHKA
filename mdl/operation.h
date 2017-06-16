#ifndef OPERATION_H
#define OPERATION_H

#include <QString>

class Operation
{
public:
    Operation();
    QString price() const;
    void setPrice(const QString &price);
    QString input() const;
    void setInput(const QString &input);
    float quanty() const;
    void setQuanty(float quanty);
    float priceGroup() const;
    void setPriceGroup(float priceGroup);
    int type() const;
    void setType(int type);
    void clear();
    int meanofpay() const;
    void setMeanofpay(int meanofpay);
    QString plu() const;
    void setPlu(const QString &plu);

private:
    QString _price;
    QString _input;
    QString _plu;
    float  _quanty;
    float  _priceGroup;
    int    _type;
    int    _meanofpay;
};

#endif // OPERATION_H
