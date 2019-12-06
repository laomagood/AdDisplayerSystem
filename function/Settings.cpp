#include <QNetworkInterface>
#include <QHostInfo>
#include <QUuid>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include <QApplication>
#include "settings.h"
#include "util/Util.h"

#define DefaultBroadcastPort        66770
#define DefaultTransferPort         16160
#define DefaultBroadcastInterval    500         // 0.5 secs
#define DefaultFileBufferSize       98304       // 96 KB
#define MaxFileBufferSize           1024*1024   // 1 MB

Settings::Settings()
{
    m_thisDevice = Device();
    m_thisDevice.setId(getMacInfo());
    m_thisDevice.setAddress(QHostAddress::LocalHost);
    m_thisDevice.setOSName(OS_NAME);

    foreach (QHostAddress address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::LocalHost) {
            m_thisDevice.setAddress(address);
            break;
        }
    }

    loadSettings();
}

Settings::~Settings() {

}

void Settings::setDeviceName(const QString &name)
{
    m_thisDevice.setName(name);
}

void Settings::setBroadcastPort(quint16 port)
{
    if (port > 0)
        m_BCPort = port;
}

void Settings::setBroadcastInterval(quint16 interval)
{
    m_BCInterval = interval;
}

void Settings::setTransferPort(quint16 port)
{
    if (port > 0)
        m_transferPort = port;
}

void Settings::setFileBufferSize(qint32 size)
{
    if (size > 0 && size < MaxFileBufferSize)
        m_fileBuffSize = size;
}

void Settings::setDownloadDir(const QString& dir)
{
    if (!dir.isEmpty() && QDir(dir).exists())
        m_downloadDir = dir;
}

void Settings::setReplaceExistingFile(bool replace)
{
    m_replaceExistingFile = replace;
}

void Settings::loadSettings()
{
    QString strIniFilePath = Util::getIniFilePath();
    if (strIniFilePath.isEmpty())
        return;

    QSettings settings(strIniFilePath, QSettings::IniFormat);
    settings.beginGroup("Transfer");
    m_thisDevice.setName(settings.value("DeviceName", QHostInfo::localHostName()).toString());
    m_BCPort = settings.value("BroadcastPort", DefaultBroadcastPort).value<quint16>();
    m_transferPort = settings.value("TransferPort", DefaultTransferPort).value<quint16>();
    m_fileBuffSize = settings.value("FileBufferSize", DefaultFileBufferSize).value<quint32>();
    m_downloadDir = settings.value("DownloadDir", getDefaultDownloadPath()).toString();
    settings.endGroup();

    if (!QDir(m_downloadDir).exists()) {
        QDir dir;
        dir.mkpath(m_downloadDir);
    }

    m_BCInterval = settings.value("BroadcastInterval", DefaultBroadcastInterval).value<quint16>();
    m_replaceExistingFile = settings.value("ReplaceExistingFile", false).toBool();
}

QString Settings::getDefaultDownloadPath()
{
    return QCoreApplication::applicationDirPath()+ "/files/Source";
}

void Settings::saveSettings()
{
    QString strIniFilePath = Util::getIniFilePath();
    if (strIniFilePath.isEmpty())
        return;

    // 将配置文件的支持的后缀保存的这个类的每个变量中
    QSettings settings(strIniFilePath, QSettings::IniFormat);
    settings.beginGroup("Transfer");
    settings.setValue("DeviceName", m_thisDevice.getName());
    settings.setValue("BroadcastPort", m_BCPort);
    settings.setValue("TransferPort", m_transferPort);
    settings.setValue("FileBufferSize", m_fileBuffSize);
    settings.setValue("DownloadDir", m_downloadDir);
    settings.setValue("BroadcastInterval", m_BCInterval);
    settings.setValue("ReplaceExistingFile", m_replaceExistingFile);
    settings.endGroup();
}

void Settings::reset()
{
    m_thisDevice.setName(QHostInfo::localHostName());
    m_BCPort = DefaultBroadcastPort;
    m_transferPort = DefaultTransferPort;
    m_BCInterval = DefaultBroadcastInterval;
    m_fileBuffSize = DefaultFileBufferSize;
    m_downloadDir = getDefaultDownloadPath();
}

quint16 Settings::getBroadcastPort() const
{
    return m_BCPort;
}

quint16 Settings::getTransferPort() const
{
    return m_transferPort;
}

quint16 Settings::getBroadcastInterval() const
{
    return m_BCInterval;
}

qint32 Settings::getFileBufferSize() const
{
    return m_fileBuffSize;
}

QString Settings::getDownloadDir() const
{
    return m_downloadDir;
}

Device Settings::getMyDevice() const
{
    return m_thisDevice;
}

QString Settings::getDeviceId() const
{
    return m_thisDevice.getId();
}

QString Settings::getDeviceName() const
{
    return m_thisDevice.getName();
}

QHostAddress Settings::getDeviceAddress() const
{
    return m_thisDevice.getAddress();
}

bool Settings::getReplaceExistingFile() const
{
    return m_replaceExistingFile;
}

QString Settings::getMacInfo()
{
    QString info;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (int i=0; i<ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        // 过滤掉本地回环地址,没有开启的地址
        if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            // 过滤掉虚拟地址
            if (!(iface.humanReadableName().contains("VMware",Qt::CaseInsensitive))) {
                info = iface.hardwareAddress();
            }
        }
    }
    return info;
}
