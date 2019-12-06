#include "MaterialActionBarWidget.h"
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>

MaterialActionBarWidget::MaterialActionBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

MaterialActionBarWidget::ArrangeType MaterialActionBarWidget::getArrangeType()
{
    return (ArrangeType)m_pComboBoxFileSort->currentIndex();
}

MaterialActionBarWidget::ApprovalType MaterialActionBarWidget::getApprovalType()
{
    return (ApprovalType)m_pComboBoxApproval->currentData().toInt();
}

bool MaterialActionBarWidget::isCheckedBtnApproval()
{
    return m_pBtnApproval->isChecked();
}

void MaterialActionBarWidget::setApprovalType(MaterialActionBarWidget::ApprovalType ApprovalType)
{
    switch (ApprovalType) {
    case ApprovalAll:
        m_pComboBoxApproval->setCurrentIndex(0);
        break;
    case ApprovalNot:
        m_pComboBoxApproval->setCurrentIndex(1);
        break;
    case ApprovalFail:
        m_pComboBoxApproval->setCurrentIndex(2);
        break;
    case ApprovalPass:
        m_pComboBoxApproval->setCurrentIndex(3);
        break;
    default:
        break;
    }
}

void MaterialActionBarWidget::initializeUi()
{
    m_pLineEditSearch = new QLineEdit;
    m_pLineEditSearch->setFixedWidth(200);

    m_pComboBoxApproval = new QComboBox;
    m_pComboBoxApproval->addItem(tr("全部"), ApprovalAll);
    m_pComboBoxApproval->addItem(tr("未审核"), ApprovalNot);
    m_pComboBoxApproval->addItem(tr("审核不通过"),ApprovalFail);
    m_pComboBoxApproval->addItem(tr("审核通过"), ApprovalPass);

    m_pComboBoxApproval->hide();

    m_pComboBoxFileSort = new QComboBox;
    QStringList sortName = QStringList() << tr("上传时间") << tr("使用频率") << tr("按文件名") << tr("按文件大小");
    m_pComboBoxFileSort->addItems(sortName);

    m_pLabelApproval = new QLabel(tr("筛选:"));

    m_pBtnSearch = new QPushButton(tr("查询"));
    m_pBtnSearchCancel = new QPushButton(tr("取消查询"));
    m_pBtnApproval = new QPushButton(tr("进入审核"));
    m_pBtnUpload = new QPushButton(tr("上传"));
    m_pBtnSelectAll = new QPushButton(tr("全选"));
    m_pBtnDelete = new QPushButton(tr("删除"));
    m_pBtnRefresh = new QPushButton(tr("刷新"));

    m_pLineEditSearch->setPlaceholderText(tr("请输入要查询的文件名:"));

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pLineEditSearch);
    hLayout->addWidget(m_pBtnSearch);
    hLayout->addWidget(m_pBtnSearchCancel);
//    hLayout->addSpacing(30);
    hLayout->addWidget(m_pLabelApproval);
    hLayout->addWidget(m_pComboBoxApproval);
    hLayout->addWidget(m_pComboBoxFileSort);
    hLayout->addStretch();
    hLayout->addWidget(m_pBtnApproval);
    hLayout->addWidget(m_pBtnUpload);
    hLayout->addWidget(m_pBtnSelectAll);
    hLayout->addWidget(m_pBtnDelete);
    hLayout->addSpacing(30);
    hLayout->addWidget(m_pBtnRefresh);

    m_pBtnSelectAll->hide();
    m_pBtnSelectAll->setCheckable(true);
    m_pBtnApproval->setCheckable(true);

    this->setLayout(hLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // 设置样式表
    QFile file("./resources/qss/MaterialActionBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void MaterialActionBarWidget::handleEvents()
{
    // 上传
    connect(m_pBtnUpload, &QPushButton::clicked, [this] {
        emit upload();
    });
    // 查询
    connect(m_pLineEditSearch, &QLineEdit::returnPressed, [this] {
        m_pBtnSearch->click();
    });
    // 搜索
    connect(m_pBtnSearch, &QPushButton::clicked, [this] {
        QString strName = m_pLineEditSearch->text();
        if (!strName.isEmpty()) {
            emit search(strName);
        }
    });
    // 取消查询
    connect(m_pBtnSearchCancel, &QPushButton::clicked, [this] {
        m_pLineEditSearch->clear();
        emit refresh();
    });
    // 删除
    connect(m_pBtnDelete, &QPushButton::clicked, [this] {
        emit deleteData();
    });
    // 刷新
    connect(m_pBtnRefresh, &QPushButton::clicked, [this] {
        emit refresh();
    });

    // 文件审核状态
    connect(m_pComboBoxApproval, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        // 全部：-1, 未审核：0，通过：1，不通过：2
        Q_UNUSED(index)
        int nApproval = m_pComboBoxApproval->currentData().toInt();
        emit approvalChange((ApprovalType)nApproval);
    });

    // 文件排序
    connect(m_pComboBoxFileSort, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        emit arrangeChange((ArrangeType)index);
    });

    // 全选反选
    connect(m_pBtnSelectAll, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            m_pBtnSelectAll->setText(tr("反选"));
        } else {
            m_pBtnSelectAll->setText(tr("全选"));
        }
        emit select(checked);
    });

    // 审核
    connect(m_pBtnApproval, &QPushButton::toggled, [this](bool bIsApproval) {
        if (bIsApproval) {
            // 审核
            m_pBtnApproval->setText("退出审核");
            m_pLineEditSearch->hide();
            m_pBtnSearch->hide();
            m_pBtnSearchCancel->hide();
            m_pBtnUpload->hide();
            m_pBtnSelectAll->show();
            m_pLabelApproval->show();
            m_pComboBoxApproval->show();
            m_pComboBoxFileSort->hide();
        } else {
            // 退出
            m_pBtnApproval->setText("进入审核");
            m_pLineEditSearch->show();
            m_pBtnSearch->show();
            m_pBtnSearchCancel->show();
            m_pBtnUpload->show();
            m_pBtnSelectAll->hide();
            m_pLabelApproval->hide();
            m_pComboBoxApproval->hide();
            m_pComboBoxFileSort->show();
        }
        emit approvalToggled(bIsApproval);
    });
}
