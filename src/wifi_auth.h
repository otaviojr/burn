#ifndef __WIFI_AUTH_H__
#define __WIFI_AUTH_H__

#include "wifi/iwd.h"

class WifiAuth : public AuthAgent
{
    Q_OBJECT

public:
    explicit WifiAuth(Iwd *iwd);

private:
    QString m_password;
    QString m_username;

public:
    void setWifiPassword(const QString &username, const QString &password);

// AuthAgent interface
protected:
    QString onRequestPrivateKeyPassphrase(const QString &networkId) override;
    QString onRequestPassphrase(const QString &networkId) override;
    QPair<QString, QString> onRequestUsernameAndPassword(const QString &networkId) override;
    QString onRequestUserPassword(const QString &username, const QString &networkId) override;
};

#endif // __WIFI_AUTH_H__
