#ifndef MATERIALMANAGEMENT_H
#define MATERIALMANAGEMENT_H

#include <QWidget>
#include <QFutureWatcher>
#include "global.h"
#include "bean/ApprovalBean.h"
#include "MaterialActionBarWidget.h"

class MaterialSideBarWidget;
//class MaterialActionBarWidget;
class FileLayoutAreaWidget;
class MultipageButtonsWidget;
class ImageViewerTool;

class MaterialManagement : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialManagement(QWidget *parent = nullptr);
    ~MaterialManagement();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理
    void setSuffixList();                                   // 设置后缀列表
    FileType setFileType(const QString &suffix);            // 设置文件类型(上传类型判断)

    void loadApprovalBeanList();                            // 从数据库中加载ApprovalBean
    void layoutAreaSetFiles();                              // 布局中加载缩略图(页码)

    void copyFiles(QMap<QString, QString> mapFilePath);     // 多线程拷贝文件并上传数据库

    MaterialSideBarWidget *m_pMaterialSideBarWidget;        // 侧边栏
    MaterialActionBarWidget *m_pMaterialActionBarWidget;    // 菜单栏
    FileLayoutAreaWidget *m_pFileLayoutAreaWidget;          // 文件发布区
    MultipageButtonsWidget *m_pMultipageButtonsWidget;      // 多页按钮栏

    QString m_strFileDir;                                   // 文件路径
    QList<ApprovalBean> m_nApprovalBeanList;                // ApprovalBean列表

    QStringList m_nListProgramSuffix;                       // 节目后缀
    QStringList m_nListImageSuffix;                         // 图片后缀
    QStringList m_nListVideoSuffix;                         // 视频后缀
    QStringList m_nListAudioSuffix;                         // 音频后缀
    QStringList m_nListDocumentSuffix;                      // 文档后缀
    QStringList m_nListOtherSuffix;                         // 其他后缀

    ImageViewerTool *m_pImageViewerTool;                    // 图片打开Widget

    int m_nDataTotal;                                       // 文件总数
    int m_nCurrentPage;                                     // 当前页码
    QFutureWatcher<void> *m_pWatcher;                       // QFutureWatcher监视器
};

#endif // MATERIALMANAGEMENT_H
