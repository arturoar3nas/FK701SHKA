#ifndef STORESALE_H
#define STORESALE_H

#include <QString>
#include <QStringList>

class StoreSale
{
public:
    StoreSale();
    QStringList plu() const;
    void setPlu(const QString &plu);
    QStringList client() const;
    void setClient(const QStringList &client);
    QStringList descRec() const;
    void setDescRec(const QStringList &descRec);
    QStringList pay() const;
    void setPay(const QString &pay);
    QStringList inv() const;
    void setInv(const QStringList &inv);
    void cleanTmp();
    void removeAtPlu(const int indx);
    bool isPlu();
private:
    QStringList _plu;
    QStringList _client;
    QStringList _descRec;
    QStringList _pay;
    QStringList _inv;
};

#endif // STORESALE_H
