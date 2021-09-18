#include <QDBusMetaType>

#include "wifi_model.h"
#include "wifi/custom_types.h"

WifiModel::WifiModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<ManagedObject>("ManagedObject");
    qDBusRegisterMetaType<ManagedObject>();

    qRegisterMetaType<ManagedObjectList>("ManagedObjectList");
    qDBusRegisterMetaType<ManagedObjectList>();

    qRegisterMetaType<OrderedNetworkList>("OrderedNetworkList");
    qDBusRegisterMetaType<OrderedNetworkList>();
    qDBusRegisterMetaType<QPair<QDBusObjectPath,int16_t>>();

    qRegisterMetaType<LevelsList>("LevelsList");
    qDBusRegisterMetaType<LevelsList>();

    connect(&m_iwd, &Iwd::visibleNetworkAdded, this, &WifiModel::onVisibleNetworkAdded);
    connect(&m_iwd, &Iwd::visibleNetworkRemoved, this, &WifiModel::onVisibleNetworkRemoved);
    connect(&m_iwd, &Iwd::knownNetworkAdded, this, &WifiModel::onKnownNetworkAdded);
    connect(&m_iwd, &Iwd::knownNetworkRemoved, this, &WifiModel::onKnownNetworkRemoved);
    connect(&m_iwd, &Iwd::deviceAdded, this, &WifiModel::onDeviceAdded);
    connect(&m_iwd, &Iwd::deviceRemoved, this, &WifiModel::onDeviceRemoved);
    connect(&m_iwd, &Iwd::signalLevelChanged, this, &WifiModel::onStationSignalChanged);
    connect(&m_iwd, &Iwd::stationScanningChanged, this, &WifiModel::onScanningChanged);

    m_iwd.init();

    m_isScanning = false;
}

WifiModel::~WifiModel()
{
}

bool WifiModel::isScanning() const
{
    return m_isScanning;
}

void WifiModel::startScan()
{
    m_iwd.scan();
}

void WifiModel::onScanningChanged(const QString &station, bool isScanning)
{
    m_isScanning = isScanning;
    isScanningChanged(m_isScanning);
}

void WifiModel::onStationSignalChanged(const QString &stationId, int newLevel)
{
    float strength = newLevel / 100.;
    if (strength == 0) { // special case in QtBluetooth apparently
        strength = 0.f;
    } else  if(strength <= -100) {
        strength = 0.f;
    } else if(strength >= -50) {
        strength = -100.f;
    } else {
        strength = -2.f * (strength/100. + 1.f);
    }
    strength *= -100;
    qDebug() << "New level for" << m_iwd.networkName(stationId) << " with " << strength << " dbs";

    if(m_networks.contains(stationId)){
        WifiNetwork network = m_networks.value(stationId);
        network.setStrength(strength);
        dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), roleVectors());
    }
}

void WifiModel::onVisibleNetworkRemoved(const QString &stationId, const QString &name)
{
    qDebug() << "Visible network removed" << stationId << "-" << name;
    if(m_networks.contains(stationId)){
        m_networks.remove(stationId);
        dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), roleVectors());
    }
}

void WifiModel::onVisibleNetworkAdded(const QString &stationId, const QString &name,
                                        const QString &type, const bool &connected)
{
    qDebug() << "Visible network added" << stationId << "=" << name;
    WifiNetwork network = WifiNetwork(stationId, name, type);
    network.setConnected(connected);
    m_networks.insert(stationId, network);
    dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), roleVectors());
}

void WifiModel::onDeviceAdded(const QString &stationId, const QString &name)
{
    qDebug() << "Device added " << stationId << "-" << name;
}

void WifiModel::onDeviceRemoved(const QString &stationId)
{
    qDebug() << "Device removed " << stationId;
}

void WifiModel::onKnownNetworkRemoved(const QString &networkId, const QString &name)
{
    qDebug() << "Known network removed" << networkId << "=" << name;
    if(m_networks.contains(networkId)){
        m_networks.remove(networkId);
        dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), roleVectors());
    }
}

void WifiModel::onKnownNetworkAdded(const QString &networkId, const QString &name,
                                        const QString &type)
{
    qDebug() << "Known network added" << networkId << "=" << name;
    WifiNetwork network = WifiNetwork(networkId, name, type);
    network.setKnown(true);
    m_networks.insert(networkId, network);
    dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0), roleVectors());
}

int WifiModel::rowCount(const QModelIndex & parent) const
{
    return m_networks.count();
}
QVariant WifiModel::data(const QModelIndex & index, int role) const
{
    WifiNetwork network = m_networks.values().value(index.row());

    switch (role) {
        case NameRole:
            return QVariant(network.name());
        case TypeRole:
            return QVariant(network.type());
        case KnownRole:
            return QVariant(network.known());
        case ConnectedRole:
            return QVariant(network.connected());
        case StrengthRole:
            return QVariant(network.strength());
    }
    return QVariant("");
}
