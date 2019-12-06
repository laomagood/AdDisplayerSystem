#ifndef FILELAYOUTAREAWIDGET_H
#define FILELAYOUTAREAWIDGET_H

#include <QFrame>
#include <QFutureWatcher>
#include "bean/ApprovalBean.h"
#include "MaterialActionBarWidget.h"
class ThumbnailWidget;

/**
 * @brief The FileLayoutAreaWidget class
 * 文件布局区用来显示文件，带QScrollArea
 */

class FileLayoutAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileLayoutAreaWidget(QWidget *parent = nullptr);
    ~FileLayoutAreaWidget();
    void setApprovalBeanList(const QList<ApprovalBean> &approvalBeanList);  // 设置QList<ApprovalBean>
    void setFilesList(const QStringList &fileNames);                        // 设置文件列表
    inline int getPageDataAmount() const { return m_nPageDataAmount; }      // 获取每页文件数
    void setInformationLabel(MaterialActionBarWidget::ArrangeType arrangeType);     // 设置图标信息
    void setApproval(bool isApproval);                                      // 设置审核状态
    void select(bool bIsChecked);                                           // 全选反选
    bool isCheckedThumbnails();                                             // 是否有勾选
    int checkedThumbnailsSize();                                            // 勾选数量
    QStringList deleteThumbnails();                                         // 删除缩略图,返回删除失败名称

signals:
    void openThumbnail(ThumbnailWidget *pThumbnailWidget);                  // 发送信号:打开文件
    void ThumbnailDeleted();                                                // 发送信号:缩略图文件被删除

private slots:
    void showImage(int index);                                              // 多线程加载缩略图

private:
    void initializeUi();                                                    // 初始化界面
    void handleEvents();                                                    // 信号与槽处理
    bool deleteThumbnail(ThumbnailWidget *pThumbnailWidget);                                 // 删除缩略图

    QWidget *m_pCentralArea;                                                // 放缩略图的Widget
    QList<ThumbnailWidget *> m_thumbnailList;                               // 缩略图List
    int m_nPageDataAmount;                                                  // 每页文件数
    QFutureWatcher<QImage> *m_pWatcher;
    QList<ThumbnailWidget *> m_thumbnailImageList;                          // 缩略图List(图片类型)
};

#endif // FILELAYOUTAREAWIDGET_H
