#include "ProgramMakeWidget.h"
#include "ProgramToolbarWidget.h"
#include "PropertyWidget.h"
#include "ProgramEditAreaWidget.h"
#include "CanvasWidget.h"
#include "PropertyWidget.h"
#include "ControlListWidget.h"
#include "Control/ImageWidget.h"
#include "Control/TextWidget.h"
#include "Control/VideoWidget.h"
#include "Control/MusicWidget.h"
#include "Control/TimeWidget.h"
#include "util/Json.h"
#include "global.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFile>
#include <QDebug>
#include <QTabWidget>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>

ProgramMakeWidget::ProgramMakeWidget(QWidget *parent) :
    QWidget(parent)
{
    initializeUi();
    handleEvents();
}

ProgramMakeWidget::~ProgramMakeWidget()
{

}

bool ProgramMakeWidget::toBean(QByteArray byteArray)
{
    // [1] 检查是否为Json文件
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    if (doucment.isNull() && (jsonError.error != QJsonParseError::NoError)) {
        qDebug() << "文件Json解析失败";
        return false;
    }

    // [2] 检查文件版本
    QString strJson(byteArray);
    Json nJson(strJson, false);
    QString nVersion = nJson.getString("Version");
    if (nVersion != "2019V01") {
        qDebug() << "版本错误";
        return false;
    }
    // [3] 其他检查····

    // [4] 生成画布，生成控件

    // 检测是否节目在发布状态
    CanvasWidget *pCanvasWidget = NULL;
    if (g_isReleaseProgram) {   // client端
        pCanvasWidget = new CanvasWidget;
    } else {    // server端
        m_pProgramEditAreaWidget->newCanvas();
        pCanvasWidget = m_pProgramEditAreaWidget->getCurrentCanvas();
    }

    pCanvasWidget->toBean(doucment);

    int nControlCount = nJson.getInt("ControlWidgetCount");
    for (int i=0; i<nControlCount; i++) {
        QString nControlWidget = "ControlWidgetList." + QString::number(i) + ".ControlWidget";
        QString nControlWidgetType = "ControlWidgetList." + QString::number(i) + ".Type";
        QString strX = "ControlWidgetList." + QString::number(i) + ".ReleaseGeometry.X";
        QString strY = "ControlWidgetList." + QString::number(i) + ".ReleaseGeometry.Y";
        QString strW = "ControlWidgetList." + QString::number(i) + ".ReleaseGeometry.W";
        QString strH = "ControlWidgetList." + QString::number(i) + ".ReleaseGeometry.H";

        QString nWidgetType = nJson.getString(nControlWidgetType);
        int x = nJson.getInt(strX);
        int y = nJson.getInt(strY);
        int w = nJson.getInt(strW);
        int h = nJson.getInt(strH);

        QJsonDocument doc;
        doc.setObject(nJson.getJsonObject(nControlWidget));

        if (nWidgetType == "TextWidget") {
            auto pWidget = new TextWidget;
            pCanvasWidget->addControlWidget(pWidget);
            pCanvasWidget->setControlWidgetGeometry(pWidget, x, y, w, h);
            pWidget->toBean(doc);
        } else if (nWidgetType == "ImageWidget") {
            auto pWidget = new ImageWidget;
            pCanvasWidget->addControlWidget(pWidget);
            pCanvasWidget->setControlWidgetGeometry(pWidget, x, y, w, h);
            pWidget->toBean(doc);
        } else if (nWidgetType == "VideoWidget") {
            auto pWidget = new VideoWidget;
            pCanvasWidget->addControlWidget(pWidget);
            pCanvasWidget->setControlWidgetGeometry(pWidget, x, y, w, h);
            pWidget->toBean(doc);
        } else {
            qDebug() << "没有相应匹配的控件类型";
        }
    }
}

void ProgramMakeWidget::initializeUi()
{
    // 创建工具栏，属性栏，图层栏，节目编辑区
    m_pProgramToolbarWidget = new ProgramToolbarWidget;
    m_pControlListWidget = new ControlListWidget;
    m_pProgramEditAreaWidget = new ProgramEditAreaWidget;
    m_pPropertyWidget = new PropertyWidget;

    m_pPropertyWidget->setFixedWidth(200);
    m_pControlListWidget->setFixedWidth(200);

    // 布局
    auto pVLayout = new QVBoxLayout;
    auto pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pPropertyWidget);
    pHLayout->addWidget(m_pProgramEditAreaWidget);
    pHLayout->addWidget(m_pControlListWidget);
    pVLayout->addWidget(m_pProgramToolbarWidget);
    pVLayout->addLayout(pHLayout);
    this->setLayout(pVLayout);

    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
}

void ProgramMakeWidget::initializeData()
{

}

