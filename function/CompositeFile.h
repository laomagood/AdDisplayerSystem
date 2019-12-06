#ifndef CompositeFile_H
#define CompositeFile_H

#include <QJsonDocument>
#include <QFile>

/**
 * @brief The CompositeFile class
 * 将多个文件合成一个复合文件or一个复合文件分解成多个文件(生成一个临时隐藏文件夹，使用后删除)
 * 将这个类放在其他线程中，以免堵塞UI线程
 * 可以发送信号更新进度，有完成的信号
 * 文件格式
 * [魔术数字][文件数] [文件大小][文件名长度][文件名] [文件数据][文件数据][文件数据]...
 */

class CompositeFile : public QObject
{
    Q_OBJECT
public:
    explicit CompositeFile(QObject *parent = 0);
    ~CompositeFile();

    bool write(QString filePath, QStringList files);    // 写入文件:1.文件路径 2.文件列表
    bool read(QString filePath, QString tmpFileDir);    // 读取文件:1.文件路径 2.解压文件夹

signals:
    void writeFinished();                               // 写入完毕
    void readFinished();                                // 读取完毕
    void updateReadWriteProgress(int progress);         // 更新读写进度

private:
    void initializeData();                              // 初始化数据
    void handleEvents();                                // 信号与槽处理
    void updateProgress();                              // 更新进度

    qint64 m_fileTotalBytes;                            // 总文件大小
    qint64 m_readWriteBytes;                            // 已读已写大小

    const qint64 BLOCK_BYTES = 4*1024;                  // 文件块大小4个字节
    const qint64 MAGIC = 0xA0B0C0D0A0B0C0D0;            // 魔术数字
};

#endif // CompositeFile_H
