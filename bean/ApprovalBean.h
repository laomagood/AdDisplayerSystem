#ifndef ApprovalBean_H
#define ApprovalBean_H

#include <QString>
#include <QDateTime>
#include "bean/MaterialBean.h"
#include "bean/UserBean.h"

class ApprovalBean
{
public:
    // 审核类型
    enum ApprovalType { ApprovalNot=0, ApprovalFail, ApprovalPass };

    ApprovalBean() : m_nId(-1), m_nApprovalTime(QDateTime::currentDateTime()), m_approvalType(ApprovalNot) { }
    void setId(qlonglong id) { m_nId = id; }
    void setUserId(qlonglong id) { m_nUserId = id; }
    void setMaterialId(qlonglong id) { m_nMaterialId = id; }
    void setApprovalTime(qint64 time) { m_nApprovalTime = QDateTime::fromSecsSinceEpoch(time); }
    void setApprovalType(qlonglong approvalType) { m_approvalType = (ApprovalType)approvalType; }
    void setApprovalType(ApprovalType approvalType) { m_approvalType = approvalType; }
    void setMaterialBean(const MaterialBean& materialBean) { m_materialBean = materialBean; }
    void setUserBean(const UserBean& userBean) { m_userBean = userBean; }

    qlonglong getID() const { return m_nId; }
    qlonglong getUserID() const { return m_nUserId; }
    qlonglong getMaterialID() const { return m_nMaterialId; }
    qint64 getApprovalTime() const { return m_nApprovalTime.toSecsSinceEpoch(); }
    QString getStrApprovalTime() const {
        if (m_nApprovalTime.toSecsSinceEpoch() == 0)
            return "";
        return m_nApprovalTime.toString("yyyy-MM-dd hh:mm:ss"); }
    ApprovalType getApprovalType() const { return m_approvalType; }
    MaterialBean& getMaterialBean() { return m_materialBean; }
    UserBean& getUserBean() {  return m_userBean; }

private:
    qlonglong m_nId;
    qlonglong m_nUserId;
    qlonglong m_nMaterialId;
    QDateTime m_nApprovalTime;
    ApprovalType m_approvalType;
    MaterialBean m_materialBean;
    UserBean m_userBean;
};

#endif // ApprovalBean_H
