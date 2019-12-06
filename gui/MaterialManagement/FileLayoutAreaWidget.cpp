#include "FileLayoutAreaWidget.h"
#include "QFlowLayout.h"
#include "ThumbnailWidget.h"
#include "dao/MaterialDao.h"
#include "dao/ApprovalDao.h"
#include "OpenFile/ImageViewerTool.h"
#include "OpenFile/VideoPlayerTool.h"
#include "util/TopWindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QDirIterator>
#include <QScrollArea>
#include <QtConcurrent>
#include <QMessageBox>
#include <QFileInfo>

const int c_nImageSize = 170;

// 缩放图片
QImage scaleImage(const QString &imageFileName)
{
    QImage image(imageFileName);
    return image.scaled(QSize(c_nImageSize, c_nImageSize), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

FileLayoutAreaWidget::FileLayoutAreaWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

FileLayoutAreaWidget::~FileLayoutAreaWidget()
{
    m_pWatcher->cancel();
    m_pWatcher->waitForFinished();
}

void FileLayoutAreaWidget::setApprovalBeanList(const QList<ApprovalBean> &approvalBeanList)
{

    // 如果已经加载图片，取消并进行等待
    if (m_pWatcher->isRunning()) {
        m_pWatcher->cancel();
        m_pWatcher->waitForFinished();
    }

    // 传入的文件列表必须过滤完文件类型的列表
    if (approvalBeanList.count() > this->m_nPageDataAmount) {
        qDebug() << "缩略图文件数:" << approvalBeanList.size();
        qDebug() << "传入文件数过多";
    }

    // [1] 释放 m_thumbnailList 中指针内存
    int nSize = m_thumbnailList.size();
    for (int i = 0; i < nSize; ++i) {
        ThumbnailWidget *pThumbnailWidget = m_thumbnailList.at(0);
        m_thumbnailList.removeOne(pThumbnailWidget);
        m_pCentralArea->layout()->removeWidget(pThumbnailWidget);
        pThumbnailWidget->deleteLater();
        pThumbnailWidget = nullptr;
    }
    m_thumbnailList.clear();

    // [2] 创建ThumbnailWidget, m_thumbnailList 中添加新的指针，
    for (int i = 0; i < approvalBeanList.size(); ++i) {
        auto pThumbnailWidget = new ThumbnailWidget;
        m_pCentralArea->layout()->addWidget(pThumbnailWidget);
        pThumbnailWidget->setApprovalBean(approvalBeanList.at(i));
        m_thumbnailList.append(pThumbnailWidget);

        // 打开
        connect(pThumbnailWidget, &ThumbnailWidget::openThumbnail, [this](ThumbnailWidget *pThumbnailWidget) {
            FileType fileType = pThumbnailWidget->getApprovalBean().getMaterialBean().getFileType();
            switch (fileType) {
            case FileType::VideoType:
            case FileType::AudioType:
            {
                auto pVideoPlayerTool = new VideoPlayerTool;
                pVideoPlayerTool->setFilePaths(QStringList(pThumbnailWidget->getFilePath()));
                pVideoPlayerTool->setAttribute(Qt::WA_DeleteOnClose);
                pVideoPlayerTool->show();
                break;
            }
            case FileType::ImageType:
            {
                ImageViewerTool *pImageViewerTool = new ImageViewerTool();
                pImageViewerTool->loadImage(pThumbnailWidget->getFilePath());
                TopWindow *pWindow = new TopWindow(pImageViewerTool);
                QString strFilaName = QFileInfo(pThumbnailWidget->getFilePath()).fileName();
                pWindow->setTitle(strFilaName);
                pWindow->setAttribute(Qt::WA_DeleteOnClose);
                pWindow->show();
                break;
            }
            default:
                break;
            }
        });

        // 删除缩略图
        connect(pThumbnailWidget, &ThumbnailWidget::deleteThumbnail, [this](ThumbnailWidget *pThumbnailWidget) {
            QString filePath = pThumbnailWidget->getFilePath();
            QFileInfo fileInfo(filePath);
            if (deleteThumbnail(pThumbnailWidget)) {
                QMessageBox::about(this, tr("删除成功"), tr("删除成功，文件 %1").arg(fileInfo.fileName()));
                emit ThumbnailDeleted();
            } else {
                QMessageBox::about(this, tr("删除失败"), tr("删除失败，文件 %1").arg(fileInfo.fileName()));
            }
        });
    }

    m_thumbnailImageList.clear();
    QStringList imageFilePaths;
    // [3-1] 判断类型是否为图片
    for (int i=0; i<m_thumbnailList.size(); i++) {
        if ( m_thumbnailList.at(i)->getApprovalBean().getMaterialBean().getFileType() == FileType::ImageType ) {
            imageFilePaths.append(m_thumbnailList.at(i)->getFilePath());
            m_thumbnailImageList.append(m_thumbnailList.at(i));
        }
    }
    if (imageFilePaths.isEmpty())
        return;
    // [3-2] 开启多线程，加载缩略图
    m_pWatcher->setFuture(QtConcurrent::mapped(imageFilePaths, scaleImage));
}

void FileLayoutAreaWidget::setInformationLabel(MaterialActionBarWidget::ArrangeType arrangeType)
{
    for (int i=0; i<m_thumbnailList.size(); i++) {
        m_thumbnailList.at(i)->setInformationLabel(arrangeType);
    }
}

void FileLayoutAreaWidget::setApproval(bool isApproval)
{
    for (int i=0; i<m_thumbnailList.size(); i++) {
        m_thumbnailList.at(i)->setApproval(isApproval);
    }
}

void FileLayoutAreaWidget::select(bool bIsChecked)
{
    for (int i=0; i<m_thumbnailList.size(); i++) {
        m_thumbnailList.at(i)->setChecked(bIsChecked);
    }
}

bool FileLayoutAreaWidget::isCheckedThumbnails()
{
    for (int i=0; i<m_thumbnailList.size(); i++) {
        if (m_thumbnailList.at(i)->isChecked()) {
            return true;
        }
    }
    return false;
}

int FileLayoutAreaWidget::checkedThumbnailsSize()
{
    int nSize = 0;
    for (int i=0; i<m_thumbnailList.size(); i++) {
        if (m_thumbnailList.at(i)->isChecked()) {
            nSize++;
        }
    }
    return nSize;
}

QStringList FileLayoutAreaWidget::deleteThumbnails()
{
    QStringList strDeletFiles;
    for (int i=0; i<m_thumbnailList.size(); i++) {
        if (m_thumbnailList.at(i)->isChecked()) {
            if (!deleteThumbnail(m_thumbnailList.at(i))) {
                strDeletFiles.append(m_thumbnailList.at(i)->getApprovalBean().getMaterialBean().getName());
            }
        }
    }
    return strDeletFiles;
}

void FileLayoutAreaWidget::initializeUi()
{
    // 获取分辨率(不算任务栏)
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm = screen->availableGeometry() ;
    int screenW = mm.width();
    int screenH = mm.height();

    // 放文件缩略图的widget
    m_pCentralArea  = new QWidget;
    m_pCentralArea->setFixedSize(screenW-245, screenH-200);

    // 计算出缩略图的HW数量，再计算缩略图的间隔距离
    int nWAmount = (m_pCentralArea->width()-20-180)/200+1;
    int nWSpacing = (m_pCentralArea->width()-20-nWAmount*180)/(nWAmount-1);
    int nHAmount = (m_pCentralArea->height()-20-220)/240+1;
    int nHSpacing = (m_pCentralArea->height()-20-nHAmount*220)/(nHAmount-1);
    //    qDebug() << "W数量" << nWAmount << "W间隔" << nWSpacing;
    //    qDebug() << "H数量" << nHAmount << "H间隔" << nHSpacing;

    // 间隔及缩略图总数量
    int nSpacing = nWSpacing < nHSpacing ? nWSpacing : nHSpacing;
    m_nPageDataAmount = nWAmount * nHAmount;

    // 有了间隔，然后开始布局
    auto pFlowLayout = new QFlowLayout(10, nSpacing, nSpacing);
    m_pCentralArea->setLayout(pFlowLayout);
    //    qDebug() << "窗口宽:" << m_pCentralArea->width() << "窗口高:" << m_pCentralArea->height();

    // ScrollArea用来缩小区域
    auto *pScrollArea = new QScrollArea;
    pScrollArea->setAlignment(Qt::AlignCenter);
    pScrollArea->setWidget(m_pCentralArea);

    auto vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(pScrollArea);
    this->setLayout(vLayout);

    // 初始化控件
    m_pWatcher = new QFutureWatcher<QImage>(this);
}

void FileLayoutAreaWidget::handleEvents()
{
    connect(m_pWatcher, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
}

bool FileLayoutAreaWidget::deleteThumbnail(ThumbnailWidget *pThumbnailWidget)
{
    // 删除本地文件
    QFile file(pThumbnailWidget->getFilePath());
    file.remove();

    // 看看本地文件是否存在
    QFileInfo fileInfo(pThumbnailWidget->getFilePath());
    bool ok = fileInfo.exists();

    // 删除数据库文件
    if (!ok) {
        // 数据库端使用了触发器,删除Material会一并删除MaterialApproval
        MaterialDao materialDao;
        materialDao.deleteById(pThumbnailWidget->getApprovalBean().getMaterialBean().getID());
        qDebug() << "删除成功···";
        return true;
    } else{
        qDebug() << "删除失败···";
        return false;
    }
}

void FileLayoutAreaWidget::showImage(int index)
{
    m_thumbnailImageList[index]->setPixmap(QPixmap::fromImage(m_pWatcher->resultAt(index)));
}
