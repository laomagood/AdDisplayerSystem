#include "FilterBarWidget.h"
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

FilterBarWidget::FilterBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

void FilterBarWidget::initializeUi()
{
    m_pLineEditSearch = new QLineEdit;
    m_pLineEditSearch->setFixedWidth(300);

    m_pComboBoxFileSort = new QComboBox;
    QStringList sortName = QStringList() << tr("上传时间") << tr("使用频率") << tr("按文件名");
    m_pComboBoxFileSort->addItems(sortName);

    m_pBtnSearch = new QPushButton(tr("查询"));
    m_pBtnSearchCancel = new QPushButton(tr("取消查询"));
    m_pBtnUpload = new QPushButton(tr("上传"));
    m_pBtnDelete = new QPushButton(tr("删除"));
    m_pBtnRefresh = new QPushButton(tr("刷新"));

    m_pLineEditSearch->setPlaceholderText(tr("请输入要查询的文件名:"));

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pLineEditSearch);
    hLayout->addWidget(m_pBtnSearch);
    hLayout->addWidget(m_pBtnSearchCancel);
    hLayout->addSpacing(30);
    hLayout->addWidget(new QLabel(tr("排序:")));
    hLayout->addWidget(m_pComboBoxFileSort);
    hLayout->addStretch();
    hLayout->addWidget(m_pBtnUpload);
    hLayout->addWidget(m_pBtnDelete);
    hLayout->addSpacing(30);
    hLayout->addWidget(m_pBtnRefresh);

    this->setLayout(hLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // 设置样式表
    QFile file("./resources/qss/FilterBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void FilterBarWidget::handleEvents()
{
    // 上传
    connect(m_pBtnUpload, &QPushButton::clicked, [this] {
        // [1] 获取文件路径
        QStringList files = QFileDialog::getOpenFileNames( this, tr("选择需要添加的内容"),
                                                               "", "All (*.*);;"
                                                                        "Musics (*.mp3 *.wma *.wav);;"
                                                                        "Images (*.png *.xpm *.jpg)");
        // [2] 从数据库中(Json配置文件中)*获取文件存储地址
        qDebug() << files;

        // [3] 将文件拷贝到存储地址（重名就重命名）
        for (int i=0; i<files.size(); i++) {
            bool ok = QFile::copy("c:/users/administrator/desktop/2.png", "c:/users/administrator/desktop/10.png");
            if (ok) {

            }
        }
        // [4] 将信息存入数据库
        QMessageBox::about(this, tr("上传素材"), tr("共%1个文件，上传成功%2个素材").arg(files.size()).arg(files.size()));
    });

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
    connect(m_pBtnRefresh, &QPushButton::clicked, [this] {
        emit refresh();
    });
}
