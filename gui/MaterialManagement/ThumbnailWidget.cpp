#include "ThumbnailWidget.h"
#include "util/Json.h"
#include "dao/ApprovalDao.h"
#include "dao/MaterialDao.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QFileInfo>
#include <QEvent>
#include <QTime>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QCoreApplication>
#include <QSettings>
#include <QMouseEvent>

ThumbnailWidget::ThumbnailWidget(QWidget *parent) : QWidget(parent),
    m_bIsApproval(false)
{
    initializeUi();
    setApproval(false);
    //    handleEvents();
}

void ThumbnailWidget::setApprovalBean(const ApprovalBean &approvalBean)
{
    m_approvalBean = approvalBean;
    setFilePath();  // 文件路径(实际路径)
    setTypeIcon();  // 类型图标
    m_pNameLabel->setText(m_approvalBean.getMaterialBean().getName());   // 文件名
}

void ThumbnailWidget::setChecked(bool isChecked)
{
    m_pCheckBox->setChecked(isChecked);
}

bool ThumbnailWidget::isChecked() const
{
    return m_pCheckBox->isChecked();
}

void ThumbnailWidget::setPixmap(const QPixmap &pixmap)
{
    if (m_approvalBean.getMaterialBean().getFileType() == FileType::ImageType) {
        p_thumbnailLabel->setPixmap(pixmap);
    }
}

void ThumbnailWidget::setInformationLabel(MaterialActionBarWidget::ArrangeType arrangeType)
{
    qint64 nSize;
    switch (arrangeType) {
    case MaterialActionBarWidget::ArrangeTime:
        m_pInformationLabel->setText(tr("时间: ") + m_approvalBean.getMaterialBean().getStrGenerateTime());
        break;
    case MaterialActionBarWidget::ArrangeFrequency:
        m_pInformationLabel->setText(tr("次数: ") + QString::number(m_approvalBean.getMaterialBean().getFrequency()));
        break;
    case MaterialActionBarWidget::ArrangeName:
    case MaterialActionBarWidget::ArrangeSize:
        nSize = (qint64)m_approvalBean.getMaterialBean().getSize();
        m_pInformationLabel->setText(tr("大小: ") + this->size(nSize));
        break;
    default:
        break;
    }
}

void ThumbnailWidget::setApproval(bool bIsApprova)
{
    m_bIsApproval = bIsApprova;
    if (m_bIsApproval) {
        // 更改鼠标样式为印章
        QCursor myCursor(QPixmap("./image/ThumbnailWidget/Stamp.png"), -1, -1);
        this->setCursor(myCursor);

        // 设置审核状态图标
        setApprovalStatusLabel();
        m_pApprovalStatusLabel->show();
    } else {
        this->setCursor(Qt::ArrowCursor);
        m_pApprovalStatusLabel->hide();
    }
}

bool ThumbnailWidget::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    // 双击
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (m_bIsApproval) {
            return true;
        }
        emit openThumbnail(this);
        return true;
    }

    // 单击
    if (event->type() == QEvent::MouseButtonPress) {
        if (m_bIsApproval) {
            changeApprovalStatus();
        }
        return true;
    }
    return false;
}

void ThumbnailWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)
    if (m_bIsApproval) {
        return;
    }
    QMenu *menu = new QMenu(this);
    QAction *openFileAct = new QAction(tr("打开"));
    QAction *renameAct = new QAction(tr("重命名"));
    QAction *removeAct = new QAction(tr("删除"));

    menu->addAction(openFileAct);
    menu->addAction(renameAct);
    menu->addAction(removeAct);

    // 打开
    connect(openFileAct, &QAction::triggered, [=] {
        qDebug() << "打开文件";
        emit openThumbnail(this);
    });

    // 删除
    // !!!电脑中的节目和此文件关联?
    connect(removeAct, &QAction::triggered, [=] {
        QString fileName = m_pNameLabel->text();
        int ret = QMessageBox::warning(this, "warning", tr("你确定要删除文件 %1 ？").arg(fileName),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            emit deleteThumbnail(this);
        }
    });

    // 重命名
    // !!!电脑中的节目和此文件关联?
    connect(renameAct, &QAction::triggered, [=] {
        bool ok = false;
        QString oldFileName = m_pNameLabel->text();
        QFileInfo iniFileInfo(m_strFilePath);
        QString newFileName = QInputDialog::getText(this, tr("重命名"), tr("将 %1 文件名更改为:").arg(oldFileName),
                                                    QLineEdit::Normal, iniFileInfo.completeBaseName(), &ok);
        // 新旧名字相同，直接return
        if (newFileName == iniFileInfo.completeBaseName()) {
            return;
        }

        if (ok && !newFileName.isEmpty()) {
            QString newFilePath = iniFileInfo.path() + "/" + newFileName + "." + iniFileInfo.suffix();
            iniFileInfo.setFile(newFilePath);
            // 文件名已经存在
            if (iniFileInfo.exists()) {
                QMessageBox::warning(this, "warning", tr("文件名已经存在,重命名文件失败,"), QMessageBox::Yes);
                return;
            }
            // 文件正在被使用
            bool bRename = QFile::rename(m_strFilePath, newFilePath);
            if (bRename) {
                MaterialDao materialDao;
                materialDao.updateBy(m_approvalBean.getMaterialBean().getID(), iniFileInfo.fileName());
                this->m_pNameLabel->setText(iniFileInfo.fileName());
            } else {
                QMessageBox::warning(this, "warning", tr("文件正在使用,重命名文件失败"), QMessageBox::Yes);
            }
        }
    });

    menu->exec(QCursor::pos());
}

