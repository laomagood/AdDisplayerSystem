#include "CompositeFile.h"
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <windows.h>

CompositeFile::CompositeFile(QObject *parent) : QObject(parent),
    m_fileTotalBytes(0),
    m_readWriteBytes(0)
{
    QString fileName("C:/Users/Technical/Desktop/temp.txt");
    QString tmpFilePath("C:/Users/Technical/Desktop/temp");
    QStringList files;

//    QString file04("D:/QtStudy/xx.txt");
//    files << file04;

    QString file01("C:/Users/Technical/Videos/春天的信使.mp4");
    QString file02("C:/Users/Technical/Videos/猫咪.mp4");
    QString file03("C:/Users/Technical/Videos/小猪佩奇.mp4");
    files << file01 << file02 << file03;

    write(fileName, files);
    read(fileName, tmpFilePath);
}

CompositeFile::~CompositeFile()
{

}


bool CompositeFile::write(QString saveFileName, QStringList files)
{
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::ReadWrite)) {
        qDebug() << "CompositeFile: file can't open";
        return false;
    }

    QDataStream inDataStream(&saveFile);
    inDataStream.setVersion(QDataStream::Qt_5_9);

    // [1] 文件魔术字
    inDataStream << MAGIC;

    // [2] [文件数]-[文件大小][文件名长度][文件名]-[文件大小][文件名长度][文件名]...[文件块数据][文件块数据]
    // 文件数
    inDataStream << quint64(files.size());

    // [文件大小][文件名长度][文件名]
    for (int i=0; i<files.size(); i++) {
        QFileInfo fileInfo(files.at(i));
        qint64 fileSize = fileInfo.size();
        qint64 fileNameLength = fileInfo.fileName().toUtf8().length();

        inDataStream << fileSize << fileNameLength;
        inDataStream << fileInfo.fileName().toUtf8();

        m_fileTotalBytes += fileSize;
    }

    qDebug() << "总文件大小:" << m_fileTotalBytes;

    // [3] 文件块数据-------------------------------------------------
    for (int i=0; i<files.size(); i++) {
        QFileInfo fileInfo(files.at(i));
        qint64 fileSize = fileInfo.size();
        qint64 remainSize = fileSize;

        QFile file(files.at(i));
        if (file.open(QIODevice::ReadOnly)) {
            while(true) {
                if(remainSize > 0) {
                    QByteArray buf = file.read(qMin(BLOCK_BYTES, remainSize));
                    inDataStream << buf;
                    remainSize -= buf.size();
                    m_readWriteBytes += buf.size();
                    updateProgress();
                } else {
                    file.close();
                    qDebug() << "写入完毕···";
                    break;
                }
            }
        }
    }
    initializeData();
    emit writeFinished();
    return true;
}

bool CompositeFile::read(QString filePath, QString tmpFileDir)
{
    // 创建临时文件夹（隐藏）
    QDir tmpDir(tmpFileDir);
    if (!tmpDir.exists()) {
        tmpDir.mkpath(tmpFileDir);
    }

    // 将文件夹设置为隐藏
//    SetFileAttributes((LPCWSTR)strTmpDir.unicode(),FILE_ATTRIBUTE_HIDDEN);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "CompositeFile: file can't open";
        return false;
    }

    QDataStream outDataStream(&file);
    outDataStream.setVersion(QDataStream::Qt_5_9);

    // [1] 文件魔术字
    qint64 magic;
    outDataStream >> magic;
    if (magic != MAGIC) {
        qDebug() << "文件错误，打开失败";
        return false;
    }

    // [2] [文件数]-[文件大小][文件名长度][文件名]-[文件大小][文件名长度][文件名]...[文件块数据][文件块数据]
    // 文件数
    qint64 filesNumber;
    outDataStream >> filesNumber;

    // [文件大小][文件名长度][文件名]
    QList<qint64> listFileSize;
    QStringList files;
    for (int i=0; i<filesNumber; i++) {
        qint64 fileSize, fileNameLength;
        outDataStream >> fileSize;
        outDataStream >> fileNameLength;
        listFileSize.append(fileSize);
        m_fileTotalBytes += fileSize;

        QString fileName;
        QByteArray buf(fileNameLength, ' ');
        outDataStream >> buf;
        fileName = tmpFileDir + "/" + (QString)buf;

        // 创建文件
        QFile file(fileName);
        file.open( QIODevice::ReadWrite);
        file.close();

        files.append(fileName);
    }

    qDebug() << "总文件大小:" << m_fileTotalBytes;

    // [3] 文件块数据-------------------------------------------------
    for (int i=0; i<filesNumber; i++) {
        qint64 remainSize = listFileSize.at(i);
        QFile file(files.at(i));
        file.open( QIODevice::WriteOnly);
        while (true) {
            if(remainSize > 0) {
                QByteArray buf(qMin(BLOCK_BYTES, remainSize), ' ');
                outDataStream >> buf;
                file.write(buf);
                remainSize -= buf.size();
                m_readWriteBytes += buf.size();
                updateProgress();
            } else {
                file.close();
                qDebug() << "读取完毕···";
                break;
            }
        }
    }
    initializeData();
    emit readFinished();
    return true;
}

// 初始化发送数据
void CompositeFile::initializeData()
{
    m_fileTotalBytes = 0;
    m_readWriteBytes = 0;
}

// 信号与槽处理
void CompositeFile::handleEvents()
{

}

void CompositeFile::updateProgress()
{
    qDebug() << "总大小:" << m_fileTotalBytes
             << "已读写:" << m_readWriteBytes;

    int progress = (m_readWriteBytes / m_fileTotalBytes *1.0) * 100;
    qDebug() << "进度:" << progress;
//    emit updateReadWriteProgress(progress);
}
