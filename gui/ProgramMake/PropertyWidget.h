#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
class QStackedWidget;
class CanvasWidget;
class CanvasPropertyWidget;
class TextPropertyWidget;
class ImagePropertyWidget;
class MVPropertyWidget;

/**
 * @brief The PropertyWidget class
 * 各种控件属性的窗口
 */
class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyWidget(QWidget *parent = nullptr);
    void setCurrentWidget(QWidget *widget);

private:
    void initializeUi();        // 初始化界面
    void handleEvents();        // 信号与槽处理

    QStackedWidget *m_pStackedWidget;
    CanvasPropertyWidget *m_pCanvasPropertyWidget;
    TextPropertyWidget *m_pTextPropertyWidget;
    ImagePropertyWidget *m_pImagePropertyWidget;
    MVPropertyWidget *m_pMVPropertyWidget;
};

#endif // PROPERTYWIDGET_H
