#include "mytcpsocket.h"

#include <QDataStream>
#include <QByteArray>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QThread>

MyTcpSocket::MyTcpSocket(QTcpSocket *tcpSocket, QObject *parent) :
    QObject(parent),
    p_tcpSocket(tcpSocket)
{
    // 当使用tcp发送一个很小的数据块时，接收方并不会立即收到,而是积累到一定字节后，才会触发相应的接收处理
//    p_tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, "1");    // 启用nagle低延迟算法
//    p_tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, "1");   // 设置保持连接状态

    initializeSendData();
    initializeReceiveData();
    handleEvents();
}

MyTcpSocket::~MyTcpSocket()
{
    p_tcpSocket->deleteLater();
    p_tcpSocket = nullptr;
}

QTcpSocket *MyTcpSocket::getTcpSocket() const
{
    if (p_tcpSocket != nullptr) {
        return p_tcpSocket;
    } else {
        return nullptr;
    }
}

bool MyTcpSocket::write(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "MyTcpSocket: file can't open";
        return false;
    }

    // 1.[总大小][类型：File][文件名长度][文件名][文件块数据][文件块数据]
    // 对于大文件必须拆分发送，QByteArray读大文件内存分配会不足
    QByteArray array;
    QDataStream out(&array, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_9);

    // [1] 首部[总大小][类型：File][文件名长度][文件名]--------------------
    // 无路径文件名
    QString sfName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);

    // 首部 = 总大小 + 类型 + 文件名长度 + 文件名
    out << qint64(0) << qint64(0) << qint64(0);
    // 总大小加上首部的大小
    m_sendTotalBytes = file.size();
    m_sendTotalBytes += array.size();
    m_sendTotalBytes += sfName.toUtf8().length();

    qDebug() << "array.size(): " << array.size();

    // 重写首部的前3个字段
    out.device()->seek(0);
    out << m_sendTotalBytes << qint64(this->FileType) << qint64(sfName.toUtf8().length());

    qDebug() << "总长度：   " << m_sendTotalBytes;
    qDebug() << "文件名长度 " << sfName.toUtf8().length();
    qDebug() << "文件长度： " << file.size();

    // 发送首部,这里文件名不用流一起发送（因为QByteArray到流中会增加4个字节的文件大小信息）
    m_sendRestBytes = m_sendTotalBytes - p_tcpSocket->write(array);
    m_sendRestBytes -= p_tcpSocket->write(sfName.toUtf8());

    qDebug() << "发送完首部后剩余大小：" << m_sendRestBytes;

    // [2] 文件块数据-------------------------------------------------
    while(true) {
        // 剩余大小判断
        qDebug() << "发送小块数据:" << "剩余大小:" << m_sendRestBytes;
        if(m_sendRestBytes > 0) {
            QByteArray buf = file.read(qMin(m_sendBlockBytes, m_sendRestBytes));
            m_sendRestBytes -= p_tcpSocket->write(buf);
            p_tcpSocket->waitForBytesWritten();
        } else {
            // 全部发送完
            file.close();
            initializeSendData();
            qDebug() << "数据全部发送完毕···";
            return true;
        }
    }
}

bool MyTcpSocket::write(const QJsonDocument &json)
{
    // 2.[总大小][类型：Json][文件数据]
    QByteArray array;
    QDataStream out(&array, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_9);

    QByteArray data = json.toJson(QJsonDocument::Indented);

    // 首部 = 总大小 + 类型
    out << qint64(0) << qint64(0);

    // 总大小加上首部的大小
    m_sendTotalBytes = data.length();
    qDebug() << "m_sendTotalBytes:" << m_sendTotalBytes;
    m_sendTotalBytes += array.size();
    qDebug() << "m_sendTotalBytes:" << m_sendTotalBytes;
    // 重写首部的前两个长度字段
    out.device()->seek(0);
    out << m_sendTotalBytes << qint64(this->JsonType);

    qDebug() << "总字节：" << m_sendTotalBytes << "类型：" << this->JsonType;
    // 发送
    p_tcpSocket->write(array);
    p_tcpSocket->write(data);

    initializeSendData();
    return true;
}

