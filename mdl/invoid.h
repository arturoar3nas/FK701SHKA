#ifndef INVOID
#define INVOID

#include <QString>
#include <QStringList>

class Invoid
{
private:
    QStringList Client;
    QStringList Items;
    QStringList DescRec;
public:
    unsigned    oldInvoid;

public:
    Invoid(){}
    int getClient(QStringList &qstr)
    {
        //! qstr = QStringList::QStringList(Client);
        qstr = Client;
        return 0;
    }
    int getItems(QStringList &qstr)
    {
        //! qstr = QStringList::QStringList(Items);
        qstr = Items;
        return 0;
    }
    void cleanItems()
    {
        Items.clear();
    }
    void setClient(QString qstr)
    {
        Client.append(qstr);
    }

    void setItem(QString qstr)
    {
        Items.append(qstr);
    }
};
#endif // INVOID

