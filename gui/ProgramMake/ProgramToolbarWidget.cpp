#include "ProgramToolbarWidget.h"
#include <QDebug>
#include <QToolBar>

ProgramToolbarWidget::ProgramToolbarWidget(QWidget *parent) : QToolBar(parent)
{
    initializeUi();
}

void ProgramToolbarWidget::initializeUi()
{
    this->setFixedHeight(53);

    // 创建工具栏，属性栏，节目窗口
    this->setStyleSheet("background: rgb(95, 190, 255);");

    // 工具栏添加按钮
    this->setMovable(false);

    p_actNew    = new QAction(QIcon("./image/programMakeWidget/new.png"), tr("新建"), this);
    p_actOpen   = new QAction(QIcon("./image/programMakeWidget/open.png"), tr("打开"), this);
    p_actSave   = new QAction(QIcon("./image/programMakeWidget/save.png"), tr("保存"), this);
    p_actSaveAs = new QAction(QIcon("./image/programMakeWidget/saveAs.png"), tr("另存"), this);
    p_actNew->setShortcut(Qt::Key_Control | Qt::Key_N);
    p_actOpen->setShortcut(Qt::Key_Control | Qt::Key_O);
    p_actSave->setShortcut(Qt::Key_Control | Qt::Key_S);
    p_actSaveAs->setShortcut(Qt::Key_Control | Qt::Key_Shift | Qt::Key_S);

    p_actText    = new QAction(QIcon("./image/programMakeWidget/text.png"), tr("文字"), this);

    p_actImage  = new QAction(QIcon("./image/programMakeWidget/image.png"), tr("图片"), this);
    p_actMusic  = new QAction(QIcon("./image/programMakeWidget/music.png"), tr("音频"), this);
    p_actVideo  = new QAction(QIcon("./image/programMakeWidget/video.png"), tr("视频"), this);

    p_actTime  = new QAction(QIcon("./image/programMakeWidget/time.png"), tr("时间"), this);
    p_actTimer = new QAction(QIcon("./image/programMakeWidget/timer.png"), tr("倒计时"), this);
    p_actWeather  = new QAction(QIcon("./image/programMakeWidget/weather.png"), tr("天气"), this);

//    p_actPreview = new QAction(QIcon("./image/icon/programMakeWidget/preview.png"), tr("预览"), this);
    p_actRelease  = new QAction(QIcon("./image/programMakeWidget/release.png"), tr("发布"), this);

    // 设置对象名称
    p_actNew->setObjectName("new");
    p_actOpen->setObjectName("open");
    p_actSave->setObjectName("save");
    p_actSaveAs->setObjectName("saveAs");
    p_actText->setObjectName("text");
    p_actImage->setObjectName("image");
    p_actMusic->setObjectName("music");
    p_actVideo->setObjectName("video");
    p_actTime->setObjectName("time");
    p_actTimer->setObjectName("timer");
    p_actWeather->setObjectName("weather");
//    p_actPreview->setObjectName("preview");
    p_actRelease->setObjectName("release");


    this->addAction(p_actNew);
    this->addAction(p_actOpen);
    this->addAction(p_actSave);
    this->addAction(p_actSaveAs);
    this->addSeparator();

    this->addAction(p_actText);
    this->addSeparator();

    this->addAction(p_actImage);
    this->addAction(p_actMusic);
    this->addAction(p_actVideo);
    this->addSeparator();

    this->addAction(p_actTime);
    this->addAction(p_actTimer);
    this->addAction(p_actWeather);
    this->addSeparator();

//    this->addAction(p_actPreview);
    this->addAction(p_actRelease);

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}
