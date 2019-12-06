#include "UserActionBarWidget.h"
#include "UserManagementModel.h"
#include "RoleUniqueProxyModel.h"
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

UserActionBarWidget::UserActionBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

void UserActionBarWidget::setModel(QAbstractItemModel *sourceModel)
{
    m_pRoleUniqueProxyModel->setSourceModel(sourceModel);
    createComboBoxModel(m_pComboBoxRole, (int)UserManagementModel::Column::Role);
    connect(sourceModel, &QAbstractItemModel::modelReset, [this] {
        m_pComboBoxRole->setCurrentIndex(0);
    });
}

QString UserActionBarWidget::getComboBoxText() const
{
    return m_pComboBoxRole->currentText();
}

bool UserActionBarWidget::isCheckBoxClicked() const
{
    return m_pCheckBoxRole->isChecked();
}

void UserActionBarWidget::initializeUi()
{
    m_pLineEditSearch = new QLineEdit;
    m_pLineEditSearch->setFixedWidth(200);
    m_pLineEditSearch->setPlaceholderText(tr("请输入要查询的用户名:"));
    m_pBtnSearch = new QPushButton(tr("查询"));
    m_pBtnSearchCancel = new QPushButton(tr("取消查询"));
    m_pCheckBoxRole = new QCheckBox(tr("启用角色筛选"));
    m_pComboBoxRole = new QComboBox();
    m_pComboBoxRole->setEnabled(false);

    m_pBtnInsert = new QPushButton(tr("新增"));
    m_pBtnDelete = new QPushButton(tr("删除"));
    m_pBtnRefresh = new QPushButton(tr("刷新"));

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pLineEditSearch);
    hLayout->addWidget(m_pBtnSearch);
    hLayout->addWidget(m_pBtnSearchCancel);
    hLayout->addSpacing(30);
    hLayout->addWidget(m_pCheckBoxRole);
    hLayout->addWidget(m_pComboBoxRole);

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

void UserActionBarWidget::initializeData()
{
    m_pRoleUniqueProxyModel = new RoleUniqueProxyModel((int)UserManagementModel::Column::Role, this);
}

void UserActionBarWidget::handleEvents()
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

    connect(m_pCheckBoxRole, &QCheckBox::clicked, [this](bool checked) {
        if (checked) {
            m_pComboBoxRole->setEnabled(true);
        } else {
            m_pComboBoxRole->setEnabled(false);
        }
        emit roleCheckClicked(checked);
    });

    connect(m_pComboBoxRole, &QComboBox::currentTextChanged, [this](const QString &text) {
        emit roleComboBoxTextChanged(text);
    });
}

void UserActionBarWidget::createComboBoxModel(QComboBox *comboBox, int column)
{
    qDebug() << "createComboBoxModel";
    delete comboBox->model();
//    m_pRoleUniqueProxyModel->sort(column, Qt::AscendingOrder);
    comboBox->setModel(m_pRoleUniqueProxyModel);
    comboBox->setModelColumn(column);
}
