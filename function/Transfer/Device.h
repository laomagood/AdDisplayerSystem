#ifndef DEVICE_H
#define DEVICE_H

#include <QtNetwork/QHostAddress>
#include <QObject>
#include <QTimer>

enum class ConnectionStatus : int {
    Online,
    Offline
};

/**
 * @brief The Device class
 * 保存了设备的相关信息：名称，系统，ID，连接状态，备注，QHostAddress
 */
class Device
{
public:
    explicit Device() = default;
    Device(const QString &name, const QString &oSName, const QString &id, const QString &remarks, const QHostAddress &addr);

    inline QString getName() const { return m_name; }
    inline ConnectionStatus  getStatus() const { return m_connectionStatus; }
    inline QString getOSName() const { return m_oSName; }
    inline QString getId() const { return m_id; }
    inline QString getProgram() const { return m_program; }
    inline QString getRemarks() const { return m_remarks; }
    inline QHostAddress getAddress() const { return m_address; }

    bool isValid() const;

    void setName(const QString& name);
    void setConnectionStatus(const ConnectionStatus& status);
    void setOSName(const QString& osName);
    void setId(const QString& id);
    void setProgram(const QString& program);
    void setRemarks(const QString& remarks);
    void setAddress(const QHostAddress& address);

    bool operator==(const Device& other) const;
    bool operator!=(const Device& other) const;

private:
    void onTimeout();                   // 定时器超时

private:
    QString m_name = "";
    ConnectionStatus m_connectionStatus = ConnectionStatus::Offline;
    QString m_oSName = "";
    QString m_id = "";
    QString m_program = "";
    QString m_remarks = "";
    QHostAddress m_address = QHostAddress::Null;

//    QTimer m_timer;
};

#endif // DEVICE_H
