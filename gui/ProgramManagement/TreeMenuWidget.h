#ifndef TreeMenuWidget_H
#define TreeMenuWidget_H

#include <QTreeWidget>
#include "bean/TreeMenuBean.h"
#include "dao/TreeMenuDao.h"

class TreeMenuBean;

/**
 * @brief The TreeMenuWidget class
 * 树形菜单
 */

class TreeMenuWidget : public QTreeWidget
{
    Q_OBJECT
public:
    struct StructBeanItem
    {
        TreeMenuBean bean;
        QTreeWidgetItem *pItem;
    };

    explicit TreeMenuWidget(QWidget *parent = nullptr);

private:
    void initializeUi();                            // 初始化界面
    void initializeData();                          // 初始化数据
    void handleEvents();                            // 信号与槽处理

    void addMenu(TreeMenuBean bean, int level);     // 添加菜单

    QList<StructBeanItem> m_nStructList;            // 菜单List
    TreeMenuDao *m_pTreeMenuDao;
    QList<TreeMenuBean> m_nTreeMenuBeanList;
};

#endif // TreeMenuWidget_H
