#ifndef __WIFI_NETWORK_H__
#define __WIFI_NETWORK_H__

class WifiNetwork
{
public:
    WifiNetwork(const QString &networkId, const QString &name, const QString &type);
    WifiNetwork();

    QString networkId() const;
    QString name() const;
    QString type() const;
    bool known() const;
    float strength() const;
    bool connected() const;

    void setNetworkId(const QString& networkId);
    void setName(const QString& name);
    void setType(const QString& type);
    void setKnown(const bool &known);
    void setStrength(const float& strength);
    void setConnected(const bool& connected);

private:
    QString m_networkId;
    QString m_type;
    QString m_name;
    bool m_known;
    float m_strength;
    bool m_connected;
};

#endif //__WIFI_NETWORK_H__
