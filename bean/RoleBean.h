#ifndef RoleBean_H
#define RoleBean_H

#include <QString>
#include <QDateTime>
#include "RightBean.h"

class RoleBean
{
public:
    RoleBean() : m_nId(-1), m_bCheck(false),
        m_nGenerateTime(QDateTime::currentDateTime()), m_nRightId(-1) { }

    void setId(qlonglong id) { m_nId = id; }
    void setCheck(const bool check) { m_bCheck = check; }
    void setName(const QString name) { m_strName = name; }
    void setGenerateTime(qint64 time) { m_nGenerateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setUpdateTime(qint64 time) { m_nUpdateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setDescription(const QString description) { m_strDescription = description; }
    void setRightId(qlonglong rightId) { m_nRightId = rightId; }

    qlonglong getID() const { return m_nId; }
    bool getCheck() const { return m_bCheck; }
    QString getName() const { return m_strName; }
    qint64 getGenerateTime() const { return m_nGenerateTime.toSecsSinceEpoch(); }
    qint64 getUpdateTime() const { return m_nUpdateTime.toSecsSinceEpoch(); }

    QString getStrGenerateTime() const {
        if (m_nGenerateTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nGenerateTime.toString("yyyy-MM-dd hh:mm:ss"); }
    QString getStrUpdateTime() const {
        if (m_nUpdateTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nUpdateTime.toString("yyyy-MM-dd hh:mm:ss"); }

    QString getDescription() const { return m_strDescription; }
    qlonglong getRightId() const { return m_nRightId; }

private:
    qlonglong m_nId;
    bool m_bCheck;
    QString m_strName;
    QDateTime m_nGenerateTime;
    QDateTime m_nUpdateTime;
    QString m_strDescription;
    qlonglong m_nRightId;
};

#endif // RoleBean_H
