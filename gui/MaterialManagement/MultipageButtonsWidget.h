#ifndef MULTIPAGEBUTTONSWIDGET_H
#define MULTIPAGEBUTTONSWIDGET_H

#include <QWidget>
class QButtonGroup;
class QPushButton;
class QLabel;
class QLineEdit;

/**
 * @brief The MultipageButtonsWidget class
 * 分页按钮 用于需要翻页时的情况
 */
class MultipageButtonsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultipageButtonsWidget(QWidget *parent = nullptr);
    /**
     * @brief setData 设置分页数据
     * @param pageDataAmount 每页的数据条数
     * @param dataTotal 总数据条数
     */
    void setData(int pageDataAmount, int dataTotal);
    inline int getPageDataAmount() const { return m_nPageDataAmount; }
    inline int getDataTotal() const { return m_nDataTotal; }

signals:
    void currentPage(int page);                 // 当前页码

private:
    void initializeUi();                        // 初始化界面
    void initializeData();                      // 初始化数据
    void handleEvents();                        // 信号与槽处理

    QButtonGroup *m_pGroupButtons;              // 分类按钮组

    QPushButton *m_pBtnPgUp;                    // 上一页
    QPushButton *m_pBtnPgDn;                    // 下一页
    QPushButton *m_pBtnFirst;                   // 第一页
    QPushButton *m_pBtnSecond;                  // 第二页
    QPushButton *m_pBtnThird;                   // 第三页
    QPushButton *m_pBtnHomePg;                  // 首页
    QPushButton *m_pBtnEndPg;                   // 尾页

    QPushButton *m_pBtnJump;                    // 跳转
    QLabel      *m_pDataInformation;            // 数据信息
    QLineEdit   *m_pPageLineEdit;               // 页码输入框

    int m_nPageTotal;                           // 页面数
    int m_nCurrentPage;                         // 当前页
    int m_nTargetpage;                          // 目标页(用来切换页码)
    int m_nPageDataAmount;                      // 每页多少条
    int m_nDataTotal;                           // 总共多少条数据
};

#endif // MULTIPAGEBUTTONSWIDGET_H
