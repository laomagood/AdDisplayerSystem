#ifndef THUMBNAILWIDGET_H
#define THUMBNAILWIDGET_H

#include <QWidget>
#include <global.h>
#include "bean/MaterialBean.h"
#include "bean/ApprovalBean.h"
#include "MaterialActionBarWidget.h"
class QLabel;
class QCheckBox;

/**
 * @brief The ThumbnailWidget class
 * 缩略图widget提供素材信息180*220
 * 右键：打开，重命名，删除
 * 审核模式：未审核，通过审核，不通过审核
 * 双击播放or查看大图
 */
class ThumbnailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThumbnailWidget(QWidget *parent = nullptr);
    void setApprovalBean(const ApprovalBean &materialBean);     // 设置ApprovalBean
    void setChecked(bool isChecked);                            // 设置是否选中
    bool isChecked() const;                                     // 返回是否被选中
    void setPixmap(QPixmap const &pixmap);                      // 设置显示图片
    void setInformationLabel(MaterialActionBarWidget::ArrangeType arrangeType);          // 设置信息标签(根据需要显示文件大小，上传时间，频率)
    void setApproval(bool bIsAudit);                            // 设置是否进入审核
    void upDateApprovalSQL();                                   // 更新approval数据库(类型，上传时间)

    inline QString getFilePath() const { return m_strFilePath; }            // 获取文件路径
    inline QString getFileName() const { return m_strFileName; }            // 获取文件名
    inline ApprovalBean getApprovalBean() const { return m_approvalBean; }  // 获取ApprovalBean

signals:
    void openThumbnail(ThumbnailWidget *pThumbnailWidget);      // 发送信号:打开文件
    void deleteThumbnail(ThumbnailWidget *pThumbnailWidget);    // 发送信号:删除文件

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeUi();                                        // 初始化界面
    void handleEvents();                                        // 信号与槽处理
    QString size(qint64 bytes);                                 // 文件大小转换
    QPixmap readProgramThumbnail();                             // 读取节目缩略图
    void setFileType(const QString &filePath);                  // 设置文件类型
    void setFilePath();                                         // 设置文件路径(实际路径)
    void setTypeIcon();                                         // 设置类型角标
    void changeApprovalStatus();                                // 更改审核状态
    void setApprovalStatusLabel();                              // 设置审核图标

    ApprovalBean m_approvalBean;                                // ApprovalBean
    QString m_strFilePath;                                      // 文件路径(实际路径)
    QString m_strFileName;                                      // 文件路径(实际路径)
    QLabel *m_pNameLabel;                                        // 文件名
    QLabel *m_pInformationLabel;                                // 文件信息：大小，时间，频率
    QLabel *p_thumbnailLabel;                                   // 缩略图
    QLabel *p_fileTypeIcon;                                     // 类型角标
    QCheckBox *m_pCheckBox;                                     // 选择框

    QLabel *m_pApprovalStatusLabel;                             // 审核图标
    bool m_bIsApproval;                                         // 是否进入审核状态
};

#endif // THUMBNAILWIDGET_H