void ThumbnailWidget::initializeUi()
{
    //设置背景颜色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(220, 220, 220));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setFixedSize(180, 220);
    auto p_widget = new QWidget;
    p_widget->setFixedSize(width()-10, width()-10);
    p_widget->setStyleSheet("background: rgb(90, 90, 90)");
    p_widget->setObjectName("thumbnail");

    // 缩略图
    p_thumbnailLabel= new QLabel;
    auto hThumbnailLayout = new QHBoxLayout;
    hThumbnailLayout->addStretch();
    hThumbnailLayout->addWidget(p_thumbnailLabel);
    hThumbnailLayout->addStretch();
    p_widget->setLayout(hThumbnailLayout);
    p_widget->layout()->setContentsMargins(0, 0, 0, 0);
    p_widget->installEventFilter(this);

    m_pNameLabel = new QLabel(tr("未加载"));
    m_pInformationLabel = new QLabel(tr("大小:0.0KB"));

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pInformationLabel);
    hLayout->addStretch();

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(p_widget);
    vLayout->addWidget(m_pNameLabel);
    vLayout->addLayout(hLayout);

    this->setLayout(vLayout);
    this->layout()->setContentsMargins(5, 5, 5, 5);

    // 选择框与类型图标
    m_pCheckBox = new QCheckBox(this);
    m_pCheckBox->setGeometry(width()-20, 0, 30, 30);

    p_fileTypeIcon = new QLabel(this);
    p_fileTypeIcon->setGeometry(width()-40, width()-40, 35, 35);
    p_fileTypeIcon->setMouseTracking(false);
    p_fileTypeIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // 文件信息样式
    m_pInformationLabel->setStyleSheet("color:#003377; font-family:微软雅黑; font-weight:bold;");

    // 审核图标
    m_pApprovalStatusLabel = new QLabel(this);
    m_pApprovalStatusLabel->setGeometry(40, 40, 100, 100);
    m_pApprovalStatusLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

