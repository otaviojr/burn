#ifndef __WIFI_AUTH_H__
#define __WIFI_AUTH_H__

#include "wifi/iwd.h"

class WifiAuth : public AuthAgent
{
    Q_OBJECT

public:
    explicit WifiAuth(Iwd *iwd);

signals:
    QString onRequestPrivateKeyPassphraseEvent(const QString &networkId);
    QString onRequestPassphraseEvent(const QString &networkId);
    QPair<QString, QString> onRequestUsernameAndPasswordEvent(const QString &networkId);
    QString onRequestUserPasswordEvent(const QString &username, const QString &networkId);

// AuthAgent interface
public:
    QString onRequestPrivateKeyPassphrase(const QString &networkId) override;
    QString onRequestPassphrase(const QString &networkId) override;
    QPair<QString, QString> onRequestUsernameAndPassword(const QString &networkId) override;
    QString onRequestUserPassword(const QString &username, const QString &networkId) override;
};

#endif // __WIFI_AUTH_H__
