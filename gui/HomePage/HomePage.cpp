#include "HomePage.h"
#include "ui_HomePage.h"
#include "HomePageWeatherWidget.h"
#include "bean/MaterialBean.h"
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QDateTime>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    m_pTimer = new QTimer(this);
    m_pTimer->start(10000);

    initializeUi();
    initializeData();
    handleEvents();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::initializeUi()
{
    // 获取分辨率
    QScreen *pScreen = QGuiApplication::primaryScreen();
    QRect mm = pScreen->availableGeometry();

    this->layout()->setContentsMargins(0, 0, 0, 0);

    auto pHomePageWeatherWidget = new HomePageWeatherWidget;
    ui->frame_2->layout()->replaceWidget(ui->widgetWeather, pHomePageWeatherWidget);

    // 背景图片
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap("./image/HomePage/HomepageBackground.png").scaled(mm.width(), mm.height()));
    this->setPalette(pal);

    // 背景颜色(方块)
    QPalette pal2;
    pal2.setColor(QPalette::Background, QColor(163,215,114,255));
    ui->widgetMaterial->setAutoFillBackground(true);
    ui->widgetMaterial->setPalette(pal2);

    pal2.setColor(QPalette::Background, QColor(205,143,22,255));
    ui->widgetClient->setAutoFillBackground(true);
    ui->widgetClient->setPalette(pal2);

    pal2.setColor(QPalette::Background, QColor(73,160,229,255));
    pHomePageWeatherWidget->setAutoFillBackground(true);
    pHomePageWeatherWidget->setPalette(pal2);

    pal2.setColor(QPalette::Background, QColor(0,0,0,50));
    ui->frameBottom->setAutoFillBackground(true);
    ui->frameBottom->setPalette(pal2);

    // 字体大小及颜色
    ui->labelSysName01->setFont(QFont("微软雅黑", 25));
    ui->labelSysName02->setFont(QFont("微软雅黑", 25));
    ui->labelTime->setFont(QFont("微软雅黑", 30));

    pal2.setColor(QPalette::WindowText, QColor(30,69,98));
    ui->labelSysName01->setPalette(pal2);
    ui->labelSysName02->setPalette(pal2);

    ui->labelCompany->setFont(QFont("微软雅黑", 10));
    pal2.setColor(QPalette::WindowText, QColor(255,255,255,150));
    ui->labelCompany->setPalette(pal2);

    ui->labelMaterial->setFont(QFont("微软雅黑", 20));
    ui->labelClient->setFont(QFont("微软雅黑", 20));

    pal2.setColor(QPalette::WindowText, QColor(255,255,255));
    ui->labelMaterial->setPalette(pal2);
    ui->labelClient->setPalette(pal2);
    ui->labelTime->setPalette(pal2);

    // 方块大小
    ui->frame_2->layout()->setSpacing(30);
    ui->frame_2->setFixedSize(mm.height()/7*5, mm.height()/2);
    pHomePageWeatherWidget->setFixedWidth(mm.height()/6*4/2);
}

void HomePage::initializeData()
{
    // 时间
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd  dddd  hh:mm");
    ui->labelTime->setText(strDateTime);

    // 素材
    QList<MaterialBean> materialBeanList = m_materialDao.findAll();
    int nProgramNum = 0, nVideoNum = 0, nAudioNum = 0, nImageNum = 0, nDocumentNum = 0, nOtherNum = 0;

    for (int i=0; i<materialBeanList.size(); i++) {
        FileType fileType = materialBeanList.at(i).getFileType();
        if (fileType == FileType::ProgramType)
            nProgramNum ++;
        if (fileType == FileType::OtherType)
            nOtherNum ++;
        if (fileType == FileType::VideoType)
            nVideoNum ++;
        if (fileType == FileType::AudioType)
            nAudioNum ++;
        if (fileType == FileType::ImageType)
            nImageNum ++;
        if (fileType == FileType::DocumentType)
            nDocumentNum ++;
    };

    ui->labelProgramNum->setText(QString(QString::number(nProgramNum) + " " + tr("个")));
    ui->labelVideoNum->setText(QString(QString::number(nVideoNum) + " " + tr("段")));
    ui->labelAudioNum->setText(QString(QString::number(nAudioNum) + " " + tr("段")));
    ui->labelImageNum->setText(QString(QString::number(nImageNum) + " " + tr("张")));
    ui->labelDocumentNum->setText(QString(QString::number(nDocumentNum) + " " + tr("个")));
    ui->labelOtherNum->setText(QString(QString::number(nOtherNum) + " " + tr("个")));
    // 客户端
    int nClientOfflineNum = 0, nClientOnlineNum = 0;

    ui->labelClientOfflineNum->setText(QString(QString::number(nClientOfflineNum) + "台"));
    ui->labelClientOnlineNum->setText(QString(QString::number(nClientOnlineNum) + "台"));
}

void HomePage::handleEvents()
{
    connect(m_pTimer, &QTimer::timeout, [this] {
        initializeData();
    });
}