// 读取字节
void MyTcpSocket::read()
{
    if (p_tcpSocket->bytesAvailable() == 0) {
        return;
    }
    qDebug() << "接收数据:";
    QDataStream in(p_tcpSocket);
    in.setVersion(QDataStream::Qt_5_9);

    // [0] 获取文件大小与类型
    if (m_receiveTotalBytes == 0) {
        if(p_tcpSocket->bytesAvailable() >= 2*sizeof(quint64)) {
            m_receivedBytes = 2*sizeof(quint64);
            in >> m_receiveTotalBytes >> m_receiveTpty;
            qDebug() << "总大小" << m_receiveTotalBytes;
            qDebug() << "Type:" << m_receiveTpty;
        } else {
            return;
        }
    }

    // 根据类型判断为File或Json
    switch (m_receiveTpty) {
    case FileType:
    {
        qDebug() << "进入File类型判断";
        qDebug() << "总字节：" << m_receiveTotalBytes;
        qDebug() << "已接受：" << m_receivedBytes;
        qDebug() << "缓冲区：" << p_tcpSocket->bytesAvailable();

        // [1.1] 获取文件名长度
        if (m_receivedNameBytes == 0) {
            if(p_tcpSocket->bytesAvailable() >= sizeof(quint64)) {
                m_receivedBytes += sizeof(quint64);
                in >>  m_receivedNameBytes;
                qDebug() << "文件名长度" << m_receivedNameBytes;
            } else {
                return;
            }
        }
        // [1.2] 获取文件名
        if (m_strFileName.isEmpty()) {
            if(m_receivedNameBytes <= (unsigned)p_tcpSocket->bytesAvailable()) {
                m_receivedBytes += m_receivedNameBytes;
                // 保存文件名
                m_strFileName =QString::fromUtf8(p_tcpSocket->read(m_receivedNameBytes));
                qDebug() << "文件名：" << m_strFileName;
                qDebug() << "已接受：" << m_receivedBytes;
                qDebug() << "缓冲区：" << p_tcpSocket->bytesAvailable();

                // 获取当前的时间戳，设置下载的临时文件名称
                QDateTime dateTime = QDateTime::currentDateTime();
                QString date = dateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz");
                m_strTmpFileName = QDir::currentPath() + "/Download" + QString("/%1.tmp").arg(date);
                qDebug() << "临时文件:" << m_strTmpFileName;

                // 路径不存在，则创建
                QFileInfo fileInfo(m_strTmpFileName);

                QDir tmpDir(fileInfo.absolutePath());
                if (!tmpDir.exists()) {
                    tmpDir.mkpath(fileInfo.absolutePath());
                }
            } else {
                return;
            }
        }

        // [1.3] 剩余接收大小 > 缓存区 : 把全部读入
        if ((m_receiveTotalBytes - m_receivedBytes) > (unsigned)p_tcpSocket->bytesAvailable()) {
            m_receivedBytes += p_tcpSocket->bytesAvailable();
            // 写文件-形式为追加
            QFile file(m_strTmpFileName);
            if (file.open(QIODevice::Append))
                qDebug() << ">：写入字节数：" << file.write(p_tcpSocket->readAll());
            file.close();
        }
        // [1.4] 剩余接收大小 <= 缓存区 : 读取剩余大小
        else {
            // 写文件-形式为追加
            QFile file(m_strTmpFileName);
            if (file.open(QIODevice::Append))
                qDebug() << "<=：写入字节数：" << file.write(p_tcpSocket->read(m_receiveTotalBytes - m_receivedBytes));
            file.close();

            QFileInfo fileInfo(m_strTmpFileName);
            QFileInfo newFileInfo = fileInfo.absolutePath() + "/" +m_strFileName;

            // 判断文件是否已经存在重命名
            int i = 1;
            while (newFileInfo.exists()) {
                qDebug() << "文件名已经存在，现在重命名";
                int fileExtensionLenth = m_strFileName.size() - m_strFileName.lastIndexOf('.') - 1;

                newFileInfo = fileInfo.absolutePath() + "/" +
                        m_strFileName.left(m_strFileName.size() - fileExtensionLenth - 1) +
                        "(" + QString::number(i) + ")" + m_strFileName.right(fileExtensionLenth + 1);
                i++;
            }

            qDebug() << "最终文件" << newFileInfo.absoluteFilePath();
            // 将临时文件重命名
            QFile::rename(m_strTmpFileName, newFileInfo.absoluteFilePath());

            emit newFileData(newFileInfo.absoluteFilePath());
            initializeReceiveData();
        }
        break;
    }
    case JsonType:
    {
        qDebug() << "进入Json类型判断";
        qDebug() << "总字节：" << m_receiveTotalBytes;
        qDebug() << "已接受：" << m_receivedBytes;
        qDebug() << "缓冲区：" << p_tcpSocket->bytesAvailable();

        // [2.1] 剩余接收大小 > 缓存区 : 把全部读入
        if ((m_receiveTotalBytes - m_receivedBytes) > (unsigned)p_tcpSocket->bytesAvailable()) {
            qDebug() << "进入>状态";
            m_receivedBytes += p_tcpSocket->bytesAvailable();
            m_inBlock.append(p_tcpSocket->readAll());
        }
        // [2.2] 剩余接收大小 <= 缓存区 : 读取剩余大小
        else {
            qDebug() << "进入<=状态";
            m_inBlock.append(p_tcpSocket->read(m_receiveTotalBytes - m_receivedBytes));
            QJsonDocument doc = QJsonDocument::fromJson(m_inBlock);

            /* 测试Json接收是否正确
            QFile fileJson("E:/QtCode/QtStudy/AdDisplayerSystem/Code/xx.json");
            if (!fileJson.open(QIODevice::ReadWrite)) {
                return;
            }
            fileJson.write(doc.toJson());
            fileJson.close(); */

            emit newJsonData(doc);
            initializeReceiveData();
        }
        break;
    }
    default:
        qDebug() << "Type Error";
        break;
    }
}

// 初始化发送数据
void MyTcpSocket::initializeSendData()
{
    m_sendTotalBytes = 0;       // 总大小
    m_sentTpty = 0;             // 类型
    m_sendFileNameBytes = 0;    // 文件名大小
    m_sendFileBytes = 0;        // 文件大小
    m_sendRestBytes = 0;        // 还剩余大小
}

// 初始化接收数据
void MyTcpSocket::initializeReceiveData()
{
    m_receiveTotalBytes = 0;    // 总大小
    m_receiveTpty = 0;          // 类型
    m_receivedBytes = 0;        // 已接收文件大小
    m_receivedNameBytes = 0;    // 文件名大小
    m_strFileName = "";         // 接收的文件名
    m_strTmpFileName = "";      // 临时的文件名
    m_inBlock.clear();          // 接收缓存
}
// 信号与槽处理
void MyTcpSocket::handleEvents()
{
    connect(p_tcpSocket, &QTcpSocket::readyRead, [this] {
        read();
    });

    connect(p_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [this](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError);

        //        m_errorString = p_tcpSocket->errorString();
        //        m_errorCode = error;
        //        emit socketError(error, m_errorString);
    });
}
