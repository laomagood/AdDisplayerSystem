#ifndef TEXTBASE_H
#define TEXTBASE_H

#include <QWidget>
#include <QDebug>
#include "ControlWidget.h"

class TextBase : public ControlWidget
{
    Q_OBJECT
public:
    explicit TextBase(QWidget *parent = nullptr);
    virtual ~TextBase() {}
    void setTextColor(const QColor &color);                 // 设置文字颜色
    void setTextSize(int size);                             // 设置文字大小
    void setTextSizeScale(float scale);                     // 设置文字大小比例
    void setTextFamily(const QString &family);              // 设置文字字体
    void setBackgroundColor(const QColor &color);           // 设置背景颜色
    void setBGTransparent(bool isTransparent);              // 设置背景透明

    QColor getTextColor() const { return m_textColor; }
    int getTextSize() const { return m_font.pointSize(); }
    float getTextSizeScale() const { return m_nTextSizeScale; }
    QString getTextFamily() const { return m_font.family(); }
    QColor getBackgroundColor() const { return m_backgroundColor; }
    bool getBGTransparent() const { return m_bBGTransparent; }

protected:
    virtual void initializeData() = 0;                      // 初始化数据 纯需函数

protected:
    QColor m_textColor;                                     // 文字颜色
    QColor m_backgroundColor;                               // 背景颜色
    bool m_bBGTransparent;                                  // 是否背景透明
    QFont m_font;                                           // 字体
    float m_nTextSizeScale;                                 // 文字大小比例
};

#endif // TEXTBASE_H
