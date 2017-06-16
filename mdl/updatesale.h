#ifndef INFOSALE_H
#define INFOSALE_H

#include <QString>
#include <QStringList>

class UpdateSale
{
public:
    UpdateSale();
    QStringList qstrCalcs() const;
    void setQstrCalcs(const QString &qstrCalcs);
    QString qstrTotal() const;
    void setQstrTotal(const QString &qstrTotal);
    QStringList qstrlItem() const;
    void setQstrlItem(const QString &qstrItem);
    void clear();
private:
    QStringList _qstrCalcs;
    QString _qstrTotal;
    QStringList _qstrlItem;
};

#endif // INFOSALE_H
