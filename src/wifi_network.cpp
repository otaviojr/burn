#include <QString>
#include "wifi_network.h"

WifiNetwork::WifiNetwork()
{
    m_networkId = "";
    m_name = "";
    m_type = "";
    m_known = false;
    m_strength = 0;
    m_connected = false;
}

WifiNetwork::WifiNetwork(const QString &networkId, const QString &name, const QString &type)
{
    m_networkId = networkId;
    m_name = name;
    m_type = type;
    m_known = false;
    m_strength = 0;
    m_connected = false;
}

QString WifiNetwork::networkId() const
{
    return m_networkId;
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

bool WifiNetwork::connected() const
{
    return m_connected;
}

void WifiNetwork::setNetworkId(const QString& networkId)
{
    m_networkId = networkId;
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

void WifiNetwork::setConnected(const bool &connected)
{
    m_connected = connected;
}
