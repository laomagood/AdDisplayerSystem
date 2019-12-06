#ifndef RightBean_H
#define RightBean_H

#include <QString>

class RightBean
{
public:
    RightBean() : m_nId(-1) { }

    void setID(qlonglong id) { m_nId = id; }
    void setGenUser(bool bGenUser) { m_bGenUser = bGenUser; }
    void setManageUser(bool bManageUser) { m_bManageUser = bManageUser; }
    void setDeleteUser(bool bDeleteUser) { m_bDeleteUser = bDeleteUser; }

    void setGenRole(bool bGenRole) { m_bGenRole = bGenRole; }
    void setManageRole(bool bManageRole) { m_bManageRole = bManageRole; }
    void setDeleteRole(bool bDeleteRole) { m_bDeleteRole = bDeleteRole; }

    void setUploadMaterial(bool bUploadMaterial) { m_bUploadMaterial = bUploadMaterial; }
    void setCheckMaterial(bool bCheckMaterial) { m_bCheckMaterial = bCheckMaterial; }
    void setDeleteMaterial(bool bDeleteMaterial) { m_bDeleteMaterial = bDeleteMaterial; }

    void setUploadProgram(bool bUploadProgram) { m_bUploadProgram = bUploadProgram; }
    void setCheckProgram(bool bCheckProgram) { m_bCheckProgram = bCheckProgram; }
    void setDeleteProgram(bool bDeleteProgram) { m_bDeleteProgram = bDeleteProgram; }
    void setGenProgram(bool bGenProgram) { m_bGenProgram = bGenProgram; }
    void setReleaseProgram(bool bReleaseProgram) { m_bReleaseProgram = bReleaseProgram; }
    void setManageProgram(bool bManageProgram) { m_bManageProgram = bManageProgram; }

    qlonglong getID() const { return m_nId; }
    bool isGenUser() const { return m_bGenUser; }
    bool isManageUser() const { return m_bManageUser; }
    bool isDeleteUser() const { return m_bDeleteUser; }

    bool isGenRole() const { return m_bGenRole; }
    bool isManageRole() const { return m_bManageRole; }
    bool isDeleteRole() const { return m_bDeleteRole; }

    bool isUploadMaterial() const { return m_bUploadMaterial; }
    bool isCheckMaterial() const { return m_bCheckMaterial; }
    bool isDeleteMaterial() const { return m_bDeleteMaterial; }

    bool isUploadProgram() const { return m_bUploadProgram; }
    bool isCheckProgram() const { return m_bCheckProgram; }
    bool isDeleteProgram() const { return m_bDeleteProgram; }
    bool isGenProgram() const { return m_bGenProgram; }
    bool isReleaseProgram() const { return m_bReleaseProgram; }
    bool isManageProgram() const { return m_bManageProgram; }


private:
    qlonglong m_nId;
    bool m_bGenUser;                    // 创建用户
    bool m_bManageUser;                 // 管理用户
    bool m_bDeleteUser;                 // 删除用户

    bool m_bGenRole;                    // 创建角色
    bool m_bManageRole;                 // 管理角色
    bool m_bDeleteRole;                 // 删除角色

    bool m_bUploadMaterial;             // 上传素材
    bool m_bCheckMaterial;              // 审核素材
    bool m_bDeleteMaterial;             // 删除素材

    bool m_bUploadProgram;              // 上传节目
    bool m_bCheckProgram;               // 审核节目
    bool m_bDeleteProgram;              // 删除节目
    bool m_bGenProgram;                 // 创建节目
    bool m_bReleaseProgram;             // 发布节目
    bool m_bManageProgram;              // 管理节目

};

#endif // RightBean_H
