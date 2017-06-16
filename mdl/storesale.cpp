#include "storesale.h"

StoreSale::StoreSale()
{

}

bool StoreSale::isPlu()
{
    return _plu.isEmpty();
}

QStringList StoreSale::plu() const
{
    return _plu;
}

void StoreSale::setPlu(const QString &plu)
{
    _plu.append(plu);
}

QStringList StoreSale::client() const
{
    return _client;
}

void StoreSale::setClient(const QStringList &client)
{
    _client = client;
}

QStringList StoreSale::descRec() const
{
    return _descRec;
}

void StoreSale::setDescRec(const QStringList &descRec)
{
    _descRec = descRec;
}

QStringList StoreSale::pay() const
{
    return _pay;
}

void StoreSale::setPay(const QString &pay)
{
    _pay.append(pay);
}

QStringList StoreSale::inv() const
{
    return _inv;
}

void StoreSale::setInv(const QStringList &inv)
{
    _inv = inv;
}

void StoreSale::removeAtPlu(const int indx)
{
    _plu.removeAt(indx);
}

void StoreSale::cleanTmp()
{
    _plu.clear();
    _client.clear();
    _pay.clear();
    _descRec.clear();
}
