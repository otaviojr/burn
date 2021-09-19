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

bool WifiModel::isScanning() const
{
    return m_isScanning;
}

void WifiModel::startScan()
{
    m_iwd.scan();
    emit layoutChanged();
}

QString WifiModel::parseNetworkId(const QString &networkId)
{
    QStringList list = networkId.split('/', Qt::SkipEmptyParts, Qt::CaseInsensitive);
    qDebug() << "Value " << networkId << " reduced to " << list[list.count()-1];
    return list[list.count()-1];
}

void WifiModel::addOrReplaceNetwork(const WifiNetwork &network)
{
    if(m_networks.contains(parseNetworkId(network.networkId()))){
        WifiNetwork e_network = m_networks.take(parseNetworkId(network.networkId()));
        e_network.setName(network.name());
        e_network.setType(network.type());
        e_network.setKnown(network.known());
        e_network.setConnected(network.connected());
        e_network.setStrength(network.strength());
        m_networks.insert(parseNetworkId(network.networkId()), e_network);
    } else {
        m_networks.insert(parseNetworkId(network.networkId()), network);
    }
    emit dataChanged(index(0), index(m_networks.count()-1));
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
        strength = -1.f;
    } else {
        strength = -2.f * (strength/100. + 1.f);
    }
    strength *= -100;
    qDebug() << "New level for" << m_iwd.networkName(stationId) << " with " << strength << " dbs";

    if(m_networks.contains(parseNetworkId(stationId))){
        qDebug() << "StationId " << stationId << " found";
        WifiNetwork network = m_networks.take(parseNetworkId(stationId));
        network.setStrength(strength);
        this->addOrReplaceNetwork(network);
    }
}

void WifiModel::onVisibleNetworkRemoved(const QString &stationId, const QString &name)
{
    qDebug() << "Visible network removed" << stationId << "-" << name;
    if(m_networks.contains(parseNetworkId(stationId))){
        m_networks.remove(parseNetworkId(stationId));
        emit dataChanged(index(0), index(m_networks.count()-1));
    }
}

void WifiModel::onVisibleNetworkAdded(const QString &stationId, const QString &name,
                                        const QString &type, const bool &connected)
{
    qDebug() << "Visible network added" << stationId << "=" << name;
    WifiNetwork network = WifiNetwork(stationId, name, type);
    network.setConnected(connected);
    this->addOrReplaceNetwork(network);
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
    if(m_networks.contains(parseNetworkId(networkId))){
        m_networks.remove(parseNetworkId(networkId));
        emit dataChanged(index(0), index(m_networks.count()-1));
    }
}

void WifiModel::onKnownNetworkAdded(const QString &networkId, const QString &name,
                                        const QString &type)
{
    qDebug() << "Known network added" << networkId << "=" << name;
    WifiNetwork network = WifiNetwork(networkId, name, type);
    network.setKnown(true);
    this->addOrReplaceNetwork(network);
}

int WifiModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_networks.count();
}

QVariant WifiModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= m_networks.count())
        return QVariant();

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
    return QVariant();
}
