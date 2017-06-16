#include "client.h"

Client::Client()
{
    bclient = false;
}

void Client::clear()
{
    id->clear();
    nDocument->clear();
    Resp->clear();
    razonSocial->clear();
    address->clear();
    bclient = false;
}

QString *Client::getId() const
{
    return id;
}

void Client::setId(QString *value)
{
    id = value;
}

QString *Client::getNDocument() const
{
    return nDocument;
}

void Client::setNDocument(QString *value)
{
    nDocument = value;
}

QString *Client::getResp() const
{
    return Resp;
}

void Client::setResp(QString *value)
{
    Resp = value;
}

QString *Client::getRazonSocial() const
{
    return razonSocial;
}

void Client::setRazonSocial(QString *value)
{
    razonSocial = value;
}

QString *Client::getAddress() const
{
    return address;
}

void Client::setAddress(QString *value)
{
    address = value;
}

bool Client::isClient() const
{
    return bclient;
}

void Client::setBclient(bool value)
{
    bclient = value;
}

