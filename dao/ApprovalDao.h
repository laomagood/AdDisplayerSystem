#ifndef ApprovalDao_H
#define ApprovalDao_H

#include "global.h"
#include "gui/MaterialManagement/MaterialActionBarWidget.h"
#include "bean/MaterialBean.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class ApprovalBean;

class ApprovalDao
{
public:
    ApprovalBean findById(qlonglong id);
    ApprovalBean findByMaterialId(qlonglong id);
    QList<ApprovalBean> findAll();
    QList<ApprovalBean> findBy(QString name, MaterialActionBarWidget::ArrangeType arrangeType);         // 模糊查询，排列类型
    QList<ApprovalBean> findBy(FileType fileType, MaterialActionBarWidget::ArrangeType arrangeType,     // 文件类型，排列类型
                               MaterialActionBarWidget::ApprovalType approvalType = MaterialActionBarWidget::ApprovalAll);

    bool isExistName(QString name);                                                                     // 查询是否存在
    bool insert(const ApprovalBean &approvalBean);                                                      // 插入(数据库使用触发器)
    bool updateBy(const ApprovalBean &approvalBean);                                                    // 更新审核状态,审核时间
    bool deleteById(qlonglong id);                                                                      // 删除(数据库使用触发器)
};

#endif // ApprovalDao_H
