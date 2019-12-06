#include "TopWindow.h"
#include "ui_topwindow.h"
#include "NinePatchPainter.h"

#include <QDebug>
#include <QSizeGrip>
#include <QPainter>
#include <QMouseEvent>

class TopWindowPrivate {
public:
    TopWindowPrivate(const QMargins &windowPaddings,
                     const QMargins &borderImageBorders,
                     const QString  &borderImagePath,
                     bool borderImageHorizontalStretch,
                     bool borderImageVerticalStrethch)
    {
        ninePatchPainter = new NinePatchPainter(QPixmap(borderImagePath),
                                                borderImageBorders.left(),
                                                borderImageBorders.top(),
                                                borderImageBorders.right(),
                                                borderImageBorders.bottom(),
                                                borderImageHorizontalStretch,
                                                borderImageVerticalStrethch);

        sizeGrip = new QSizeGrip(NULL);
        padding = windowPaddings;
    }

public:
    NinePatchPainter *ninePatchPainter;
    QPoint windowPositionBeforeMoving;
    QPoint mousePressedPosition;
    QSizeGrip *sizeGrip;
    QMargins padding;
    bool resizable = true;


};

TopWindow::TopWindow(QWidget *centralWidget,
                              const QMargins &windowPaddings,
                              const QMargins &borderImageBorders,
                              const QString  &borderImagePath,
                              bool borderImageHorizontalStretch,
                              bool borderImageVerticalStrethch) : ui(new Ui::TopWindow)
{
    ui->setupUi(this);
    ui->restoreButton->hide();
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint |
                   Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);    //设置无边框窗口
    setAttribute(Qt::WA_TranslucentBackground); //背景透明

    d = new TopWindowPrivate(windowPaddings, borderImageBorders, borderImagePath,
                             borderImageHorizontalStretch, borderImageVerticalStrethch);

    ui->titleBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // 将中心widget调整策略设置为拉伸
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 获取布局，替换原来的widget
    QGridLayout *l = qobject_cast<QGridLayout*>(QWidget::layout());
    l->setSpacing(0);
    l->setContentsMargins(d->padding);
    delete l->replaceWidget(ui->centralWidget, centralWidget);
    delete ui->centralWidget;
    this->setTitle(centralWidget->windowTitle());

    // 添加 size grip 到窗口右下角
    l->addWidget(d->sizeGrip, 1, 0, Qt::AlignRight | Qt::AlignBottom);

    handleEvents();
}

TopWindow::~TopWindow()
{
    delete ui;
    delete d;
}
// 设置标题
void TopWindow::setTitle(const QString &title)
{
    ui->titleLabel->setText(title);
}
// 设置标题栏
void TopWindow::setTitleBar(QWidget *titleBar)
{
    ui->titleBar->hide();
    delete QWidget::layout()->replaceWidget(ui->titleBar, titleBar);
}
// 设置标题栏是否可见
void TopWindow::setTitleBarVisible(bool visible)
{
    ui->titleBar->setVisible(visible);
}
// 设置标题栏按钮是否可见
void TopWindow::setTitleBarButtonsVisible(bool min, bool max, bool close)
{
    ui->minButton->setVisible(min);
    ui->maxButton->setVisible(max);
    ui->closeButton->setVisible(close);
}
// 设置窗口大小是否可以修改
void TopWindow::setResizable(bool resizale)
{
    d->resizable = resizale;
    d->sizeGrip->setVisible(resizale);
}
// 最大化
void TopWindow::showMaximized()
{
    ui->maxButton->hide();
    ui->restoreButton->show();
    QWidget::showMaximized();
    // 最大化时去掉边距
    this->layout()->setContentsMargins(0, 0, 0, 0);
    d->sizeGrip->setVisible(false);
}
// 还原
void TopWindow::showNormal()
{
    ui->maxButton->show();
    ui->restoreButton->hide();
    QWidget::showNormal();
    this->layout()->setContentsMargins(d->padding);
    d->sizeGrip->setVisible(true);
}
// 显示模态
void TopWindow::showModal()
{
//    setWindowFlags(Qt::Dialog | Qt::Popup | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    this->show();

    QEventLoop loop;
    connect(this, &TopWindow::aboutClose, &loop, &QEventLoop::quit);
    loop.exec();
}

// 事件--绘制
void TopWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    d->ninePatchPainter->paint(&painter, rect(),d->padding.top(), d->padding.left(),
                               d->padding.bottom(), d->padding.right());
}
// 事件--鼠标按压
void TopWindow::mousePressEvent(QMouseEvent *event)
{
    d->mousePressedPosition         = event->globalPos();
    d->windowPositionBeforeMoving   = frameGeometry().topLeft();
    // qDebug() << "鼠标按下时的坐标：" << d->mousePressedPosition;
    // qDebug() << "窗口移动前的坐标：" << d->windowPositionBeforeMoving;
}
// 事件--鼠标释放
void TopWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    // 鼠标释放时 mousePressedPosition 为空
    d->mousePressedPosition = QPoint();
}
// 事件--鼠标移动
void TopWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 最大化时不可移动窗口
    if (isMaximized()) {
        return;
    }
    // 鼠标移动的位移差，就是窗口移动的位移差
    if (!d->mousePressedPosition.isNull()) {
       QPoint delta = event->globalPos() - d->mousePressedPosition;
       QPoint newPosition = d->windowPositionBeforeMoving + delta;
       move(newPosition);
    }
}
// 事件--关闭:关闭窗口发送aboutClose()信号,模态对话框
void TopWindow::closeEvent(QCloseEvent *event)
{
    emit aboutClose();
    QWidget::closeEvent(event);
}
// 信号与槽
void TopWindow::handleEvents()
{
    connect(ui->minButton, &QPushButton::clicked, [this] { showMinimized(); });
    connect(ui->maxButton, &QPushButton::clicked, [this] { showMaximized(); });
    connect(ui->restoreButton, &QPushButton::clicked, [this] { showNormal(); });
    connect(ui->closeButton, &QPushButton::clicked, [this] { close(); });
}
