#ifndef FILTERBARWIDGET_H
#define FILTERBARWIDGET_H

#include <QWidget>
class QLineEdit;
class QComboBox;
class QDateEdit;
class QPushButton;


/**
 * @brief The FilterBarWidget class
 * 素材筛选栏，根据条件筛选出需要的素材，并提供上传功能
 */
class FilterBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterBarWidget(QWidget *parent = nullptr);

signals:
    void search(const QString &name);
    void insert();
    void deleteData();
    void refresh();

private:
    void initializeUi();    // 初始化界面
    void handleEvents();    // 信号与槽处理

    QLineEdit *m_pLineEditSearch;               // 名称搜索框
    QComboBox *m_pComboBoxFileSort;             // 文件排序下拉框
    QPushButton *m_pBtnSearch;                  // 搜索按钮
    QPushButton *m_pBtnSearchCancel;            // 清除按钮
    QPushButton *m_pBtnUpload;                  // 上传素材按钮
    QPushButton *m_pBtnDelete;                  // 删除按钮
    QPushButton *m_pBtnRefresh;                 // 刷新按钮
};

#endif // FILTERBARWIDGET_H
