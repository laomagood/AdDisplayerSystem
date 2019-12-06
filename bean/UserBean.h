#ifndef UserBean_H
#define UserBean_H

#include <QString>
#include <QDateTime>

class UserBean
{
public:
    UserBean() : m_nId(-1), m_bCheck(false), m_nGenerateTime(QDateTime::currentDateTime()) { }

    void setId(qlonglong id) { m_nId = id; }
    void setCheck(const bool check) { m_bCheck = check; }
    void setName(const QString name) { m_strName = name; }
    void setPassword(const QString password) { m_strPassword = password; }
    void setGenerateTime(qint64 time) { m_nGenerateTime = QDateTime::fromSecsSinceEpoch(time); }
    void setLoginTime(qint64 time) { m_nLoginTime = QDateTime::fromSecsSinceEpoch(time); }
    void setLastLoginTime(qint64 time) { m_nLastLoginTime = QDateTime::fromSecsSinceEpoch(time); }
    void setRoleId(int roleId) { m_nRoleId = roleId; }
    void setRoleName(QString roleName) { m_RoleName = roleName; }

    qlonglong getID() const { return m_nId; }
    bool getCheck() const { return m_bCheck; }
    QString getName() const { return m_strName; }
    QString getPassword() const { return m_strPassword; }
    qint64 getGenerateTime() const { return m_nGenerateTime.toSecsSinceEpoch(); }
    qint64 getLoginTime() const { return m_nLoginTime.toSecsSinceEpoch(); }
    qint64 getLastLoginTime() const { return m_nLastLoginTime.toSecsSinceEpoch(); }

    QString getStrGenerateTime() const {
       if (m_nGenerateTime.toSecsSinceEpoch() == 0)
           return "";
        return m_nGenerateTime.toString("yyyy-MM-dd hh:mm:ss"); }
    QString getStrLoginTime() const {
        if (m_nLoginTime.toSecsSinceEpoch() == 0)
            return "";
         return m_nLoginTime.toString("yyyy-MM-dd hh:mm:ss"); }
    QString getStrLastLoginTime() const {
        if (m_nLastLoginTime.toSecsSinceEpoch() == 0)
            return "";
         return m_nLastLoginTime.toString("yyyy-MM-dd hh:mm:ss"); }
    int getRoleId() const { return m_nRoleId; }
    QString getRoleName() const { return m_RoleName; }

private:
    qlonglong m_nId;
    bool m_bCheck;
    QString m_strName;
    QString m_strPassword;
    QDateTime m_nGenerateTime;
    QDateTime m_nLoginTime;
    QDateTime m_nLastLoginTime;
    int m_nRoleId;
    QString m_RoleName;
};

#endif // UserBean_H
