#ifndef PROGRAMEDITAREAWIDGET_H
#define PROGRAMEDITAREAWIDGET_H

#include <QTabWidget>
class CanvasWidget;
/**
 * @brief The ProgramEditAreaWidget class
 * 界面编辑区使用tabWidget多文档同时操作
 * 可能有内存泄漏！！
 */

class ProgramEditAreaWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ProgramEditAreaWidget(QWidget *parent = nullptr);
    CanvasWidget* getCurrentCanvas() const;             // 获取当前Tab的Canvas
    void newCanvas();                                   // 新建
    void openCanvas();                                  // 打开

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
    void focusWidget(QWidget *widget);
    void updateControlList(CanvasWidget *);             // 更新控件列表
private:
    void initializeUi();                                // 初始化界面
    void handleEvents();                                // 信号与槽处理

    int m_newFileNumber;                                // 新建文件序号
    QList<CanvasWidget *> m_listCanvasWidget;           // 画布对象列表
    QHash<QWidget *,CanvasWidget *> m_widgeCanvastHash; // key:Widget指针，value:CanvasWidget指针
    QList<QWidget *> m_listWidget;                      // 画布父对象列表(用来)
};

#endif // PROGRAMEDITAREAWIDGET_H