void ProgramMakeWidget::handleEvents()
{
    // 点击工具栏按钮
    connect(m_pProgramToolbarWidget, &ProgramToolbarWidget::actionTriggered, [this](QAction *action){
        QString strName = action->objectName();
        if (strName != "new" && strName != "open") {
            if (m_pProgramEditAreaWidget->count() <= 0) {
                qDebug() << "没有画布";
                return;
            }
        }

        if (strName == "new") {             // 新建文件--------------
            m_pProgramEditAreaWidget->newCanvas();
        } else if (strName == "open") {     // 打开文件--------------
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/Ma",
                                                             tr("节目 (*.addi);;模板 (*.txt)"));
            QFile file(fileName);
            if (file.open(QFile::ReadOnly)) {
                qDebug() << "打开文件" << fileName;
                QByteArray nArray = file.readAll();

                // 打开失败
                bool ok = toBean(nArray);
                if (!ok) {
                    QMessageBox::warning(this, QStringLiteral("打开失败"), QString("打开文件失败,请检查文件！"));
                }
            }
        } else if (strName == "save") {     // 保存文件--------------
            // [1] 没有存盘
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/Ma",
                                                            tr("节目 (*.addi);;模板 (*.txt)"));
            // [2] 已经存盘
            QFile file(fileName);
            if(file.open(QFile::WriteOnly)) {
                QJsonDocument doc = m_pProgramEditAreaWidget->getCurrentCanvas()->toJson();
                file.write(doc.toJson());
                QMessageBox::information(this, QStringLiteral("保存文件"), QString("节目文件保存成功"));
            }
        } else if (strName == "saveAs") {   // 另存文件--------------
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save As File"), "/Ding",
                                                            tr("节目 (*.txt);;模板 (*.txt)"));
            QFile file(fileName);
            if(file.open(QFile::WriteOnly)) {
                QJsonDocument doc = m_pProgramEditAreaWidget->getCurrentCanvas()->toJson();
                file.write(doc.toJson());
                QMessageBox::information(this, QStringLiteral("保存文件"), QString("节目文件保存成功"));
            }
        } else if (strName == "text") {
            auto pWidget = new TextWidget;
            m_pProgramEditAreaWidget->getCurrentCanvas()->addControlWidget(pWidget);
            qDebug() << pWidget;
        } else if (strName == tr("image")) {
            auto pWidget = new ImageWidget;
            pWidget->move(100, 100);
            m_pProgramEditAreaWidget->getCurrentCanvas()->addControlWidget(pWidget);
        } else if (strName == "music") {
            auto pWidget = new MusicWidget;
            pWidget->move(-200, -200);
            pWidget->setHidden(true);
            m_pProgramEditAreaWidget->getCurrentCanvas()->addControlWidget(pWidget);
        } else if (strName == "video") {
            auto pWidget = new VideoWidget;
            pWidget->move(100, 100);
            m_pProgramEditAreaWidget->getCurrentCanvas()->addControlWidget(pWidget);
        } else if (strName == "time") {
            auto pWidget = new TimeWidget;
            m_pProgramEditAreaWidget->getCurrentCanvas()->addControlWidget(pWidget);
        } else if (strName == "timer") {
            qDebug() << strName;
        } else if (strName == "preview") {
            auto pWidget = new QWidget;
            auto pLayout = new QHBoxLayout;
            pLayout->addWidget(this->m_pProgramEditAreaWidget->getCurrentCanvas());
            pWidget->setLayout(pLayout);
            pWidget->show();
        } else {
            qDebug() << strName;

        }
    });

    // 切换属性栏
    connect(m_pProgramEditAreaWidget, &ProgramEditAreaWidget::focusWidget, [this](QWidget *pWidget) {
        m_pPropertyWidget->setCurrentWidget(pWidget);
        if (qobject_cast<CanvasWidget*>(pWidget)) {
            qDebug() << "ProgramMakeWidget::" << pWidget;
            m_pControlListWidget->setCanvasWidget(qobject_cast<CanvasWidget*>(pWidget));
        }
    });

    // 列表栏更新
    connect(m_pProgramEditAreaWidget, &ProgramEditAreaWidget::updateControlList, [this](CanvasWidget *pWidget) {
        m_pControlListWidget->setCanvasWidget(pWidget);
    });

    // 图层栏
    connect(m_pControlListWidget, &ControlListWidget::itemClicked, [this](ControlWidget* pControlWidget) {
        // 切换焦点
        pControlWidget->setFocus();
    });

    connect(m_pControlListWidget, &ControlListWidget::menuClicked, [this](QVariant variant) {
        // 获取控件指针， 以及右键菜单类型
        ST_SIGNAL_DATA signalData = variant.value<ST_SIGNAL_DATA>();
        ControlWidget *pControlWidget = signalData.pControlWidget;
        MenuOption menuOption = signalData.menuOption;

        switch (menuOption) {
        case REMOVECONTROL:
            m_pProgramEditAreaWidget->getCurrentCanvas()->removeControlWidget(pControlWidget);
            break;
        case MOVELAYERUP:
            m_pProgramEditAreaWidget->getCurrentCanvas()->moveLayerUP(pControlWidget);
            break;
        case MOVELAYERDOWN:
            m_pProgramEditAreaWidget->getCurrentCanvas()->moveLayerDown(pControlWidget);
            break;
        case MOVELAYERTOP:
            m_pProgramEditAreaWidget->getCurrentCanvas()->moveLayerTop(pControlWidget);
            break;
        case MOVELAYERBOTTOM:
            m_pProgramEditAreaWidget->getCurrentCanvas()->moveLayerBottom(pControlWidget);
            break;
        default:
            break;
        }
    });
}
