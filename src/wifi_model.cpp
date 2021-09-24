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

    m_auth = new WifiAuth(&m_iwd);
    if (QDBusConnection::systemBus().registerObject(m_auth->objectPath().path(), this)) {
        m_iwd.setAuthAgent(m_auth->objectPath());
    } else {
        qWarning() << "Failed to register auth agent";
    }

    connect(m_auth, &WifiAuth::onRequestPrivateKeyPassphraseEvent, this,
                &WifiModel::onRequestPrivateKeyPassphrase, Qt::BlockingQueuedConnection);

    connect(m_auth, &WifiAuth::onRequestPassphrase, this,
                &WifiModel::onRequestPassphrase, Qt::BlockingQueuedConnection);

    m_iwd.init();
    m_isScanning = false;
}

WifiModel::~WifiModel(){
    delete m_auth;
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

void WifiModel::connectNetwork(const QString& networkId)
{
    m_iwd.connectNetwork(networkId);
}

void WifiModel::disconnectNetwork(const QString& networkId)
{
    m_iwd.disconnectStation(networkId);
}

QString WifiModel::parseNetworkId(const QString &networkId)
{
    QStringList list = networkId.split('/', Qt::SkipEmptyParts, Qt::CaseInsensitive);
    qDebug() << "Value " << networkId << " reduced to " << list[list.count()-1];
    return list[list.count()-1];
}

void WifiModel::addOrReplaceNetwork(const WifiNetwork &network, const enum WifiModelChanged &changed)
{
    if(m_networks.contains(parseNetworkId(network.networkId()))){
        WifiNetwork e_network = m_networks.take(parseNetworkId(network.networkId()));
        if(changed & AllChanged || changed & NameChanged)
            e_network.setName(network.name());

        if(changed & AllChanged || changed & TypeChanged)
            e_network.setType(network.type());

        if(changed & AllChanged || changed & KnownChanged)
            e_network.setKnown(network.known());

        if(changed & AllChanged || changed & ConnectedChanged)
            e_network.setConnected(network.connected());

        if(changed & AllChanged || changed & StrengthChanged)
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
        this->addOrReplaceNetwork(network, StrengthChanged);
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
    qDebug() << "Visible network added" << stationId << "=" << name << "(connected=" << connected << ")";
    WifiNetwork network = WifiNetwork(stationId, name, type);
    network.setConnected(connected);
    this->addOrReplaceNetwork(network, ConnectedChanged);
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
    this->addOrReplaceNetwork(network, KnownChanged);
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

    /*QList<WifiNetwork> list = m_networks.values();
    std::sort(list.begin(), list.end(), [](const WifiNetwork &n1, const WifiNetwork &n2){
        return n1 > n2;
    });
    WifiNetwork network = list[index.row()];*/
    WifiNetwork network = m_networks.values().value(index.row());

    switch (role) {
        case IdRole:
            return QVariant(network.networkId());
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

void WifiModel::setWifiPassword(const QString &password)
{
    this->m_password = password;
    emit wifiPassword();
}

void WifiModel::setWifiUsernamePassword(const QString &username, const QString &password)
{
    this->m_username = username;
    this->m_password = password;
    emit wifiPassword();
}

QString WifiModel::onRequestPrivateKeyPassphrase(const QString &networkId)
{
    qDebug() << networkId << " requesting onRequestPrivateKeyPassphrase";
    emit requestPassword();

    QEventLoop loop;
    connect(this, SIGNAL(wifiPassword()), &loop, SLOT(quit()));
    loop.exec();

    return QString(this->m_password);
}
QString WifiModel::onRequestPassphrase(const QString &networkId)
{
    qDebug() << networkId << " requesting onRequestPassphrase";
    emit requestPassword();

    QEventLoop loop;
    connect(this, SIGNAL(wifiPassword()), &loop, SLOT(quit()));
    loop.exec();

    return QString(this->m_password);
}
QPair<QString, QString> WifiModel::onRequestUsernameAndPassword(const QString &networkId)
{
    qDebug() << networkId << " requesting onRequestUsernameAndPassword";
    emit requestUsernamePassword();

    QEventLoop loop;
    connect(this, SIGNAL(wifiPassword()), &loop, SLOT(quit()));
    loop.exec();

    return QPair(this->m_username,this->m_password);
}
QString WifiModel::onRequestUserPassword(const QString &username, const QString &networkId)
{
    emit requestPasswordForUsername(username);

    QEventLoop loop;
    connect(this, SIGNAL(wifiPassword()), &loop, SLOT(quit()));
    loop.exec();

    return QString(this->m_password);
}
