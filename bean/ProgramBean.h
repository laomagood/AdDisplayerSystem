#ifndef ProgramBean_H
#define ProgramBean_H

#include <QString>
#include <QDateTime>
#include "global.h"

class ProgramBean
{
public:
    ProgramBean() : m_nId(-1), m_bCheck(false), m_nGenerateTime(QDateTime::currentDateTime()) { }

    void setId(qlonglong id) { m_nId = id; }
    void setCheck(const bool check) { m_bCheck = check; }
    void setName(const QString name) { m_strName = name; }
    void setGenerateTime(qint64 time) { m_nGenerateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setFrequency(int frequency) { m_nFrequency = frequency; }
    void setSize(qlonglong size) { m_nSize = size; }

    inline qlonglong getID() const { return m_nId; }
    inline bool getCheck() const { return m_bCheck; }
    inline QString getName() const { return m_strName; }
    inline qint64 getGenerateTime() const { return m_nGenerateTime.toSecsSinceEpoch(); }
    inline QString getStrGenerateTime() const {
        if (m_nGenerateTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nGenerateTime.toString("yyyy-MM-dd hh:mm:ss"); }

    inline int getFrequency() const { return m_nFrequency; }
    inline qlonglong getSize() const { return m_nSize; }

private:
    qlonglong m_nId;                            // ID
    bool m_bCheck;                              // 审核
    QString m_strName;                          // 节目名称
    QDateTime m_nGenerateTime;                  // 创建时间
    int m_nFrequency;                           // 使用频率
    QStringList m_nFilePaths;                   // 文件列表
    qlonglong m_nSize;                          // 文件大小
};

#endif // ProgramBean_H