QString ThumbnailWidget::size(qint64 bytes)
{
    QString strUnit;
    double dSize = bytes * 1.0;
    if (dSize <= 0) {
        dSize = 0.0;
    } else if (dSize < 1024) {
        strUnit = "Bytes";
    } else if (dSize < 1024 * 1024) {
        dSize /= 1024;
        strUnit = "KB";
    } else if (dSize < 1024 * 1024 * 1024) {
        dSize /= (1024 * 1024);
        strUnit = "MB";
    } else {
        dSize /= (1024 * 1024 * 1024);
        strUnit = "GB";
    }

    return QString("%1%2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
}

QPixmap ThumbnailWidget::readProgramThumbnail()
{
    QPixmap pix;
    QFile file(m_strFilePath);
    if (file.open(QFile::ReadOnly)) {
        QByteArray nArray = file.readAll();
        QString strJson(nArray);

        Json nJson(strJson, false);
        QString strThumbnail = nJson.getString("Thumbnail");

        QByteArray byte_array = QByteArray::fromBase64(strThumbnail.toLocal8Bit());
        QDataStream data_stream(&byte_array, QIODevice::ReadOnly);
        data_stream >> pix;
    }
    return pix;
}

void ThumbnailWidget::setFilePath()
{
    // 获取配置文件中的文件夹路径config.ini
    QString strIniFilePath = QCoreApplication::applicationDirPath() + "/config" + "/config.ini";
    QFileInfo iniFileInfo(strIniFilePath);
    if(!iniFileInfo.isFile()) {
        qDebug() << "配置文件缺失";
        QMessageBox::warning(this, tr("配置文件缺失"), tr("配置文件缺失,请确认配置文件的存在···"));
        return;
    }

    // 配置文件中的路径
    QSettings settings(strIniFilePath, QSettings::IniFormat);
    settings.beginGroup("basic");
    m_strFilePath = settings.value("filepath").toString();
    settings.endGroup();

    // 拼合地址(配置文件中的路径 + 类型文件夹 + 数据库中的名字)
    // ./files + /Source/Image/ + timg.jpg
    FileType fileType = m_approvalBean.getMaterialBean().getFileType();
    switch (fileType) {
    case FileType::ProgramType:
        m_strFilePath = m_strFilePath + "/Source/Program";
        break;
    case FileType::VideoType:
        m_strFilePath = m_strFilePath + "/Source/Video";
        break;
    case FileType::AudioType:
        m_strFilePath = m_strFilePath + "/Source/Audio";
        break;
    case FileType::ImageType:
        m_strFilePath = m_strFilePath + "/Source/Image";
        break;
    case FileType::DocumentType:
        m_strFilePath = m_strFilePath + "/Source/Document";
        break;
    case FileType::OtherType:
        m_strFilePath = m_strFilePath + "/Source/Other";
        break;
    default:
        break;
    }
    m_strFilePath = m_strFilePath + "/" + m_approvalBean.getMaterialBean().getName();

    // 判断文件是否存在，不存在则为空
    QFileInfo fileInfo(m_strFilePath);
    if (!fileInfo.exists()) {
        m_strFilePath.clear();
    }
}

void ThumbnailWidget::setTypeIcon()
{
    FileType fileType = this->m_approvalBean.getMaterialBean().getFileType();
    int backgroundW = width()-10;
    QPixmap nPixmap;

    switch (fileType) {
    case FileType::ImageType:
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Image.png"));
        break;
    case FileType::VideoType:
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Video.png"));
        p_thumbnailLabel->setPixmap(QPixmap("./image/ThumbnailWidget/Video-background.png"));
        break;
    case FileType::AudioType:
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Audio.png"));
        p_thumbnailLabel->setPixmap(QPixmap("./image/ThumbnailWidget/Audio-background"));
        break;
    case FileType::DocumentType:
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Audio.png"));
        break;
    case FileType::OtherType:
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Audio.png"));
        break;
    case FileType::ProgramType:
        // 读取界面的缩略图
        nPixmap = readProgramThumbnail();
        p_thumbnailLabel->setPixmap(nPixmap.scaled(backgroundW, backgroundW, Qt::KeepAspectRatio));
        p_fileTypeIcon->setPixmap(QPixmap("./image/ThumbnailWidget/Program.png"));
        break;
    default:
        break;
    }
}

void ThumbnailWidget::changeApprovalStatus()
{
    // 点击审核状态切换
    if (m_approvalBean.getApprovalType() == ApprovalBean::ApprovalNot) {
        m_approvalBean.setApprovalType(ApprovalBean::ApprovalPass);
    } else if (m_approvalBean.getApprovalType() == ApprovalBean::ApprovalPass) {
        m_approvalBean.setApprovalType(ApprovalBean::ApprovalFail);
    } else {
        m_approvalBean.setApprovalType(ApprovalBean::ApprovalNot);
    }

    // 数据库更新审核状态，时间
    qint64 time = QDateTime::currentDateTime().toSecsSinceEpoch();
    m_approvalBean.setApprovalTime(time);
    m_approvalBean.setApprovalType(m_approvalBean.getApprovalType());

    qDebug() << m_approvalBean.getApprovalTime()
             << m_approvalBean.getApprovalType()
             << m_approvalBean.getID();

    ApprovalDao approvalDao;
    approvalDao.updateBy(m_approvalBean);

    // 设置审核图标
    setApprovalStatusLabel();

}

void ThumbnailWidget::setApprovalStatusLabel()
{
    // 绘制图标
    switch (m_approvalBean.getApprovalType()) {
    case ApprovalBean::ApprovalPass:
        m_pApprovalStatusLabel->setPixmap(QPixmap("./image/ThumbnailWidget/pass.png"));
        break;
    case ApprovalBean::ApprovalFail:
        m_pApprovalStatusLabel->setPixmap(QPixmap("./image/ThumbnailWidget/fail.png"));
        break;
    case ApprovalBean::ApprovalNot:
        m_pApprovalStatusLabel->setPixmap(QPixmap(""));
        break;
    default:
        break;
    }
}
