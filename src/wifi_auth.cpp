#include "wifi_auth.h"

WifiAuth::WifiAuth(Iwd *iwd) : AuthAgent(iwd)
{
    connect(this, &AuthAgent::released, this, [this](){
        qDebug() << this << "Released";
    });

    connect(this, &AuthAgent::released, this, &QObject::deleteLater);
}

QString WifiAuth::onRequestPrivateKeyPassphrase(const QString &networkId)
{
    qDebug() << "Requesting private key passphrase for" << networkId;
    return "";
}

QString WifiAuth::onRequestPassphrase(const QString &networkId)
{
    qDebug() << "Requesting passphrase for" << networkId;
    return "";
}

QPair<QString, QString> WifiAuth::onRequestUsernameAndPassword(const QString &networkId)
{
    qDebug() << "Requesting username and password for" << networkId;
    return QPair("","") ;
}

QString WifiAuth::onRequestUserPassword(const QString &username, const QString &networkId)
{
    qDebug() << "Requesting user password for" << username << "for" << networkId;
    return "";
}
