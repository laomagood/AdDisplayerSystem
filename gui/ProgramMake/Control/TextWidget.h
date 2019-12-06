#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QWidget>
#include "TextBase.h"
class QLabel;
class QPropertyAnimation;

/**
 * @brief The TextWidget class
 * 文字
 */

class TextWidget : public TextBase
{
    Q_OBJECT
public:
    enum ScrollDirection{
        LeftToRight,
        RightToLeft
    };
    enum TextAlignment{
        AlignLeft,
        AlignRight,
        AlignCenter
    };

    explicit TextWidget(QWidget *parent = nullptr);

    void setIsScrolled(bool isScoll);                           // 设置是否滚动
    void setScrollDirection(ScrollDirection dir);               // 设置滚动方向
    void setScrollSpeed(int speed);                             // 设置滚动速度
    void setText(const QString &text);                          // 设置文字

    bool getIsScrolled() const { return m_isScrolled; }
    ScrollDirection getScrollDirection() const { return m_scrollDir; }
    int getScrollSpeed() const { return m_scrollSpeed; }
    QString getText() const { return m_text; }

    QJsonDocument toJson();                                     // 转为Json
    bool toBean(const QJsonDocument &doc);                      // 转为Bean

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void initializeData() Q_DECL_OVERRIDE;                      // 初始化数据

private:
    void initializeUi();                                        // 初始化数据
    void handleEvents();                                        // 信号与槽处理
    void textAnimation();                                       // 文字动画

    TextAlignment m_textAlignment;                              // 文字对齐方式
    bool m_isScrolled;                                          // 是否滚动
    int m_scrollSpeed;                                          // 滚动速度(1-100)
    ScrollDirection m_scrollDir;                                // 滚动方向
    QString m_text;                                             // 文本内容

    QLabel *p_label;                                            // Label用于显示文字
    int m_textPixelWidth;                                       // 文字像素总长

    QPropertyAnimation *m_positionAnimation;
};

#endif // TEXTWIDGET_H
