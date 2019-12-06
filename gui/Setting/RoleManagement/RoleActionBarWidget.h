#ifndef RoleActionBarWidget_H
#define RoleActionBarWidget_H

#include <QWidget>
class QAbstractItemModel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QDateEdit;
class QPushButton;
class RoleUniqueProxyModel;


class RoleActionBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoleActionBarWidget(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *sourceModel);     // 设置表格模型(combobox下拉选择)

signals:
    void search(const QString &name);
    void insert();
    void deleteData();
    void refresh();

private:
    void initializeUi();                        // 初始化界面
    void initializeData();                      // 初始化数据
    void handleEvents();                        // 信号与槽处理

    QLineEdit *m_pLineEditSearch;               // 名称搜索框
    QPushButton *m_pBtnSearch;                  // 查询按钮
    QPushButton *m_pBtnSearchCancel;            // 取消查询按钮

    QPushButton *m_pBtnInsert;                  // 插入按钮
    QPushButton *m_pBtnDelete;                  // 删除按钮
    QPushButton *m_pBtnRefresh;                 // 刷新按钮
};

#endif // RoleActionBarWidget_H
