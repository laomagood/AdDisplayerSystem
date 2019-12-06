#ifndef MaterialActionBarWidget_H
#define MaterialActionBarWidget_H

#include <QWidget>
class QLineEdit;
class QLabel;
class QComboBox;
class QDateEdit;
class QPushButton;


/**
 * @brief The MaterialActionBarWidget class
 * 素材筛选栏，根据条件筛选出需要的素材，并提供上传功能
 */
class MaterialActionBarWidget : public QWidget
{
    Q_OBJECT
public:
    // 审核类型
    enum ApprovalType { ApprovalAll = -1, ApprovalNot, ApprovalFail, ApprovalPass};
    // 排列类型
    enum ArrangeType { ArrangeTime, ArrangeFrequency, ArrangeName, ArrangeSize };

    explicit MaterialActionBarWidget(QWidget *parent = nullptr);
    ArrangeType getArrangeType();
    ApprovalType getApprovalType();
    bool isCheckedBtnApproval();
    void setApprovalType(ApprovalType approvalType);

signals:
    void search(const QString &name);
    void upload();
    void deleteData();
    void refresh();
    void approvalChange(ApprovalType approvalType);
    void arrangeChange(ArrangeType arrangeType);
    void approvalToggled(bool checked);
    void select(bool checked);

private:
    void initializeUi();                        // 初始化界面
    void handleEvents();                        // 信号与槽处理

    QLineEdit *m_pLineEditSearch;               // 名称搜索框
    QLabel *m_pLabelApproval;                   // 标签审核
    QComboBox *m_pComboBoxApproval;             // 下拉框审核状态
    QComboBox *m_pComboBoxFileSort;             // 下拉框排序
    QPushButton *m_pBtnSearch;                  // 搜索按钮
    QPushButton *m_pBtnSearchCancel;            // 清除按钮
    QPushButton *m_pBtnApproval;                // 审核按钮
    QPushButton *m_pBtnUpload;                  // 上传素材按钮
    QPushButton *m_pBtnSelectAll;               // 全选按钮
    QPushButton *m_pBtnDelete;                  // 删除按钮
    QPushButton *m_pBtnRefresh;                 // 刷新按钮
};

#endif // MaterialActionBarWidget_H
