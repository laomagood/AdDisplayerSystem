#include "filecopy.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

FileCopy::FileCopy(QObject *parent) : QObject(parent)
{
    m_createfile = new QDir();
}

FileCopy::~FileCopy()
{
    if(m_createfile) {
        m_createfile = Q_NULLPTR;
        delete m_createfile;
    }
}

bool FileCopy::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    bool exist = m_createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            m_createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir)) {
        return false;
    }
    return true;
}

bool FileCopy::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    qDebug() << "copyDirectoryFiles:" << fromDir << toDir;
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath())) {
            return false;
        }
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();

    if(m_firstRead) {
        int isfileTMP = 0;
        qDebug() << "a copyDirectoryFiles:" << fileInfoList.count();
        foreach(QFileInfo fileInfo, fileInfoList){
            if(fileInfo.isFile()) {
                isfileTMP++;
            }
        }
        m_total = fileInfoList.count() - 2 - isfileTMP; // 2为.和..
        m_value = 0;
        m_firstRead = false;
        qDebug() << "a copyDirectoryFiles:" << fileInfoList.count() <<m_total << isfileTMP;
        emit sigCopyDirStation(m_value/m_total);
        if(m_value == m_total) {
            m_firstRead = true;
            emit sigCopyDirStation(1);
            emit sigCopyDirOver();
        }
    } else {
        m_value++;
        if(m_value == m_total) {
            m_firstRead = true;
            emit sigCopyDirOver();
        }
        qDebug() << "a copyDirectoryFiles:" << m_value <<m_total;
        emit sigCopyDirStation(m_value/m_total);
    }

    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
            continue;
        }
        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), coverFileIfExist)) {
                return false;
            }
        } else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}
