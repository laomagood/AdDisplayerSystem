#include "PropertyWidget.h"
#include "CanvasPropertyWidget.h"
#include "Control/TextPropertyWidget.h"
#include "Control/TextWidget.h"
#include "Control/ImagePropertyWidget.h"
#include "Control/ImageWidget.h"
#include "Control/MVPropertyWidget.h"
#include "Control/MusicWidget.h"
#include "Control/VideoWidget.h"
#include "CanvasWidget.h"
#include <QDebug>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QStackedWidget>


PropertyWidget::PropertyWidget(QWidget *parent) :
    QWidget(parent)
{
    initializeUi();
    handleEvents();
}

void PropertyWidget::setCurrentWidget(QWidget *widget)
{
    if (widget == nullptr) {
       m_pStackedWidget->setCurrentIndex(0);
       return;
    }
    qDebug() << "属性栏切换" << qobject_cast<QWidget*>(widget);

    if (qobject_cast<CanvasWidget*>(widget)) {
        qDebug() << "画布属性";
        m_pStackedWidget->setCurrentWidget(m_pCanvasPropertyWidget);
        m_pCanvasPropertyWidget->setCanvasWidget(widget);
    } else if (qobject_cast<TextWidget*>(widget)) {
        qDebug() << "文字属性";
        m_pStackedWidget->setCurrentWidget(m_pTextPropertyWidget);
        m_pTextPropertyWidget->setTextWidget(widget);
    } else if (qobject_cast<ImageWidget*>(widget)) {
        qDebug() << "图片属性";
        m_pStackedWidget->setCurrentWidget(m_pImagePropertyWidget);
        m_pImagePropertyWidget->setImageWidget(widget);
    } else if (qobject_cast<MusicWidget*>(widget)) {
        qDebug() << "音乐属性";
        m_pStackedWidget->setCurrentWidget(m_pMVPropertyWidget);
        m_pMVPropertyWidget->setWidget(widget);
    } else if (qobject_cast<VideoWidget*>(widget)) {
        qDebug() << "视频属性";
        m_pStackedWidget->setCurrentWidget(m_pMVPropertyWidget);
        m_pMVPropertyWidget->setWidget(widget);
    } else {
        qDebug() << "转换失败";
    }
}

void PropertyWidget::initializeUi()
{
    // 背景颜色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(255, 255, 255));
    this->setPalette(palette);
    setAutoFillBackground(true);

    // 属性栏控件属性
    auto *pGroupBox = new QGroupBox(tr("控件属性"));
    auto *pLayoutMain = new QVBoxLayout;
    pLayoutMain->addWidget(pGroupBox);
//    pLayoutMain->addStretch(2);
    this->setLayout(pLayoutMain);
    this->layout()->setContentsMargins(3, 7, 3, 3);

    // QStackedWidget 用来放置各种属性widget
    m_pStackedWidget = new QStackedWidget;
    auto *pLayoutGroupBox = new QVBoxLayout;
    pLayoutGroupBox->addWidget(m_pStackedWidget);
    pGroupBox->setLayout(pLayoutGroupBox);
    //    pGroupBox->layout()->setContentsMargins(0, 0, 0, 0);

    // 加入所有的属性widget
    m_pCanvasPropertyWidget = new CanvasPropertyWidget;
    m_pTextPropertyWidget = new TextPropertyWidget;
    m_pImagePropertyWidget = new ImagePropertyWidget;
    m_pMVPropertyWidget = new MVPropertyWidget;

    QWidget *pWidget = new QWidget;
    m_pStackedWidget->addWidget(pWidget);
    m_pStackedWidget->addWidget(m_pCanvasPropertyWidget);
    m_pStackedWidget->addWidget(m_pTextPropertyWidget);
    m_pStackedWidget->addWidget(m_pImagePropertyWidget);
    m_pStackedWidget->addWidget(m_pMVPropertyWidget);
}

void PropertyWidget::handleEvents()
{

}
