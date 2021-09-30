#include <QDBusConnection>
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
    connect(&m_iwd, &Iwd::networkConnectedChanged, this, &WifiModel::onNetworkConnectedChanged);

    m_signal = new SignalLevelAgent(&m_iwd);
    if (QDBusConnection::systemBus().registerObject(m_signal->objectPath().path(), m_signal, QDBusConnection::ExportAllContents)) {
        m_iwd.setSignalAgent(m_signal->objectPath(), {-20, -40, -49, -50, -51, -60, -80});
        qDebug() << "set signal agent";
    } else {
        qWarning() << "Failed to register signal agent";
    }
    connect(m_signal, &SignalLevelAgent::signalLevelChanged, this, &WifiModel::onStationSignalChanged);

    m_auth = new WifiAuth(&m_iwd);
    if (QDBusConnection::systemBus().registerObject(m_auth->objectPath().path(), m_auth, QDBusConnection::ExportAllContents)) {
        m_iwd.setAuthAgent(m_auth->objectPath());
        qDebug() << "set auth agent";
    } else {
        qWarning() << "Failed to register auth agent";
    }

    m_iwd.init();
    m_isScanning = false;
}

WifiModel::~WifiModel(){
    QList<QPointer<WifiNetwork>> networks = m_networks.values();

    while(!networks.isEmpty()){
        delete networks.takeFirst();
    }

    m_networks.clear();

    delete m_signal;
    delete m_auth;
}

bool WifiModel::isScanning() const
{
    return m_isScanning;
}

bool WifiModel::isConnected() const
{
    return (this->m_connectedNetwork != NULL);
}

void WifiModel::startScan()
{
    m_iwd.scan();
}

void WifiModel::connectNetwork(const QString& id)
{
    this->connectNetwork(id, "", "");
}

void WifiModel::connectNetworkWithPassphrase(const QString& id, const QString& passphrase)
{
    this->connectNetwork(id, "", passphrase);
}

void WifiModel::connectNetwork(const QString& id, const QString &username, const QString &password)
{
    qDebug() << "Connecting at network" << id;
    if(this->m_networks.contains(id)){
        qDebug() << "Network" << id << "found";
        QPointer<WifiNetwork> network = this->m_networks.value(id);
        if(network){
            qDebug() << "Connecting at networkId" << network->stationId();
            this->m_auth->setWifiPassword(username, password);
            this->m_iwd.connectNetwork(network->stationId());
        }
    }
}

void WifiModel::disconnectNetwork(const QString& id)
{
    qDebug() << "Disconnecting from network" << id;
    if(m_networks.contains(id)){
        qDebug() << "Network" << id << "found";
        QPointer<WifiNetwork> network = m_networks.value(id);
        if(network){
            qDebug() << "Disconnecting from stationId" << this->parseStationId(network->stationId());
            m_iwd.disconnectStation(this->parseStationId(network->stationId()));
            this->m_connectedNetwork = NULL;
            emit isConnectedChanged(false);
        }
    }
}

void WifiModel::forgetNetwork(const QString& id)
{
    qDebug() << "Forgetting from network" << id;
    if(m_networks.contains(id)){
        qDebug() << "Network" << id << "found";
        QPointer<WifiNetwork> network = m_networks.value(id);
        if(network){
            qDebug() << "Forgetting stationId" << network->id();
            m_iwd.forgetNetwork(network->networkId());
            network->setKnown(false);
            refreshWifiList();
        }
    }
}

void WifiModel::refreshWifiList()
{
    this->m_orderedNetworks = m_networks.values();
    std::sort(this->m_orderedNetworks.begin(), this->m_orderedNetworks.end(), [](QPointer<WifiNetwork> n1, QPointer<WifiNetwork> n2){
        return *n1 > *n2;
    });
    emit dataChanged(index(0), index(this->m_orderedNetworks.count()-1));
}

QString WifiModel::parseNetworkId(const QString &networkId)
{
    QStringList list = networkId.split('/', Qt::SkipEmptyParts, Qt::CaseInsensitive);
    qDebug() << "Value " << networkId << " reduced to " << list[list.count()-1];
    return list[list.count()-1];
}

QString WifiModel::parseStationId(const QString &stationId)
{
    QStringList list = stationId.split('/', Qt::SkipEmptyParts, Qt::CaseInsensitive);
    list.removeLast();
    qDebug() << "Value " << stationId << " reduced to " << ("/" + list.join('/'));
    return ("/" + list.join('/'));
}

