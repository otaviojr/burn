#ifndef __WIFI_MODEL__
#define __WIFI_MODEL__

#include <QAbstractListModel>
#include <QQmlComponent>
#include <QHash>
#include <QMap>

#include "wifi/iwd.h"
#include "wifi_network.h"
#include "wifi_auth.h"

class WifiModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)
    Q_ENUMS(WifiModelRoles)
public:
    enum WifiModelRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        KnownRole,
        ConnectedRole,
        StrengthRole
    };

    enum WifiModelChanged {
        NameChanged      = 0b00000001,
        TypeChanged      = 0b00000010,
        KnownChanged     = 0b00000100,
        ConnectedChanged = 0b00001000,
        StrengthChanged  = 0b00010000,
        AllChanged       = 0b10000000
    };

    WifiModel(QObject *parent = Q_NULLPTR);

    bool isScanning() const;

    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[KnownRole] = "known";
        roles[ConnectedRole] = "connected";
        roles[StrengthRole] = "strength";
        return roles;
    }

public:
    Q_INVOKABLE void startScan();
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void setWifiPassword(const QString &password);
    Q_INVOKABLE void setWifiUsernamePassword(const QString &username, const QString &password);

private slots:
    void onKnownNetworkRemoved(const QString &networkId, const QString &name);
    void onKnownNetworkAdded(const QString &networkId, const QString &name, const QString &type);
    void onDeviceAdded(const QString &stationId, const QString &name);
    void onDeviceRemoved(const QString &stationId);
    void onVisibleNetworkRemoved(const QString &stationId, const QString &name);
    void onVisibleNetworkAdded(const QString &stationId, const QString &name, const QString &type, const bool &connected);
    void onStationSignalChanged(const QString &stationId, int newLevel);
    void onScanningChanged(const QString &station, bool isScanning);

    QString onRequestPrivateKeyPassphrase(const QString &networkId);
    QString onRequestPassphrase(const QString &networkId);
    QPair<QString, QString> onRequestUsernameAndPassword(const QString &networkId);
    QString onRequestUserPassword(const QString &username, const QString &networkId);

signals:
    void isScanningChanged(const bool value);
    void requestPassword();
    void requestUsernamePassword();
    void requestPasswordForUsername(const QString &username);
    void wifiPassword();

private:
    QString parseNetworkId(const QString &networkId);
    void addOrReplaceNetwork(const WifiNetwork &network, const enum WifiModelChanged &changed);

private:
    bool m_isScanning;
    QQmlComponent *m_delegateLogin;
    Iwd m_iwd;
    QMap<QString, WifiNetwork> m_networks;
    WifiAuth m_auth;
    QString m_password;
    QString m_username;
};

#endif
