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
    m_iwd.scan();
}

WifiModel::~WifiModel()
{

}

void WifiModel::onScanningChanged(const QString &station, bool isScanning)
{
    if(isScanning){
        qDebug() << "Starting wifi network scan";
    } else {
        qDebug() << "Stopping wifi network scan";
    }
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
}

void WifiModel::onVisibleNetworkRemoved(const QString &stationId, const QString &name)
{
    qDebug() << "Visible network removed" << stationId << "-" << name;
}

void WifiModel::onVisibleNetworkAdded(const QString &stationId, const QString &name)
{
    qDebug() << "Visible network added" << stationId << "=" << name;
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
}

void WifiModel::onKnownNetworkAdded(const QString &networkId, const QString &name)
{
}

int WifiModel::rowCount(const QModelIndex & parent) const
{
    return 0;
}
QVariant WifiModel::data(const QModelIndex & index, int role) const
{
    return QVariant("Teste");
}
