#include "ProgramManagement.h"
//#include "MaterialSideBarWidget.h"
#include "../MaterialManagement/FileLayoutAreaWidget.h"
#include "../MaterialManagement/MultipageButtonsWidget.h"
#include "../MaterialManagement/MaterialActionBarWidget.h"
#include "../MaterialManagement/OpenFile/ImageViewerTool.h"
#include "../MaterialManagement/ThumbnailWidget.h"
#include "util/Util.h"
#include "dao/MaterialDao.h"
#include "dao/ApprovalDao.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QSettings>
#include <QCoreApplication>
#include <QProgressDialog>
#include <QCursor>
#include <QtConcurrent>
#include <QFuture>

ProgramManagement::ProgramManagement(QWidget *parent) :
    QWidget(parent),
    m_nCurrentPage(1)
{
    initializeUi();
    initializeData();
    loadApprovalBeanList();  // 加载
    handleEvents();
}

ProgramManagement::~ProgramManagement()
{
    m_pWatcher->cancel();
    m_pWatcher->waitForFinished();
}

void ProgramManagement::initializeUi()
{
    // [1] 左边：文件类型筛选栏（图片，视频，音频，文档···）
    // [2] 右上：文件搜索，上传栏
    // [3] 右中：文件布局区（用来预览文件）
    // [4] 右下：多页翻页按钮

    //    m_pMaterialSideBarWidget = new MaterialSideBarWidget;
    m_pMaterialActionBarWidget = new MaterialActionBarWidget;
    m_pFileLayoutAreaWidget = new FileLayoutAreaWidget;
    m_pMultipageButtonsWidget = new MultipageButtonsWidget;

    auto pRightLayout = new QVBoxLayout;
    pRightLayout->addWidget(m_pMaterialActionBarWidget);
    pRightLayout->addWidget(m_pFileLayoutAreaWidget);
    pRightLayout->addWidget(m_pMultipageButtonsWidget);
    pRightLayout->setMargin(0);
    pRightLayout->setContentsMargins(15, 15, 15, 0);

    //  右区：用于美观的外框pFrame
    auto pFrame  = new QFrame;
    pFrame->setObjectName("centreFrame");
    pFrame->setLayout(pRightLayout);
    this->setStyleSheet("#centreFrame {"
                        "background: rgb(255, 255, 255);"
                        "border:15px solid rgb(230, 230, 230);}");

    auto hMainLayout = new QHBoxLayout;
    //    hMainLayout->addWidget(m_pMaterialSideBarWidget);
    hMainLayout->addWidget(pFrame);
    this->setLayout(hMainLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    m_pImageViewerTool = new ImageViewerTool;
    m_pWatcher = new QFutureWatcher<void>(this);
}

void ProgramManagement::initializeData()
{
    // 获取配置文件中的文件夹路径config.ini
    QString strIniFilePath = QCoreApplication::applicationDirPath() + "/config" + "/config.ini";
    QFileInfo iniFileInfo(strIniFilePath);
    if(!iniFileInfo.isFile()) {
        qDebug() << "配置文件缺失";
        QMessageBox::warning(this, tr("配置文件缺失"), tr("配置文件缺失,请确认配置文件的存在···"));
        return;
    }

    QSettings settings(strIniFilePath, QSettings::IniFormat);
    settings.beginGroup("basic");
    m_strFileDir = settings.value("filepath").toString();
    settings.endGroup();

    // 读取后缀列表
    setSuffixList();
}

void ProgramManagement::handleEvents()
{
    // 侧边栏类型按钮点击
    //    connect(m_pMaterialSideBarWidget, &MaterialSideBarWidget::typeClicked, [this](FileType fileType) {
    //        loadApprovalBeanList();
    //        Q_UNUSED(fileType);
    //    });

    // 多页按钮点击
    connect(m_pMultipageButtonsWidget, &MultipageButtonsWidget::currentPage, [this](int page) {
        m_nCurrentPage = page;
        layoutAreaSetFiles();
        // 设置缩略图信息
        MaterialActionBarWidget::ArrangeType arrangeType = m_pMaterialActionBarWidget->getArrangeType();
        m_pFileLayoutAreaWidget->setInformationLabel(arrangeType);
    });

    // 刷新
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::refresh, [this] {
        loadApprovalBeanList();
    });

    // 查询
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::search, [this](const QString &name) {
        MaterialActionBarWidget::ArrangeType arrangeType = m_pMaterialActionBarWidget->getArrangeType();
        // 从数据库中加载数据
        // (根据不同的文件类型加载) 排序:名称，大小，时间，频率
        ApprovalDao approvalDao;
        m_nApprovalBeanList = approvalDao.findBy(name, arrangeType);
        if (m_nApprovalBeanList.size() == 0) {
            QMessageBox::warning(this, tr("警告"), tr("对不起没有查到名字为\"%1\"的数据").arg(name));
            return;
        }

        //  设置多页按钮页码数据
        m_nDataTotal = m_nApprovalBeanList.size();
        m_pMultipageButtonsWidget->setData(m_pFileLayoutAreaWidget->getPageDataAmount(), m_nDataTotal);
        m_nCurrentPage = 1;

        // 布局中设置缩略图
        layoutAreaSetFiles();
        // 设置缩略图信息
        m_pFileLayoutAreaWidget->setInformationLabel(arrangeType);
    });

    // 取消查询
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::refresh, [this] {
        loadApprovalBeanList();
    });

    // 上传文件
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::upload, [this] {
        // 文件类型过滤
        QString strAddi = "Addi (*." + m_nListProgramSuffix.join(" *.") + ")";
        QString strFilter = "All (*.*);;" + strAddi;

        QStringList strUpdateFilePaths = QFileDialog::getOpenFileNames( this, tr("选择需要添加的内容"), "", strFilter);
        if (strUpdateFilePaths.isEmpty()) {
            return;
        }

        // 获取配置文件中的文件夹路径config.ini
        QString strIniFilePath = QCoreApplication::applicationDirPath() + "/config" + "/config.ini";
        QFileInfo iniFileInfo(strIniFilePath);
        if(!iniFileInfo.isFile()) {
            qDebug() << "配置文件缺失";
            return;
        }

        QSettings settings(strIniFilePath, QSettings::IniFormat);
        settings.beginGroup("basic");
        QString strFilePath = settings.value("filepath").toString();
        settings.endGroup();

        strFilePath = strFilePath + "/Program";

        // map对象存放原路径与新路径
        QMap<QString, QString> mapFilePath;

        for (int i=0; i<strUpdateFilePaths.size(); i++) {
            QFileInfo fileInfo(strUpdateFilePaths.at(i));

            QString uniqueFileName = Util::getUniqueFileName(fileInfo.fileName(), strFilePath);
            mapFilePath.insert(strUpdateFilePaths.at(i), uniqueFileName);
        }

        // 多线程上传(复制文件，上传数据库)
        m_pWatcher->setFuture(QtConcurrent::run(this, copyFiles, mapFilePath));
        connect(m_pWatcher, &QFutureWatcher<void>::finished, [this] {
            qDebug() << "上传完毕";
            loadApprovalBeanList();
        });
    });

    // 排序
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::arrangeChange, [this](MaterialActionBarWidget::ArrangeType arrangeType) {
        loadApprovalBeanList();
        Q_UNUSED(arrangeType);
    });

    // 删除文件（勾选删除文件）
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::deleteData, [this] {
        // 勾选数量
        int nSize = m_pFileLayoutAreaWidget->checkedThumbnailsSize();
        if (nSize == 0) {
            QMessageBox::warning(this, tr("警告"), tr("删除失败,没有勾选任何项目···"), QMessageBox::Yes);
        } else {
            int ret = QMessageBox::warning(this, tr("确认删除"), tr("是否删除选中项?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                QStringList strListDeleteFiles = m_pFileLayoutAreaWidget->deleteThumbnails();
                if (!strListDeleteFiles.isEmpty()) {
                    // 一部分删除
                    int nDeleteSize = strListDeleteFiles.size();
                    QString strDeleteFiles = strListDeleteFiles.join("\n");

                    QMessageBox msgBox;
                    msgBox.setText(tr("共%1个文件，删除成功%2个文件，删除失败%3个文件").arg(nSize).arg(nSize - nDeleteSize).arg(nDeleteSize));
                    msgBox.setInformativeText(tr("删除失败的文件列表点击详情查看"));
                    msgBox.setDetailedText(strDeleteFiles);
                    msgBox.exec();
                } else {
                    // 全部删除
                    QMessageBox::about(this, tr("删除成功"), tr("删除成功,一共删除%1个文件···").arg(nSize));
                }

                loadApprovalBeanList();
            }
        }
    });

    // 删除文件（右键删除文件）
    connect(m_pFileLayoutAreaWidget, &FileLayoutAreaWidget::ThumbnailDeleted, [this]{
        loadApprovalBeanList();
    });

    // 全选反选（审核中）
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::select, [this](bool checked){
        m_pFileLayoutAreaWidget->select(checked);
    });

    // 审核按钮
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::approvalToggled, [this](bool bIsApproval){
        loadApprovalBeanList();
        // 退出审核,更改工作栏审核选项为全部
        if(!bIsApproval) {
            m_pMaterialActionBarWidget->setApprovalType(MaterialActionBarWidget::ApprovalAll);
        }
        m_pFileLayoutAreaWidget->setApproval(bIsApproval);
    });

    // 审核状态
    connect(m_pMaterialActionBarWidget, &MaterialActionBarWidget::approvalChange, [this](MaterialActionBarWidget::ApprovalType approvalType){
        loadApprovalBeanList();
        Q_UNUSED(approvalType);
    });
}

