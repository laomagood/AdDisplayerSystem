#ifndef UserActionBarWidget_H
#define UserActionBarWidget_H

#include <QWidget>
class QAbstractItemModel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QDateEdit;
class QPushButton;
class RoleUniqueProxyModel;


class UserActionBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserActionBarWidget(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *sourceModel);     // 设置表格模型(combobox下拉选择)
    QString getComboBoxText() const;
    bool isCheckBoxClicked() const;

signals:
    void search(const QString &name);
    void insert();
    void deleteData();
    void refresh();
    void roleCheckClicked(bool checked);
    void roleComboBoxTextChanged(const QString &text);

private:
    void initializeUi();                        // 初始化界面
    void initializeData();                      // 初始化数据
    void handleEvents();                        // 信号与槽处理

    void createComboBoxModel(QComboBox *comboBox, int column);

    QLineEdit *m_pLineEditSearch;               // 名称搜索框
    QPushButton *m_pBtnSearch;                  // 查询按钮
    QPushButton *m_pBtnSearchCancel;            // 取消查询按钮
    QComboBox *m_pComboBoxRole;                 // 下拉框角色选择
    QCheckBox *m_pCheckBoxRole;                 // 角色过滤启用

    QPushButton *m_pBtnInsert;                  // 插入按钮
    QPushButton *m_pBtnDelete;                  // 删除按钮
    QPushButton *m_pBtnRefresh;                 // 刷新按钮

    RoleUniqueProxyModel *m_pRoleUniqueProxyModel;   // 角色筛选的代理模型
};

#endif // UserActionBarWidget_H
