#include "MultipageButtonsWidget.h"
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QButtonGroup>
#include <QIntValidator>
#include <QtMath>

MultipageButtonsWidget::MultipageButtonsWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
    setData(-1, -1);
}

void MultipageButtonsWidget::setData(int pageDataAmount, int dataTotal)
{
    m_nPageDataAmount = pageDataAmount;
    m_nDataTotal = dataTotal;
    m_nPageTotal = qCeil((float)((float)dataTotal / (float)pageDataAmount));
    if (m_nPageTotal == 0) {
        m_nPageTotal = 1;
    }
    m_nCurrentPage = 1;
    m_nTargetpage = 1;
    m_pPageLineEdit->setValidator(new QIntValidator(1, m_nPageTotal, this));
    m_pDataInformation->setText(tr("共 %1 页 / 合计 %3 条数据").arg(m_nPageTotal)
                                .arg(m_nDataTotal));
    initializeData();
}

void MultipageButtonsWidget::initializeUi()
{
    m_pBtnPgUp      = new QPushButton("<");
    m_pBtnPgDn      = new QPushButton(">");
    m_pBtnFirst     = new QPushButton("1");
    m_pBtnSecond    = new QPushButton("2");
    m_pBtnThird     = new QPushButton("3");
    m_pBtnHomePg    = new QPushButton(tr("首页"));
    m_pBtnEndPg     = new QPushButton(tr("尾页"));

    m_pBtnJump      = new QPushButton(tr("跳转"));
    m_pDataInformation= new QLabel(tr("共 %1 页 / 合计 %3 条数据").arg(m_nPageTotal)
                                   .arg(m_nDataTotal));
    m_pPageLineEdit = new QLineEdit;

    m_pBtnHomePg->setMaximumWidth(50);
    m_pBtnEndPg->setMaximumWidth(50);
    m_pBtnPgUp->setMaximumWidth(25);
    m_pBtnPgDn->setMaximumWidth(25);
    m_pBtnFirst->setMaximumWidth(25);
    m_pBtnSecond->setMaximumWidth(25);
    m_pBtnThird->setMaximumWidth(25);

    auto hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(m_pDataInformation);
    hLayout->addSpacing(20);
    hLayout->addWidget(m_pBtnHomePg);
    hLayout->addWidget(m_pBtnPgUp);
    hLayout->addWidget(m_pBtnFirst);
    hLayout->addWidget(m_pBtnSecond);
    hLayout->addWidget(m_pBtnThird);
    hLayout->addWidget(m_pBtnPgDn);
    hLayout->addWidget(m_pBtnEndPg);

    hLayout->addSpacing(20);
    hLayout->addWidget(m_pPageLineEdit);
    hLayout->addWidget(m_pBtnJump);
    this->setLayout(hLayout);
    this->setContentsMargins(0, 0, 0, 0);

    m_pPageLineEdit->setPlaceholderText(tr("页码"));
    m_pPageLineEdit->setMaximumWidth(75);

    m_pGroupButtons = new QButtonGroup(this);
    m_pGroupButtons->addButton(m_pBtnFirst);
    m_pGroupButtons->addButton(m_pBtnSecond);
    m_pGroupButtons->addButton(m_pBtnThird);
    m_pGroupButtons->setExclusive(true);

    for (int i=0; i<m_pGroupButtons->buttons().size(); i++) {
        m_pGroupButtons->buttons().at(i)->setCheckable(true);
    }
    m_pBtnFirst->setChecked(true);
}

