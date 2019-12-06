#include "TreeMenuWidget.h"
#include <QDebug>
#include <QFile>


TreeMenuWidget::TreeMenuWidget(QWidget *parent) : QTreeWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

void TreeMenuWidget::initializeUi()
{
    this->setFixedWidth(180);
    this->resize(180, 600);

    // 设置样式表
    QFile file("./resources/qss/TreeMenuWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void TreeMenuWidget::initializeData()
{
    this->clear();
    this->headerItem()->setHidden(true);
    m_pTreeMenuDao = new TreeMenuDao;
    m_nTreeMenuBeanList = m_pTreeMenuDao->findtAll();

    for (int i=0; i<m_nTreeMenuBeanList.size(); i++) {
        // [1] 先添加一级菜单
        if (m_nTreeMenuBeanList.at(i).getLevel() == 1) {
            addMenu(m_nTreeMenuBeanList.at(i), 1);
        }
        // [2] 再添加二级菜单
        if (m_nTreeMenuBeanList.at(i).getLevel() == 2) {
            addMenu(m_nTreeMenuBeanList.at(i), 2);
        }
    }
}

void TreeMenuWidget::handleEvents()
{
    connect(this, &TreeMenuWidget::itemClicked, [this](QTreeWidgetItem *item, int column) {
        qDebug() << "item:" << item << column;
    });
}

void TreeMenuWidget::addMenu(TreeMenuBean bean, int level)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem();
    QString name = bean.getName();
    pItem->setText(0, name);
    if (level == 1) {
        // [1] 加入一级菜单
        this->addTopLevelItem(pItem);
    } else if (level == 2) {
        // [2] 加入二级菜单
        for (int i=0; i<m_nStructList.size(); i++) {
            if (m_nStructList.at(i).bean.getId() == bean.getParentId()) {
                m_nStructList.at(i).pItem->addChild(pItem);
            }
        }
    }
    // 加入结构体list
    StructBeanItem beanItem = { bean,  pItem };
    m_nStructList.append(beanItem);
}
