#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QDir>
#include <QVector>
#include <QPair>

/**
 * @brief The Util class
 * 工具类函数（静态）
 */

class Util
{
public:
    // 文件大小转String
    static QString sizeToString(qint64 size);
    // 获取内部目录名和完整文件路径
    static QVector< QPair<QString, QString> >
        getInnerDirNameAndFullFilePath(const QDir& startingDir, const QString& innerDirName);
    // 获取app版本
    static QString parseAppVersion(bool onlyVerNum = true);
    // 获取唯一文件名
    static QString getUniqueFileName(const QString& fileName, const QString& folderPath);
    // 获取配置文件路径
    static QString getIniFilePath();
    // 获取保存的Json文件中所有外部文件
    static QStringList getADJsonFiles(QString fileName);
};

#endif // UTIL_H
