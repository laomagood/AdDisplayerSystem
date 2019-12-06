#include "TransferTableWidget.h"
#include "ui_TransferTableWidget.h"
#include "util/Util.h"

TransferTableWidget::TransferTableWidget(QAbstractItemModel *model, QWidget *parent) :
    m_pModel(model),QWidget(parent),
    ui(new Ui::TransferTableWidget)
{
    ui->setupUi(this);
    this->resize(800, 400);

    // 模型
    if (m_pModel) {
        ui->tableView->setModel(m_pModel);
    }

    // 视图------------------------------------------------------------------
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);                   // 选择模式
    ui->tableView->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);   // 选择行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);                       // 不可编辑
    //    ui->tableView->setShowGrid(false);                                                       // 设置不显示格子线
    ui->tableView->horizontalHeader()->setHighlightSections(false);                          // 点击表头不高亮
    ui->tableView->verticalHeader()->hide();                                                 // 设置垂直头不可见
    ui->tableView->setFrameShape(QFrame::NoFrame);                                           // 设置无边框
    ui->tableView->horizontalHeader()->setFixedHeight(30);                                   // 设置表头的高度
    ui->tableView->setMouseTracking(true);

    // [2] 设置列宽
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 170);

    // 行高
    ui->tableView->verticalHeader()->setDefaultSectionSize(35);

    // 设置样式
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setStyleSheet( "QTableView{ background-color: rgb(255, 255, 255);"
                                       "alternate-background-color: rgb(230, 230, 255);"
                                       "selection-background-color:rgb(65, 180, 80);}" );

    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{ background:#1E90FF;"
                                                          "border:0px solid #E0DDDC;"
                                                          "border-bottom:1px solid #E0DDDC;"
                                                          "height:40 };");

    //设置表头字体加粗
    QFont font = ui->tableView->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView->horizontalHeader()->setFont(font);

    connect(ui->btnOk, &QPushButton::clicked, [this] {
        this->hide();
    });
}

TransferTableWidget::~TransferTableWidget()
{
    delete ui;
}

void TransferTableWidget::setName(QString name)
{
    ui->labelName->setText(name);
}

void TransferTableWidget::setSize(qint64 size)
{
    QString strSize = Util::sizeToString(size);
    ui->labelSize->setText(strSize);
}

void TransferTableWidget::setFileNum(int num)
{
    ui->labelNumber->setText(QString::number(num));
}

void TransferTableWidget::setModel(QAbstractItemModel *model)
{
    m_pModel = model;
    ui->tableView->setModel(m_pModel);
}

void TransferTableWidget::setIndexWidget(const QModelIndex &index, QWidget *widget)
{
    ui->tableView->setIndexWidget(index, widget);
}

void TransferTableWidget::scrollToTop()
{
    ui->tableView->scrollToTop();
}
