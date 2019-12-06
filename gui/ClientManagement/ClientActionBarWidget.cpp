#include "ClientActionBarWidget.h"
#include "global.h"
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

ClientActionBarWidget::ClientActionBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

void ClientActionBarWidget::initializeUi()
{
    m_pBtnProgram = new QPushButton(tr("节目"));
    m_pBtnOrder = new QPushButton(tr("指令"));
    m_pBtnDelete = new QPushButton(tr("删除"));
    m_pBtnRefresh = new QPushButton(tr("刷新"));

    auto hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(new QLabel(tr("发送:")));
    hLayout->addWidget(m_pBtnProgram);
    hLayout->addWidget(m_pBtnOrder);
    hLayout->addSpacing(30);
    hLayout->addWidget(m_pBtnDelete);
    hLayout->addSpacing(30);
    hLayout->addWidget(m_pBtnRefresh);

    this->setLayout(hLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // 设置样式表
    QFile file("./resources/qss/ActionBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void ClientActionBarWidget::initializeData()
{

}

void ClientActionBarWidget::handleEvents()
{

}
