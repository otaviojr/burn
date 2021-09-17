#ifndef __WIFI_MODEL__
#define __WIFI_MODEL__

#include <QAbstractListModel>

#include "wifi/iwd.h"

class WifiModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum WifiModelRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole
    };

    WifiModel(QObject *parent = 0);
    ~WifiModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        return roles;
    }

private slots:
    void onKnownNetworkRemoved(const QString &networkId, const QString &name);
    void onKnownNetworkAdded(const QString &networkId, const QString &name);
    void onDeviceAdded(const QString &stationId, const QString &name);
    void onDeviceRemoved(const QString &stationId);
    void onVisibleNetworkRemoved(const QString &stationId, const QString &name);
    void onVisibleNetworkAdded(const QString &stationId, const QString &name);
    void onStationSignalChanged(const QString &stationId, int newLevel);
    void onScanningChanged(const QString &station, bool isScanning);

private:
    Iwd m_iwd;
};

#endif