void MultipageButtonsWidget::initializeData()
{
    if (m_nPageTotal == 1) {
        m_pBtnFirst->show();
        m_pBtnSecond->hide();
        m_pBtnThird->hide();
        m_nCurrentPage = 1;
        m_nTargetpage = 1;
        m_pBtnFirst ->setText(QString::number(1));
        m_pBtnFirst ->setChecked(true);
    } else if (m_nPageTotal == 2) {
        m_pBtnFirst->show();
        m_pBtnSecond->show();
        m_pBtnThird->hide();
        m_pBtnFirst ->setText(QString::number(1));
        m_pBtnSecond->setText(QString::number(2));
        if (m_nTargetpage == 1 ) {
            m_nCurrentPage = m_nTargetpage;
            m_pBtnFirst ->setChecked(true);
        } else if (m_nTargetpage == 2 ) {
            m_nCurrentPage = m_nTargetpage;
            m_pBtnSecond ->setChecked(true);
        } else if (m_nTargetpage < 1){
            m_nTargetpage = 1;
        } else if (m_nTargetpage > m_nPageTotal) {
            m_nTargetpage = m_nPageTotal;
        }
    } else  {
        m_pBtnFirst->show();
        m_pBtnSecond->show();
        m_pBtnThird->show();
        if (m_nTargetpage >= 1 && m_nTargetpage <= m_nPageTotal) {
            m_nCurrentPage = m_nTargetpage;
            if (m_nTargetpage == 1) {
                m_pBtnFirst ->setChecked(true);
                m_pBtnFirst ->setText(QString::number(1));
                m_pBtnSecond->setText(QString::number(2));
                m_pBtnThird ->setText(QString::number(3));
            } else if (m_nTargetpage == m_nPageTotal) {
                m_pBtnThird ->setChecked(true);
                m_pBtnFirst ->setText(QString::number(m_nPageTotal-2));
                m_pBtnSecond->setText(QString::number(m_nPageTotal-1));
                m_pBtnThird ->setText(QString::number(m_nPageTotal));
            } else {
                m_pBtnSecond->setChecked(true);
                m_pBtnFirst ->setText(QString::number(m_nCurrentPage-1));
                m_pBtnSecond->setText(QString::number(m_nCurrentPage));
                m_pBtnThird ->setText(QString::number(m_nCurrentPage+1));
            }
        } else if (m_nTargetpage < 1){
            m_nTargetpage = 1;
        } else if (m_nTargetpage > m_nPageTotal) {
            m_nTargetpage = m_nPageTotal;
        }
    }
    // 发送当前页码信号
    emit currentPage(m_nCurrentPage);
}

void MultipageButtonsWidget::handleEvents()
{
    // 上一页，下一页
    connect(m_pBtnPgDn, &QPushButton::clicked, [this] {
        m_nTargetpage ++;
        initializeData();
    });
    connect(m_pBtnPgUp, &QPushButton::clicked, [this] {
        m_nTargetpage --;
        initializeData();
    });

    // 页码按钮
    connect(m_pGroupButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
        if (button == m_pBtnFirst) {
            int nDValue = m_nCurrentPage - m_pBtnFirst->text().toInt();
            m_nTargetpage -= nDValue;
            initializeData();
        } else if (button == m_pBtnSecond) {
            int nDValue = m_pBtnSecond->text().toInt() - m_nCurrentPage;
            m_nTargetpage += nDValue;
            initializeData();
        } else if (button == m_pBtnThird) {
            int nDValue = m_pBtnThird->text().toInt() - m_nCurrentPage;
            m_nTargetpage += nDValue;
            initializeData();
        }
    });

    // 首页，尾页
    connect(m_pBtnHomePg, &QPushButton::clicked, [this] {
        m_nTargetpage = 1;
        initializeData();
    });
    connect(m_pBtnEndPg, &QPushButton::clicked, [this] {
        m_nTargetpage = m_nPageTotal;
        initializeData();
    });

    // 跳转
    connect(m_pPageLineEdit, &QLineEdit::returnPressed, [this] {
        if (m_pPageLineEdit->text().isEmpty()) {
            return;
        } else {
            m_nTargetpage = m_pPageLineEdit->text().toInt();
            m_pPageLineEdit->clear();
            initializeData();
        }
    });
    connect(m_pBtnJump, &QPushButton::clicked, [this] {
        if (m_pPageLineEdit->text().isEmpty()) {
            return;
        } else {
            m_nTargetpage = m_pPageLineEdit->text().toInt();
            m_pPageLineEdit->clear();
            initializeData();
        }
    });
}
