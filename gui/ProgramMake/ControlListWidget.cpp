#include "ControlListWidget.h"
#include "Control/ControlListItemWidget.h"
#include "Control/ControlWidget.h"
#include <QDebug>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMetaType>
#include <QVariant>

ControlListWidget::ControlListWidget(QWidget *parent) :
    QWidget(parent)
{
    initializeUi();
    handleEvents();
    handleEvents();
}

void ControlListWidget::setCanvasWidget(CanvasWidget *canvasWidget)
{
    m_pCanvasWidget = canvasWidget;
    updateCanvas();
//    if (m_pCanvasWidget != nullptr) {
//        updateCanvas();
//    }
}

void ControlListWidget::initializeUi()
{
    // 属性栏控件属性
    auto *pGroupBox = new QGroupBox(tr("控件图层"));
    auto *pLayoutMain = new QVBoxLayout;
    pLayoutMain->addWidget(pGroupBox);
    //    pLayoutMain->addStretch(2);
    this->setLayout(pLayoutMain);
    this->layout()->setContentsMargins(3, 7, 3, 3);

    m_pListWidget = new QListWidget;
    m_pListWidget->setMinimumHeight(200);
    m_pListWidget->setStyleSheet("border:0");

    auto *pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(2, 2, 2, 2);
    pLayout->addWidget(m_pListWidget);
    pGroupBox->setLayout(pLayout);

    m_pListWidget->setResizeMode(QListView::Adjust);
    m_pListWidget->setAutoScroll(true);

//    m_pListWidget->setDragEnabled(true);
//    m_pListWidget->setDropIndicatorShown(true);

    contextMenu();
}

void ControlListWidget::handleEvents()
{
    // 点击发送信号，用来画布部件控件确定焦点
    connect(m_pListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        ControlListItemWidget* pControlListItemWidget = (ControlListItemWidget*)m_pListWidget->itemWidget(item);
        ControlWidget* pControlWidget = pControlListItemWidget->getControlWidget();
        emit itemClicked(pControlWidget);
    });
}

void ControlListWidget::contextMenu()
{
    // 增加右键功能
    m_pListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QMenu *pMenu = new QMenu(this);
    QAction *removeAct = new QAction(tr("删除部件"));
    QAction *moveLayerUPAct = new QAction(tr("上移一层"));
    QAction *moveLayerDowmAct = new QAction(tr("下移一层"));
    QAction *moveLayerTopAct = new QAction(tr("移动到顶层"));
    QAction *moveLayerBottomAct = new QAction(tr("移动到底层"));

    pMenu->addAction(removeAct);
    pMenu->addAction(moveLayerUPAct);
    pMenu->addAction(moveLayerDowmAct);
    pMenu->addAction(moveLayerTopAct);
    pMenu->addAction(moveLayerBottomAct);

    // 获取右键点击时ControlWidget指针
    connect(m_pListWidget, &QListWidget::customContextMenuRequested, [=](QPoint pox) {
        ControlListItemWidget* pControlListItemWidget;
        pControlListItemWidget = (ControlListItemWidget*)m_pListWidget->itemWidget(m_pListWidget->itemAt(pox));
        m_pControlWidget = pControlListItemWidget->getControlWidget();

        pMenu->exec(QCursor::pos());
    });

    // 右键发送信号
    connect(pMenu, &QMenu::triggered, [=](QAction *action) {
        ST_SIGNAL_DATA signalData;
        signalData.pControlWidget = m_pControlWidget;

        if (action == removeAct) {
            signalData.menuOption = REMOVECONTROL;
        }
        if (action == moveLayerUPAct) {
            signalData.menuOption = MOVELAYERUP;
        }
        if (action == moveLayerDowmAct) {
            signalData.menuOption = MOVELAYERDOWN;
        }
        if (action == moveLayerTopAct) {
            signalData.menuOption = MOVELAYERTOP;
        }
        if (action == moveLayerBottomAct) {
            signalData.menuOption = MOVELAYERBOTTOM;
        }

        QVariant variant;
        variant.setValue(signalData);
        emit menuClicked(variant);
    });
}

void ControlListWidget::updateCanvas()
{
    // 全部删除
    while (m_pListWidget->count() > 0) {
        m_pListWidget->removeItemWidget(m_pListWidget->item(0));
        m_pListWidget->takeItem(0);
    }

    if (m_pCanvasWidget != nullptr) {
        // 这里可能有内存泄漏
        QList<ControlWidget*> list = m_pCanvasWidget->getListControlWidget();

        int nSize = list.size();
        for(int i=(nSize-1); i>=0; i--) {
            auto *pControlListItemWidget = new ControlListItemWidget;
            pControlListItemWidget->setControlWidget(list.at(i));

            QListWidgetItem *pItem = new QListWidgetItem(m_pListWidget);
            m_pListWidget->addItem(pItem);
            m_pListWidget->setItemWidget(pItem, pControlListItemWidget);
            pItem->setSizeHint(QSize(0, 40));
        }
    }
}
