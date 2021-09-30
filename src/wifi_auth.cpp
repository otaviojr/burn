#include "wifi_auth.h"

WifiAuth::WifiAuth(Iwd *iwd) : AuthAgent(iwd)
{
    connect(this, &AuthAgent::released, this, [this](){
        qDebug() << this << "Released";
    });

    connect(this, &AuthAgent::released, this, &QObject::deleteLater);
}

void WifiAuth::setWifiPassword(const QString &username, const QString &password)
{
    this->m_password = password;
    this->m_username = username;
}

QString WifiAuth::onRequestPrivateKeyPassphrase(const QString &networkId)
{
    qDebug() << "Requesting private key passphrase for" << networkId;
    return this->m_password;
}

QString WifiAuth::onRequestPassphrase(const QString &networkId)
{
    qDebug() << "Requesting passphrase for" << networkId;
    qDebug() << "Using password" << this->m_password;
    return this->m_password;
}

QPair<QString, QString> WifiAuth::onRequestUsernameAndPassword(const QString &networkId)
{
    qDebug() << "Requesting username and password for" << networkId;
    return QPair(this->m_username, this->m_password);
}

QString WifiAuth::onRequestUserPassword(const QString &username, const QString &networkId)
{
    qDebug() << "Requesting user password for" << username << "for" << networkId;
    return this->m_password;
}
