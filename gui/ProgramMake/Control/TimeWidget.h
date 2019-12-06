#ifndef TimeWidget_H
#define TimeWidget_H

#include <QWidget>
#include "TextBase.h"
class QLabel;
class QTimer;

/**
 * @brief The TimeWidget class
 * 时间
 */

class TimeWidget : public TextBase
{
    Q_OBJECT
public:
    explicit TimeWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void initializeData() Q_DECL_OVERRIDE;          // 初始化数据

private:
    void initializeUi();                            // 初始化数据
    void handleEvents();                            // 信号与槽处理
    void setTextHeight();                           // 设置字高
    void timerUpdate();                             // 定时器更新

    QLabel *m_pLabelMonth;                          // 年月日
    QLabel *m_pLabelWeek;                           // 星期
    QLabel *m_pLabelHour;                           // 小时
    QLabel *m_pLabelSecond;                         // 秒
    QTimer  *m_pTimer;                              // 定时器
};

#endif // TimeWidget_H
