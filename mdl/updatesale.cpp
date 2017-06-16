#include "updatesale.h"

UpdateSale::UpdateSale()
{

}

QStringList UpdateSale::qstrCalcs() const
{
    return _qstrCalcs;
}

void UpdateSale::setQstrCalcs(const QString &qstrCalcs)
{
    _qstrCalcs.append(qstrCalcs);
}

QString UpdateSale::qstrTotal() const
{
    return _qstrTotal;
}

void UpdateSale::setQstrTotal(const QString &qstrTotal)
{
    _qstrTotal = qstrTotal;
}

QStringList UpdateSale::qstrlItem() const
{
    return _qstrlItem;
}

void UpdateSale::setQstrlItem(const QString &qstrItem)
{
    _qstrlItem.append(qstrItem);
}

void UpdateSale::clear()
{
    _qstrCalcs.clear();
    _qstrlItem.clear();
    _qstrTotal.clear();
}
