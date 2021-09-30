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
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_ENUMS(WifiModelRoles)
public:
    enum WifiModelRoles {
        IdRole = Qt::UserRole + 1,
        NetworkIdRole,
        StationIdRole,
        NameRole,
        TypeRole,
        KnownRole,
        ConnectedRole,
        StrengthRole
    };

    enum WifiModelChanged {
        NetworkIdChanged      = 0b00000001,
        StationIdChanged      = 0b00000010,
        NameChanged           = 0b00000100,
        TypeChanged           = 0b00001000,
        KnownChanged          = 0b00010000,
        StrengthChanged       = 0b00100000,
        AllChanged            = 0b10000000
    };

    WifiModel(QObject *parent = Q_NULLPTR);
    ~WifiModel();

    bool isScanning() const;
    bool isConnected() const;

    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[NetworkIdRole] = "networkId";
        roles[StationIdRole] = "stationId";
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[KnownRole] = "known";
        roles[ConnectedRole] = "connected";
        roles[StrengthRole] = "strength";
        return roles;
    }

public:
    Q_INVOKABLE void startScan();
    Q_INVOKABLE void connectNetwork(const QString &id);
    Q_INVOKABLE void connectNetworkWithPassphrase(const QString& id, const QString& passphrase);
    Q_INVOKABLE void connectNetwork(const QString &id, const QString &username, const QString &password);
    Q_INVOKABLE void disconnectNetwork(const QString &id);
    Q_INVOKABLE void forgetNetwork(const QString &id);
    Q_INVOKABLE void setWifiPassword(const QString &password);
    Q_INVOKABLE void setWifiUsernamePassword(const QString &username, const QString &password);

    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;


private slots:
    void onKnownNetworkRemoved(const QString &networkId, const QString &name);
    void onKnownNetworkAdded(const QString &networkId, const QString &name, const QString &type);
    void onDeviceAdded(const QString &stationId, const QString &name);
    void onDeviceRemoved(const QString &stationId);
    void onVisibleNetworkRemoved(const QString &stationId, const QString &name);
    void onVisibleNetworkAdded(const QString &stationId, const QString &name, const QString &type, const bool &connected);
    void onStationSignalChanged(const QString &stationId, int newLevel);
    void onScanningChanged(const QString &stationId, bool isScanning);
    void onNetworkConnectedChanged(const QString &networkId, bool isConnected);

signals:
    void isScanningChanged(const bool value);
    void isConnectedChanged(const bool value);

private:
    void refreshWifiList();
    QString parseNetworkId(const QString &networkId);
    QString parseStationId(const QString &stationId);
    QPointer<WifiNetwork> addOrReplaceNetwork(QPointer<WifiNetwork> network, const unsigned int &changed);

private:
    bool m_isScanning;
    Iwd m_iwd;
    QPointer<WifiNetwork> m_connectedNetwork;
    QMap<QString, QPointer<WifiNetwork>> m_networks;
    QList<QPointer<WifiNetwork>> m_orderedNetworks;
    QPointer<SignalLevelAgent> m_signal;
    QPointer<WifiAuth> m_auth;
    //QMutex mutex;
};

#endif
