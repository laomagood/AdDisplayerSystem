#ifndef ProgramManagement_H
#define ProgramManagement_H

#include <QWidget>
#include <QFutureWatcher>
#include "global.h"
#include "bean/ApprovalBean.h"
#include "../MaterialManagement/MaterialActionBarWidget.h"

class MaterialSideBarWidget;
//class MaterialActionBarWidget;
class FileLayoutAreaWidget;
class MultipageButtonsWidget;
class ImageViewerTool;

class ProgramManagement : public QWidget
{
    Q_OBJECT
public:
    explicit ProgramManagement(QWidget *parent = nullptr);
    ~ProgramManagement();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理
    void setSuffixList();                                   // 设置后缀列表

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

    ImageViewerTool *m_pImageViewerTool;                    // 图片打开Widget

    int m_nDataTotal;                                       // 文件总数
    int m_nCurrentPage;                                     // 当前页码
    QFutureWatcher<void> *m_pWatcher;                       // QFutureWatcher监视器
};

#endif // ProgramManagement_H
