#include <QString>
#include "wifi_network.h"

WifiNetwork::WifiNetwork()
{
    m_id = "";
    m_networkId = "";
    m_stationId = "";
    m_name = "";
    m_type = "";
    m_known = false;
    m_strength = 0;
}

WifiNetwork::WifiNetwork(const QString &id, const QString &name, const QString &type)
{
    m_id = id;
    m_name = name;
    m_type = type;
    m_networkId = "";
    m_stationId = "";
    m_known = false;
    m_strength = 0;
}

QString WifiNetwork::id() const
{
    return m_id;
}

QString WifiNetwork::networkId() const
{
    return m_networkId;
}

QString WifiNetwork::stationId() const
{
    return m_stationId;
}

QString WifiNetwork::name() const
{
    return m_name;
}

QString WifiNetwork::type() const
{
    return m_type;
}

bool WifiNetwork::known() const
{
    return m_known;
}

float WifiNetwork::strength() const
{
    return m_strength;
}

void WifiNetwork::setId(const QString& id)
{
    m_id = id;
}

void WifiNetwork::setNetworkId(const QString& networkId)
{
    m_networkId = networkId;
}

void WifiNetwork::setStationId(const QString& stationId)
{
    m_stationId = stationId;
}

void WifiNetwork::setName(const QString& name)
{
    m_name = name;
}

void WifiNetwork::setType(const QString& type)
{
    m_type = type;
}

void WifiNetwork::setKnown(const bool &known)
{
    m_known = known;
}

void WifiNetwork::setStrength(const float& strength)
{
    m_strength = strength;
}
