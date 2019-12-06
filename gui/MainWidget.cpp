#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "gui/HomePage/HomePage.h"
#include "gui/programMake/ProgramMakeWidget.h"
#include "gui/MaterialManagement/MaterialManagement.h"
#include "gui/ProgramManagement/ProgramManagement.h"
#include "gui/ClientManagement/ClientManagementWidget.h"
#include "gui/Setting/Setting.h"
#include "UserCornerMarker.h"
#include "function/Transfer/DeviceBroadcaster.h"
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QSettings>
#include <QMenu>
#include <QCloseEvent>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);


    initializeUi();
    initializeData();
    handleEvents();
    ui->itemButton1->click();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    if (mSystrayIcon && mSystrayIcon->isVisible() && !mForceQuit) {
        this->hide();
        event->ignore();
        return;
    }

    // 文件没有保存
    /*
    if (needToConfirm) {
        QMessageBox::StandardButton ret =
                QMessageBox::question(this, tr("Confirm close"),
                                      tr("You are about to close & abort all transfers. Do you want to continue?"));
        if (ret == QMessageBox::No) {
            event->ignore();
            mForceQuit = false;
            return;
        }
    }
    */

    event->accept();
    qApp->quit();
}

// 初始化界面
void MainWidget::initializeUi()
{
    this->setWindowTitle(tr("多媒体发布系统"));
    this->setWindowState(Qt::WindowMaximized);

    // 自定义动态属性
    ui->logoWidget->setProperty("class", "logoWidget");
    ui->topBarWidget->setProperty("class", "topBarWidget");
    ui->centralStackedWidget->setProperty("class", "centralStackedWidget");

    ui->itemButton1->setProperty("class", "ItemButton");
    ui->itemButton2->setProperty("class", "ItemButton");
    ui->itemButton3->setProperty("class", "ItemButton");
    ui->itemButton4->setProperty("class", "ItemButton");
    ui->itemButton5->setProperty("class", "ItemButton");
    ui->itemButton6->setProperty("class", "ItemButton");

    ui->itemButton3->hide();

    // 用户
    auto pUserCornerMarker = new UserCornerMarker;
    delete ui->topBarWidget->layout()->replaceWidget(ui->userWidget, pUserCornerMarker);
    delete ui->userWidget;

    // 查找sideBar上的按钮,判断class属性，并放入QButtonGroup*中
    mItemButtons = new QButtonGroup;
    QObjectList children = ui->topBarWidget->children();
    foreach (QObject *object, children) {
        // 强制转换
        QAbstractButton *btn = qobject_cast<QAbstractButton*> (object);
        if (btn == NULL) {
            continue;
        }
        QString className = object->property("class").toString();
        if (className == "ItemButton") {
            btn->setCheckable(true);
            mItemButtons->addButton(btn);
        }
    }
    // 设置按钮互斥
    mItemButtons->setExclusive(true);

    // 去掉边框
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
    ui->topBarWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->topBarWidget->layout()->setSpacing(0);

    // 设置样式表
    QFile file("./resources/qss/mainWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }

    // 文件图标
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\.addi\\DefaultIcon", QSettings::NativeFormat);
    QString applicationDirPathStr = QCoreApplication::applicationDirPath();
    QString appFileIcon = applicationDirPathStr + "/ad_file.ico";
    reg.setValue("Default", appFileIcon);

    // 系统托盘图标
    setupActions();
    setupSystrayIcon();
}

void MainWidget::initializeData()
{
    // 广播
//    m_pBroadcaster = new DeviceBroadcaster(this);
//    m_pBroadcaster->start();
}

// 信号与槽处理
void MainWidget::handleEvents()
{
    // 点击成员按钮，切换stackedWidget
    connect(this->mItemButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {

        if (!mButtonWidgetHash.contains(button)) {
            createWidgetInContentStackedWidget(button);
        }

        ui->centralStackedWidget->setCurrentWidget(mButtonWidgetHash.value(button));
    });
}

// 创建需要在内容区 stacked widget 中显示的 widget
void MainWidget::createWidgetInContentStackedWidget(QAbstractButton *button)
{
    if (button == ui->itemButton1) {
        // 主页
        QWidget *w = new HomePage();
        mButtonWidgetHash.insert(ui->itemButton1, w);
        ui->centralStackedWidget->addWidget(w);
    } else if (button == ui->itemButton2) {
        // 节目制作
        QWidget *w = new ProgramMakeWidget();
        mButtonWidgetHash.insert(ui->itemButton2, w);
        ui->centralStackedWidget->addWidget(w);
    } else if (button == ui->itemButton4) {
        // 素材节目管理
        QWidget *w = new MaterialManagement();
        mButtonWidgetHash.insert(ui->itemButton4, w);
        ui->centralStackedWidget->addWidget(w);
    } else if (button == ui->itemButton5) {
        // 终端管理
        QWidget *w = new ClientManagementWidget();
        mButtonWidgetHash.insert(ui->itemButton5, w);
        ui->centralStackedWidget->addWidget(w);
    } else {
        // 设置
        QWidget *w = new Setting();
        mButtonWidgetHash.insert(ui->itemButton6, w);
        ui->centralStackedWidget->addWidget(w);
    }
}

void MainWidget::setupActions()
{
    // 系统托盘
    mShowMainWindowAction = new QAction(tr("Show Main Window"), this);
    connect(mShowMainWindowAction, &QAction::triggered, [this] { setMainWindowVisibility(true); });
    mSettingsAction = new QAction(QIcon("./image/MainWidget/settings.png"), tr("设置"), this);
    mAboutAction = new QAction(QIcon("./image/MainWidget/about.png"), tr("关于"), this);
    mAboutAction->setMenuRole(QAction::AboutRole);
    connect(mAboutAction, &QAction::triggered, [this] { });
    mAboutQtAction = new QAction(QIcon("./image/MainWidget/aboutQt.png"), tr("关于 Qt"), this);
    mAboutQtAction->setMenuRole(QAction::AboutQtRole);
    connect(mAboutQtAction, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);
    mQuitAction = new QAction(tr("退出"), this);
    connect(mQuitAction, &QAction::triggered, [this]{
        mForceQuit = true;
        close();
    });
}

void MainWidget::setupSystrayIcon()
{
    // 系统不支持系统托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        mSystrayIcon = nullptr;
        return;
    }

    mSystrayMenu = new QMenu(this);
    mSystrayMenu->addAction(mShowMainWindowAction);
    mSystrayMenu->addSeparator();
    mSystrayMenu->addAction(mSettingsAction);
    mSystrayMenu->addSeparator();
    mSystrayMenu->addAction(mAboutAction);
    mSystrayMenu->addAction(mAboutQtAction);
    mSystrayMenu->addSeparator();
    mSystrayMenu->addAction(mQuitAction);

    mSystrayIcon = new QSystemTrayIcon(QIcon("./image/MainWidget/Logo_Server.png"), this);
    mSystrayIcon->setToolTip(tr("多媒体发布系统"));
    mSystrayIcon->setContextMenu(mSystrayMenu);
    mSystrayIcon->show();

    // 单击显示界面
    connect(mSystrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason) {
        if (this->isHidden() && reason==QSystemTrayIcon::Trigger)
            setMainWindowVisibility(true);
    });
}

void MainWidget::setMainWindowVisibility(bool visible)
{
    if (visible) {
        show();
        setWindowState(Qt::WindowNoState);
        qApp->processEvents();
        setWindowState(Qt::WindowActive);
        qApp->processEvents();
        qApp->setActiveWindow(this);
        qApp->processEvents();
    }
    else {
        hide();
    }
}
