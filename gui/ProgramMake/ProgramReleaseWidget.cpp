#include "ProgramReleaseWidget.h"
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

ProgramReleaseWidget::ProgramReleaseWidget(QWidget *parent) :
    QWidget(parent)
{
    initializeUi();
    handleEvents();
    openFile("C:/Users/Technical/Desktop/test.addi");
}

ProgramReleaseWidget::~ProgramReleaseWidget()
{

}

bool ProgramReleaseWidget::openFile(const QString filepath)
{
    // 打开文件--------------
    QFile file(filepath);
    if (file.open(QFile::ReadOnly)) {
        qDebug() << "打开文件" << filepath;
        QByteArray nArray = file.readAll();

        // 打开失败
        bool ok = toBean(nArray);
        if (!ok) {
            QMessageBox::warning(this, QStringLiteral("打开失败"), QString("打开文件失败,请检查文件！"));
        }
    }
}

bool ProgramReleaseWidget::toBean(QByteArray byteArray)
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
    auto pCanvasWidget = new CanvasWidget;
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(pCanvasWidget);
    this->setLayout(pLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    qDebug() << pCanvasWidget;
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

void ProgramReleaseWidget::initializeUi()
{
    // 全屏
    this->showFullScreen();
}

void ProgramReleaseWidget::initializeData()
{

}

void ProgramReleaseWidget::handleEvents()
{


}
