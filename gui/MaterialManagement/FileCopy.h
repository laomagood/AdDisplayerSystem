#ifndef FileCopy_H
#define FileCopy_H

#include <QObject>
#include <QDir>

class FileCopy : public QObject
{
    Q_OBJECT
public:
    explicit FileCopy(QObject *parent = 0);
    ~FileCopy();

    //拷贝文件：
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    bool copyFilesToPath(QStringList filePahts, QString toDir);

    //拷贝文件夹：
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
signals:
    void sigCopyDirStation(float num);
    void sigCopyDirOver();
private:
    QDir * m_createfile = Q_NULLPTR;
    float m_total = 0;
    float m_value = 0;
    bool m_firstRead = true;
};

#endif // FileCopy_H
