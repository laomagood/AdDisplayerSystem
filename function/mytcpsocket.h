#ifndef MyTcpSocket_H
#define MyTcpSocket_H


#include <QTcpSocket>
#include <QJsonDocument>
#include <QFile>

/**
 * @brief The MyTcpSocket class
 * TCP接收与发送Json与文件
 * 1:[总大小][类型：File][文件名长度][文件名][文件数据]
 * 2:[总大小][类型：Json][文件数据]
 */

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    enum SendType {
        FileType = 1,
        JsonType
    };

    explicit MyTcpSocket(QTcpSocket *tcpSocket, QObject *parent = 0);
    ~MyTcpSocket();

    QTcpSocket* getTcpSocket() const;

    // 发送
    bool write(const QString &filePath);        // 发送File(传入文件的绝对路径)
    bool write(const QJsonDocument &json);      // 发送Json
    // 接收
    void read();
    void networkError(QAbstractSocket::SocketError error);    // 网络错误

signals:
    void newJsonData(QJsonDocument doc);        // 新Json
    void newFileData(QString filePath);         // 新File


private:
    // 接收文件
    quint64 m_receiveTotalBytes;    // 总大小
    quint64 m_receiveTpty;          // 类型
    quint64 m_receivedBytes;        // 已接收大小
    quint64 m_receivedNameBytes;    // 文件名大小
    QString m_strFileName;          // 接收的文件名
    QString m_strTmpFileName;       // 临时的文件名
    QByteArray m_inBlock;           // 接收缓存

    // 发送文件
    quint64 m_sendTotalBytes;       // 总大小
    quint64 m_sentTpty;             // 类型
    quint64 m_sendRestBytes;        // 剩余大小
    quint64 m_sendFileNameBytes;    // 文件名大小
    quint64 m_sendFileBytes;        // 文件大小

    const quint64 m_sendBlockBytes = 4*1024;    // 文件发送块大小4个字节

    QString m_errorString;          // 错误信息
    int m_errorCode;                // 错误代码

    void initializeSendData();      // 初始化发送数据
    void initializeReceiveData();   // 初始化接收数据
    void handleEvents();            // 信号与槽处理

    QTcpSocket *p_tcpSocket;
};

#endif // MyTcpSocket_H