QPointer<WifiNetwork> WifiModel::addOrReplaceNetwork(QPointer<WifiNetwork> network, const unsigned int &changed)
{
    QPointer<WifiNetwork> ret_network;

    //QMutexLocker locker(&mutex);
    if(m_networks.contains(network->id())){
        QPointer<WifiNetwork> e_network = m_networks.value(network->id());
        if(e_network){
            if(changed & AllChanged || changed & NetworkIdChanged)
                e_network->setNetworkId(network->networkId());

            if(changed & AllChanged || changed & StationIdChanged)
                e_network->setStationId(network->stationId());

            if(changed & AllChanged || changed & NameChanged)
                e_network->setName(network->name());

            if(changed & AllChanged || changed & TypeChanged)
                e_network->setType(network->type());

            if(changed & AllChanged || changed & KnownChanged)
                e_network->setKnown(network->known());

            if(changed & AllChanged || changed & StrengthChanged)
                e_network->setStrength(network->strength());

            delete network;
            ret_network = e_network;
        }
    } else {
        m_networks.insert(network->id(), network);
        ret_network = network;
    }
    refreshWifiList();
    return ret_network;
}

void WifiModel::onScanningChanged(const QString &station, bool isScanning)
{
    m_isScanning = isScanning;
    emit layoutChanged();
    emit isScanningChanged(m_isScanning);
}

void WifiModel::onNetworkConnectedChanged(const QString &networkId, bool isConnected)
{
    if(m_networks.contains(parseNetworkId(networkId))){
        qDebug() << "NetworkId " << networkId << "is" << (isConnected ? "connected" : "disconnected");
        QPointer<WifiNetwork> network = m_networks.value(parseNetworkId(networkId));
        if(network && isConnected){
            m_connectedNetwork = network;
            emit isConnectedChanged(isConnected);
        } else {
            if(m_connectedNetwork && m_connectedNetwork->id() == parseNetworkId(networkId)){
                m_connectedNetwork = NULL;
                emit isConnectedChanged(isConnected);
            }
        }
        refreshWifiList();
    }
}

void WifiModel::onStationSignalChanged(const QString &stationId, int newLevel)
{
    //QMutexLocker locker(&mutex);
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
        QPointer<WifiNetwork> network = m_networks.value(parseNetworkId(stationId));
        if(network){
            network->setStrength(strength);
            refreshWifiList();
        }
    }
}

void WifiModel::onVisibleNetworkRemoved(const QString &stationId, const QString &name)
{
    //QMutexLocker locker(&mutex);
    qDebug() << "Visible network removed" << stationId << "-" << name;
    if(m_networks.contains(parseNetworkId(stationId))){
        QPointer<WifiNetwork> network = m_networks.value(parseNetworkId(stationId));
        if(network){
            m_networks.remove(network->id());
            refreshWifiList();
            delete network;
        }
    }
}

void WifiModel::onVisibleNetworkAdded(const QString &stationId, const QString &name,
                                        const QString &type, const bool &connected)
{
    qDebug() << "Visible network added" << stationId << "=" << name << "(connected=" << connected << ")";
    QPointer<WifiNetwork> network = new WifiNetwork(parseNetworkId(stationId), name, type);
    network->setStationId(stationId);

    QPointer<WifiNetwork> ret_network = this->addOrReplaceNetwork(network, StationIdChanged);
    if(connected){
        this->m_connectedNetwork = ret_network;
        emit isConnectedChanged(connected);
    }
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
        QPointer<WifiNetwork> network = m_networks.value(parseNetworkId(networkId));
        if(network){
            if(network->stationId().isEmpty()){
                m_networks.remove(network->id());
                delete network;
            } else {
                network->setKnown(false);
            }
            refreshWifiList();
        }
    }
}

void WifiModel::onKnownNetworkAdded(const QString &networkId, const QString &name,
                                        const QString &type)
{
    qDebug() << "Known network added" << networkId << "=" << name;
    QPointer<WifiNetwork> network = new WifiNetwork(parseNetworkId(networkId), name, type);
    network->setNetworkId(networkId);
    network->setKnown(true);
    this->addOrReplaceNetwork(network, KnownChanged | NetworkIdChanged);
}

int WifiModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return this->m_orderedNetworks.count();
}

QVariant WifiModel::data(const QModelIndex & index, int role) const
{
    //QMutexLocker locker(&mutex);

    if (index.row() < 0 || index.row() >= this->m_orderedNetworks.count())
        return QVariant();

    //QPointer<WifiNetwork> network = m_networks.values().value(index.row());
    QPointer<WifiNetwork> network = this->m_orderedNetworks.value(index.row());
    if(!network){
        return QVariant();
    }

    switch (role) {
        case IdRole:
            return QVariant(network->id());
        case NetworkIdRole:
            return QVariant(network->networkId());
        case StationIdRole:
            return QVariant(network->stationId());
        case NameRole:
            return QVariant(network->name());
        case TypeRole:
            return QVariant(network->type());
        case KnownRole:
            return QVariant(network->known());
        case ConnectedRole:
            return QVariant(this->m_connectedNetwork && this->m_connectedNetwork->id() == network->id());
        case StrengthRole:
            return QVariant(network->strength());
    }
    return QVariant();
}

void WifiModel::setWifiPassword(const QString &password)
{
    this->m_auth->setWifiPassword("",password);
}

void WifiModel::setWifiUsernamePassword(const QString &username, const QString &password)
{
    this->m_auth->setWifiPassword(username,password);
}
