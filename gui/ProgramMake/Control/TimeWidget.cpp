#include "TimeWidget.h"
#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QDateTime>
#include <QTimer>

TimeWidget::TimeWidget(QWidget *parent) :
    TextBase(parent)
{
    this->setControlName(tr("时间"));
    this->setControlIconPath("./image/icon/programMakeWidget/time.png");

    initializeUi();
    initializeData();
    handleEvents();
}

void TimeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
//    this->setFixedWidth(this->height() * 6.2);
//    this->resize(this->height() * 6.2, this->height());
    setTextHeight();
}

void TimeWidget::initializeData()
{
    // 背景颜色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, m_backgroundColor);
    this->setPalette(palette);
    setAutoFillBackground(true);

    // 字体颜色
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, m_textColor);
    m_pLabelMonth->setPalette(labelPalette);
    m_pLabelWeek->setPalette(labelPalette);
    m_pLabelHour->setPalette(labelPalette);
    m_pLabelSecond->setPalette(labelPalette);

    // 字高
    setTextHeight();
}

void TimeWidget::initializeUi()
{
    this->resize(350, 50);

    m_pLabelMonth   = new QLabel;
    m_pLabelWeek    = new QLabel;
    m_pLabelHour    = new QLabel;
    m_pLabelSecond  = new QLabel;

    // 忽略鼠标事件
    m_pLabelMonth->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_pLabelWeek->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_pLabelHour->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_pLabelSecond->setAttribute(Qt::WA_TransparentForMouseEvents);

    // 布局
    auto *pLayout = new QGridLayout;
    pLayout->addWidget(m_pLabelHour, 0, 0, 2, 1);
    pLayout->addWidget(m_pLabelSecond, 1, 1, 1, 1);
    pLayout->addWidget(m_pLabelWeek, 0, 2, 1, 1);
    pLayout->addWidget(m_pLabelMonth, 1, 2, 1, 1);
    this->setLayout(pLayout);
    this->layout()->setSpacing(0);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // 定时器
    m_pTimer = new QTimer(this);
    m_pTimer->start(0);
}

void TimeWidget::handleEvents()
{
    connect(m_pTimer, &QTimer::timeout, [this] {
        timerUpdate();
    });
}

void TimeWidget::setTextHeight()
{
    int nTextHeight = this->height() - 10;
    m_pLabelMonth->setFont(QFont("微软雅黑", nTextHeight/2.75));
    m_pLabelWeek->setFont(QFont("微软雅黑", nTextHeight/2.5));
    m_pLabelHour->setFont(QFont("微软雅黑", nTextHeight, QFont::Bold));
    m_pLabelSecond->setFont(QFont("微软雅黑", nTextHeight/2.2, QFont::Bold));
}

void TimeWidget::timerUpdate()
{
    QDateTime dateTime = QDateTime::currentDateTime();

    m_pLabelMonth->setText(dateTime.toString("yy-MM-dd"));
    m_pLabelWeek->setText(dateTime.toString("dddd"));
    m_pLabelHour->setText(dateTime.toString("HH:mm"));
    m_pLabelSecond->setText(dateTime.toString("ss"));
}