void ProgramManagement::setSuffixList()
{
    // 获取配置文件中的文件夹路径config.ini
    QString strIniFilePath = QCoreApplication::applicationDirPath() + "/config" + "/config.ini";
    QFileInfo iniFileInfo(strIniFilePath);
    if(!iniFileInfo.isFile()) {
        qDebug() << "配置文件缺失";
        return;
    }

    // 将配置文件的支持的后缀保存的这个类的每个变量中
    QSettings settings(strIniFilePath, QSettings::IniFormat);
    settings.beginGroup("FileType");
    m_nListProgramSuffix = settings.value("Program").toString().split("/", QString::SkipEmptyParts);
    settings.endGroup();
}

void ProgramManagement::loadApprovalBeanList()
{
    FileType fileType = FileType::ProgramType;
    MaterialActionBarWidget::ArrangeType arrangeType = m_pMaterialActionBarWidget->getArrangeType();

    // 从数据库中加载数据
    // (根据不同的文件类型加载) 排序:名称，大小，时间，频率
    ApprovalDao approvalDao;
    // 根据审核是否按下从数据库加载Bean
    if (m_pMaterialActionBarWidget->isCheckedBtnApproval()) {
        MaterialActionBarWidget::ApprovalType approvalType = m_pMaterialActionBarWidget->getApprovalType();
        qDebug() << "审核类型：" << approvalType;
        m_nApprovalBeanList = approvalDao.findBy(fileType, arrangeType, approvalType);
    } else {
        m_nApprovalBeanList = approvalDao.findBy(fileType, arrangeType);
    }

    //  设置多页按钮页码数据
    m_nDataTotal = m_nApprovalBeanList.size();
    m_pMultipageButtonsWidget->setData(m_pFileLayoutAreaWidget->getPageDataAmount(), m_nDataTotal);
    //    m_nCurrentPage = 1;
    qDebug() << "缩略图数量:" << m_nDataTotal;

    // 布局中设置缩略图
    layoutAreaSetFiles();

    // 设置缩略图信息
    m_pFileLayoutAreaWidget->setInformationLabel(arrangeType);

    // 审核中
    if (m_pMaterialActionBarWidget->isCheckedBtnApproval()) {
        m_pFileLayoutAreaWidget->setApproval(true);
    }
}

