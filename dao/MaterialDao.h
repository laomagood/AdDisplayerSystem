#ifndef MaterialDao_H
#define MaterialDao_H

#include "gui/MaterialManagement/MaterialActionBarWidget.h"
#include "bean/MaterialBean.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class MaterialBean;

class MaterialDao
{
public:
    MaterialBean findById(qlonglong id);
    QList<MaterialBean> findAll();
    QList<MaterialBean> findBy(QString name, MaterialActionBarWidget::ArrangeType arrangeType);            // 模糊查询，排列类型
    QList<MaterialBean> findBy(FileType fileType, MaterialActionBarWidget::ArrangeType arrangeType);       // 文件类型，排列类型
    bool isExistName(QString name);         // 查询是否存在

    bool insert(const MaterialBean &materialBean);
    bool updateBy(const MaterialBean &materialBean);
    bool updateBy(const int &id, const QString &name);
    bool updateBy(const int &id, const int &frequency);
    bool deleteById(qlonglong id);
};

#endif // MaterialDao_H
