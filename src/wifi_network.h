#ifndef __WIFI_NETWORK_H__
#define __WIFI_NETWORK_H__

#include <QObject>

class WifiNetwork : public QObject
{
    Q_OBJECT
public:
    WifiNetwork(const QString &id, const QString &name, const QString &type);
    WifiNetwork();

    QString id() const;
    QString networkId() const;
    QString stationId() const;
    QString name() const;
    QString type() const;
    bool known() const;
    float strength() const;
    bool connected() const;

    void setId(const QString& id);
    void setNetworkId(const QString& networkId);
    void setStationId(const QString& stationId);
    void setName(const QString& name);
    void setType(const QString& type);
    void setKnown(const bool &known);
    void setStrength(const float& strength);
    void setConnected(const bool& connected);

    bool operator<(const WifiNetwork& n) const {
      return this->connected() || this->strength() < n.strength();
    }

    bool operator>(const WifiNetwork& n) const {
      return n.connected() || this->strength() > n.strength();
    }

private:
    QString m_id;
    QString m_networkId;
    QString m_stationId;
    QString m_type;
    QString m_name;
    bool m_known;
    float m_strength;
    bool m_connected;
};

#endif //__WIFI_NETWORK_H__
