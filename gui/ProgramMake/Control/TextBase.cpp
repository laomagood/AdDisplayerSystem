#include "TextBase.h"

TextBase::TextBase(QWidget *parent) : ControlWidget(parent),
  m_textColor(QColor(255, 0, 0)),
  m_backgroundColor(QColor(200, 200, 200)),
  m_bBGTransparent(false)
{
    this->setControlBorderBackgroundVisible(false);
}

void TextBase::setTextColor(const QColor &color)
{
    m_textColor = color;
    initializeData();
}

void TextBase::setTextSizeScale(float scale)
{
    m_nTextSizeScale = scale;
    int size = this->height()*scale;
    m_font.setPointSize(size);
    initializeData();
}

void TextBase::setTextSize(int size)
{
    m_font.setPointSize(size);
    initializeData();
}

void TextBase::setTextFamily(const QString &family)
{
    m_font.setFamily(family);
    initializeData();
}

void TextBase::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    initializeData();
}

void TextBase::setBGTransparent(bool isTransparent)
{
    m_bBGTransparent = isTransparent;
    initializeData();
}
