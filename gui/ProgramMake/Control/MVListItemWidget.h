#ifndef MVLISTITEMWIDGET_H
#define MVLISTITEMWIDGET_H

#include <QWidget>
class QLabel;

class MVListItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MVListItemWidget(QWidget *parent = nullptr);

    void setFilePath(QString filePath);     // 文件路径

    void initializeUi();                    // 初始化界面
    void initializeData();                  // 初始化数据
    void handleEvents();                    // 信号与槽处理

private:
    QLabel *m_pMVName;                      // 名称
    QLabel *m_pMVDuration;                  // 时长
    QString m_nFilePath;                    // 路径
};

#endif // MVLISTITEMWIDGET_H
