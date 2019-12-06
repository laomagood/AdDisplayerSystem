#ifndef OrderBean_H
#define OrderBean_H

#include <QString>
#include <QDateTime>
#include "global.h"

class OrderBean
{
public:
    OrderBean() : m_nId(-1), m_nGenerateTime(QDateTime::currentDateTime()) { }

    void setId(qlonglong id) { m_nId = id; }
    void setName(const QString name) { m_strName = name; }
    void setGenerateTime(qint64 time) { m_nGenerateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setFrequency(int frequency) { m_nFrequency = frequency; }

    qlonglong getID() const { return m_nId; }
    QString getName() const { return m_strName; }
    qint64 getGenerateTime() const { return m_nGenerateTime.toSecsSinceEpoch(); }
    QString getStrGenerateTime() const {
        if (m_nGenerateTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nGenerateTime.toString("yyyy-MM-dd hh:mm:ss"); }

    int getFrequency() const { return m_nFrequency; }

private:
    qlonglong m_nId;                            // 指令ID
    QString m_strName;                          // 指令名称
    QDateTime m_nGenerateTime;                  // 创建时间
    QStringList m_nFilePaths;                   // 文件列表
};

#endif // OrderBean_H
