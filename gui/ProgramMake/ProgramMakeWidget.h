#ifndef PROGRAMMAKEWIDGET_H
#define PROGRAMMAKEWIDGET_H

#include <QWidget>
class ProgramToolbarWidget;
class PropertyWidget;
class ProgramEditAreaWidget;
class ControlListWidget;

/**
 * @brief The ProgramMakeWidget class
 * 节目制作的主界面
 */

class ProgramMakeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramMakeWidget(QWidget *parent = 0);
    ~ProgramMakeWidget();
    bool toBean(QByteArray byteArray);

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    ProgramToolbarWidget *m_pProgramToolbarWidget;          // 工具栏
    PropertyWidget *m_pPropertyWidget;                      // 属性栏
    ProgramEditAreaWidget *m_pProgramEditAreaWidget;        // 编辑区
    ControlListWidget *m_pControlListWidget;                // 图层栏
};

#endif // PROGRAMMAKEWIDGET_H
