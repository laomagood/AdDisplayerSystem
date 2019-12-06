#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include "util.h"
#include "function/Settings.h"
#include "Json.h"

QString Util::sizeToString(qint64 size)
{
    int count = 0;
    double f_size = size;
    while (f_size >= 1024) {
        f_size /= 1024;
        count++;
    }

    QString suffix;
    switch (count) {
    case 0 : suffix = " B"; break;
    case 1 : suffix = " KB"; break;
    case 2 : suffix = " MB"; break;
    case 3 : suffix = " GB"; break;
    case 4 : suffix = " TB"; break;
    }

    return QString::number(f_size, 'f', 2).append(suffix);
}

QVector< QPair<QString, QString> >
Util::getInnerDirNameAndFullFilePath(const QDir& startingDir, const QString& innerDirName)
{
    QVector< QPair<QString, QString> > pairs;

    QFileInfoList fiList = startingDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for (const auto& fi : fiList)
        pairs.push_back( QPair<QString, QString>(innerDirName, fi.filePath()) );

    fiList = startingDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const auto& fi : fiList) {
        QString newInnerDirName;
        if (innerDirName.isEmpty())
            newInnerDirName = fi.fileName();
        else
            newInnerDirName = innerDirName + QDir::separator() + fi.fileName();

        QVector< QPair<QString, QString> > otherPairs =
                getInnerDirNameAndFullFilePath( QDir(fi.filePath()), newInnerDirName );
        pairs.append(otherPairs);
    }
    return pairs;
}

QString Util::parseAppVersion(bool onlyVerNum)
{
    if (onlyVerNum) {
        return QString::number(PROGRAM_X_VER) + "." +
                QString::number(PROGRAM_Y_VER) + "." +
                QString::number(PROGRAM_Z_VER);
    }

    return "v " + QString::number(PROGRAM_X_VER) + "." +
            QString::number(PROGRAM_Y_VER) + "." +
            QString::number(PROGRAM_Z_VER) +
            " (" + QString(OS_NAME) + ")";
}

QString Util::getUniqueFileName(const QString& fileName, const QString& folderPath)
{
    int count = 1;
    QString originalFilePath = folderPath + "/" + fileName;
    QString fPath = originalFilePath;
    while (QFile::exists(fPath)) {
        QFileInfo fInfo(originalFilePath);
        QString baseName = fInfo.baseName() + " (" + QString::number(count) + ")";
        fPath = folderPath + "/" + baseName + "." + fInfo.completeSuffix();
        count++;
    }

    return fPath;
}

QString Util::getIniFilePath()
{
    // 获取配置文件中的文件夹路径config.ini
    QString strIniFilePath = QCoreApplication::applicationDirPath() + "/config" + "/config.ini";
    QFileInfo iniFileInfo(strIniFilePath);
    if(!iniFileInfo.isFile()) {
        qDebug() << "配置文件缺失";
        return QString();
    }

    return strIniFilePath;
}

QStringList Util::getADJsonFiles(QString fileName)
{
    QStringList fileList;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        qDebug() << "打开文件" << fileName;
        QByteArray byteArray = file.readAll();

        // 打开失败

        // [1] 检查是否为Json文件
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
        if (doucment.isNull() && (jsonError.error != QJsonParseError::NoError)) {
            qDebug() << "文件Json解析失败";
            return fileList;
        }

        // [2] 检查文件版本
        QString strJson(byteArray);
        Json nJson(strJson, false);
        QString nVersion = nJson.getString("Version");
        if (nVersion != "2019V01") {
            qDebug() << "版本错误";
            return fileList;
        }
        // [3] 其他检查····

        // 背景图片
        QString backgroundImage = nJson.getString("BackgroundImage");
        if (!backgroundImage.isEmpty()) {
            fileList.append(backgroundImage);
        }

        int nControlCount = nJson.getInt("ControlWidgetCount");
        for (int i=0; i<nControlCount; i++) {
            QString nControlWidget = "ControlWidgetList." + QString::number(i) + ".ControlWidget";
            QString nControlWidgetType = "ControlWidgetList." + QString::number(i) + ".Type";
            QString nWidgetType = nJson.getString(nControlWidgetType);

            if (nWidgetType == "ImageWidget" || nWidgetType == "VideoWidget" ||
                    nWidgetType == "MusicWidget" ) {
                QJsonDocument doc;
                doc.setObject(nJson.getJsonObject(nControlWidget));
                QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
                QString strJson(byteArray);
                Json nxJson(strJson, false);

                int size = nxJson.getInt("ListSize");
                if (size == 0)
                    break;
                for (int i=0; i<size; i++) {
                    QString file = "List." + QString::number(i);
                    fileList.append(nxJson.getString(file));
                }
            }
        }
    }
    return fileList;
}
