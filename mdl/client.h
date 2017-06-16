#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
public:
    Client();
    void clear();
    QString *getId() const;
    void setId(QString *value);
    QString *getNDocument() const;
    void setNDocument(QString *value);
    QString *getResp() const;
    void setResp(QString *value);
    QString *getRazonSocial() const;
    void setRazonSocial(QString *value);
    QString *getAddress() const;
    void setAddress(QString *value);
    bool isClient() const;
    void setBclient(bool value);

private:
    QString *id;
    QString *nDocument;
    QString *Resp;
    QString *razonSocial;
    QString *address;
    bool    bclient;
};

#endif // CLIENT_H
