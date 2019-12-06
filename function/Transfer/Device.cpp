#include "device.h"
#include <QNetworkInterface>

Device::Device(const QString &name, const QString &oSName, const QString &id, const QString &remarks, const QHostAddress &addr)
    : m_name{name}, m_oSName{oSName}, m_id{id}, m_remarks{remarks}, m_address{addr}
{
//    m_timer.setInterval(10000);
//    m_timer.start();
//    connect(m_timer, &QTimer::timeout, this, &onTimeout());
}

bool Device::isValid() const
{
    return (m_id != "" && m_name != "" && m_oSName != "" && m_address != QHostAddress::Null);
}

void Device::setName(const QString &name)
{
    m_name = name;
}

void Device::setConnectionStatus(const ConnectionStatus &status)
{
    m_connectionStatus = status;
    // 状态为在线，重新启动定时器
    if (status == ConnectionStatus::Online) {
//        m_timer.start();
    }
}

void Device::setOSName(const QString &osName)
{
    m_oSName = osName;
}

void Device::setId(const QString &id)
{
    m_id = id;
}

void Device::setProgram(const QString &program)
{
    m_program = program;
}

void Device::setRemarks(const QString &remarks)
{
    m_remarks = remarks;
}

void Device::setAddress(const QHostAddress &address)
{
    m_address = address;
}

bool Device::operator==(const Device& other) const
{
    return m_id == other.getId() && m_name == other.getName() && m_address == other.getAddress();
}

bool Device::operator!=(const Device& other) const
{
    return !((*this) == other);
}

void Device::onTimeout()
{
    m_connectionStatus = ConnectionStatus::Offline;
}
