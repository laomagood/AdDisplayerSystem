#include "RoleActionBarWidget.h"
#include "global.h"
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

RoleActionBarWidget::RoleActionBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

void RoleActionBarWidget::initializeUi()
{
    m_pLineEditSearch = new QLineEdit;
    m_pLineEditSearch->setFixedWidth(200);
    m_pLineEditSearch->setPlaceholderText(tr("请输入要查询的用户名:"));
    m_pBtnSearch = new QPushButton(tr("查询"));
    m_pBtnSearchCancel = new QPushButton(tr("取消查询"));

    m_pBtnInsert = new QPushButton(tr("新增"));
    m_pBtnDelete = new QPushButton(tr("删除"));
    m_pBtnRefresh = new QPushButton(tr("刷新"));

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pLineEditSearch);
    hLayout->addWidget(m_pBtnSearch);
    hLayout->addWidget(m_pBtnSearchCancel);
    hLayout->addSpacing(30);

    hLayout->addStretch();
    hLayout->addWidget(m_pBtnInsert);
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

void RoleActionBarWidget::initializeData()
{

}

void RoleActionBarWidget::handleEvents()
{
    // 查询
    connect(m_pLineEditSearch, &QLineEdit::returnPressed, [this] {
        m_pBtnSearch->click();
    });
    connect(m_pBtnSearch, &QPushButton::clicked, [this] {
        QString strName = m_pLineEditSearch->text();
        if (!strName.isEmpty()) {
            emit search(strName);
        }
    });
    connect(m_pBtnSearchCancel, &QPushButton::clicked, [this] {
        m_pLineEditSearch->clear();
        emit refresh();
    });

    connect(m_pBtnDelete, &QPushButton::clicked, [this] {
        emit deleteData();
    });
    connect(m_pBtnInsert, &QPushButton::clicked, [this] {
        emit insert();
    });
    connect(m_pBtnRefresh, &QPushButton::clicked, [this] {
        emit refresh();
    });
}
