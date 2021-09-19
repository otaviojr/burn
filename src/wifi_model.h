#ifndef __WIFI_MODEL__
#define __WIFI_MODEL__

#include <QAbstractListModel>
#include <QHash>

#include "wifi/iwd.h"
#include "wifi_network.h"

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

private slots:
    void onKnownNetworkRemoved(const QString &networkId, const QString &name);
    void onKnownNetworkAdded(const QString &networkId, const QString &name, const QString &type);
    void onDeviceAdded(const QString &stationId, const QString &name);
    void onDeviceRemoved(const QString &stationId);
    void onVisibleNetworkRemoved(const QString &stationId, const QString &name);
    void onVisibleNetworkAdded(const QString &stationId, const QString &name, const QString &type, const bool &connected);
    void onStationSignalChanged(const QString &stationId, int newLevel);
    void onScanningChanged(const QString &station, bool isScanning);

signals:
    void isScanningChanged(const bool value);

private:
    QString parseNetworkId(const QString &networkId);
    void addOrReplaceNetwork(const WifiNetwork &network);

private:
    bool m_isScanning;
    Iwd m_iwd;
    QHash<QString, WifiNetwork> m_networks;
};

#endif
