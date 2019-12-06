#ifndef MaterialBean_H
#define MaterialBean_H

#include <QString>
#include <QDateTime>
#include "global.h"


// 文件类型列枚举变量
enum class FileType : int { AllType = 0, ImageType, VideoType, AudioType,
                DocumentType, OtherType, VoidType, ProgramType };

class MaterialBean
{
public:
    MaterialBean() : m_nId(-1), m_bCheck(false), m_nGenerateTime(QDateTime::currentDateTime()) { }

    void setId(qlonglong id) { m_nId = id; }
    void setCheck(const bool check) { m_bCheck = check; }
    void setName(const QString name) { m_strName = name; }
    void setGenerateTime(qint64 time) { m_nGenerateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setFrequency(int frequency) { m_nFrequency = frequency; }
    void setFileType(FileType fileType) { m_nFileType = fileType; }
    void setFileType(qlonglong fileType) { m_nFileType = (FileType)fileType; }
    void setSize(qlonglong size) { m_nSize = size; }

    qlonglong getID() const { return m_nId; }
    bool getCheck() const { return m_bCheck; }
    QString getName() const { return m_strName; }
    qint64 getGenerateTime() const { return m_nGenerateTime.toSecsSinceEpoch(); }
    QString getStrGenerateTime() const {
        if (m_nGenerateTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nGenerateTime.toString("yyyy-MM-dd hh:mm:ss"); }

    int getFrequency() const { return m_nFrequency; }
    FileType getFileType() const { return m_nFileType; }
    FileType getFileTypeInt() const { return (FileType)m_nFileType; }
    qlonglong getSize() const { return m_nSize; }

private:
    qlonglong m_nId;
    bool m_bCheck;
    QString m_strName;
    QDateTime m_nGenerateTime;
    int m_nFrequency;
    FileType m_nFileType;
    qlonglong m_nSize;
};

#endif // MaterialBean_H