void ProgramManagement::layoutAreaSetFiles()
{
    // 界面缩略图数量
    int nPageDataAmount = m_pFileLayoutAreaWidget->getPageDataAmount();

    // [1] 文件不到一页布局数
    if( m_nDataTotal <= nPageDataAmount) {
        m_pFileLayoutAreaWidget->setApprovalBeanList(m_nApprovalBeanList);
    }
    // [2] 文件超过一页布局数
    else {
        QList<ApprovalBean> nList;
        int nStarteNumber = (m_nCurrentPage-1) * nPageDataAmount;   // 开始序号
        int nEndNumber = nStarteNumber + nPageDataAmount;           // 结束序号
        if (nEndNumber > m_nDataTotal) {
            nEndNumber = m_nDataTotal;
        }

        for(int i = nStarteNumber; i < nEndNumber; i++) {
            nList.append(m_nApprovalBeanList.at(i));
        }
        qDebug() << nList.size();
        m_pFileLayoutAreaWidget->setApprovalBeanList(nList);
    }
}

void ProgramManagement::copyFiles(QMap<QString, QString> mapFilePath)
{
    // 文件原路径与新路径：拷贝文件，上传数据库
    QFileInfo fileInfo;
    QMap<QString, QString>::const_iterator i = mapFilePath.constBegin();
    while (i != mapFilePath.constEnd()) {
        qDebug() << i.key() << i.value();

        // [1]将文件拷贝到存储地址
        bool ok = QFile::copy(i.key(), i.value());
        // [2] 将文件信息上传到数据库
        if (ok) {
            fileInfo.setFile(i.value());
            MaterialBean materialBean;
            materialBean.setName(fileInfo.fileName());
            materialBean.setFileType(FileType::ProgramType);
            materialBean.setSize(fileInfo.size());

            // 数据库端使用了触发器,上传Material会一并上传MaterialApproval
            MaterialDao materialDao;
            materialDao.insert(materialBean);
        }
        ++i;
    }
}

