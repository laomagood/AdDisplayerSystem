#include "TextWidget.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QJsonObject>
#include <QJsonArray>
#include "util/Json.h"

TextWidget::TextWidget(QWidget *parent) :
    TextBase(parent),
    m_textAlignment(AlignCenter),
    m_isScrolled(true),
    m_scrollSpeed(50),
    m_scrollDir(TextWidget::LeftToRight)
{
    m_text = tr("欢迎使用本多媒体系统");
    m_font = QFont("微软雅黑", 15);

    this->setControlName(tr("文字"));
    this->setControlIconPath("./image/icon/programMakeWidget/text.png");
    this->setControlType("TextWidget");

    this->resize(350, 50);
    initializeUi();
    initializeData();
    handleEvents();
}

void TextWidget::setIsScrolled(bool isScoll)
{
    m_isScrolled = isScoll;
    initializeData();
}

void TextWidget::setScrollDirection(TextWidget::ScrollDirection dir)
{
    m_scrollDir = dir;
    textAnimation();
}

void TextWidget::setScrollSpeed(int speed)
{
    if (speed > 1 && speed < 99) {
        m_scrollSpeed = speed;
        textAnimation();
    }
}

void TextWidget::setText(const QString &text)
{
    m_text = text;
    initializeData();
}

QJsonDocument TextWidget::toJson()
{
    // 字体颜色
    QJsonObject textColorObject;
    textColorObject.insert("R", m_textColor.red());
    textColorObject.insert("G", m_textColor.green());
    textColorObject.insert("B", m_textColor.blue());

    // 背景颜色
    QJsonObject bgColorObject;
    bgColorObject.insert("R", m_backgroundColor.red());
    bgColorObject.insert("G", m_backgroundColor.green());
    bgColorObject.insert("B", m_backgroundColor.blue());

    QJsonObject json;
    json.insert("Name", getControlName());
    json.insert("TextColor", textColorObject);
    json.insert("TextSizeScale", getTextSizeScale());
    json.insert("TextFamily", getTextFamily());
    json.insert("Text", m_text);

    json.insert("BackgroundColor", bgColorObject);
    json.insert("IsBGTransparent", m_bBGTransparent);
    json.insert("IsScrolled", m_isScrolled);
    json.insert("ScrollDirection", (int)m_scrollDir);
    json.insert("ScrollSpeed", m_scrollSpeed);

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    return document;
}

bool TextWidget::toBean(const QJsonDocument &doc)
{
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    Json nJson(strJson, false);

    this->setControlName(nJson.getString("Name"));
    this->m_textColor.setRed(nJson.getInt("TextColor.R"));
    this->m_textColor.setGreen(nJson.getInt("TextColor.G"));
    this->m_textColor.setBlue(nJson.getInt("TextColor.B"));

    this->setTextSizeScale(nJson.getInt("TextSizeScale"));
    this->setTextFamily(nJson.getString("TextFamily"));
    this->m_text = nJson.getString("Text");

    this->m_backgroundColor.setRed(nJson.getInt("BackgroundColor.R"));
    this->m_backgroundColor.setGreen(nJson.getInt("BackgroundColor.G"));
    this->m_backgroundColor.setBlue(nJson.getInt("BackgroundColor.B"));
    this->m_bBGTransparent = nJson.getBool("IsBGTransparent");
    this->m_isScrolled = nJson.getBool("IsScrolled");
    this->m_scrollDir = (ScrollDirection)nJson.getInt("ScrollDirection");
    this->m_scrollSpeed = nJson.getInt("ScrollSpeed");

    initializeData();
    return true;
}

void TextWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    initializeData();
    this->resizeControlBorder();
}

void TextWidget::initializeUi()
{
    p_label = new QLabel(this);
    m_positionAnimation = new QPropertyAnimation(p_label, "pos");
}

void TextWidget::initializeData()
{
    // 背景颜色
    QPalette palette(this->palette());
    if (m_bBGTransparent) {
        palette.setColor(QPalette::Background, QColor(0, 0, 0, 0));
    } else {
        palette.setColor(QPalette::Background, m_backgroundColor);
    }
    this->setPalette(palette);
    setAutoFillBackground(true);

    p_label->setAttribute(Qt::WA_TransparentForMouseEvents);
    p_label->setText(m_text);
    p_label->adjustSize();
    p_label->setFont(m_font);

    // 字体颜色
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, m_textColor);
    p_label->setPalette(labelPalette);

    // 字符串所占的像素宽度
    QFontMetrics fm(m_font);
    m_textPixelWidth = fm.boundingRect(m_text).width();
    int textPixeHeight = fm.boundingRect(m_text).height();
    int y = (height() - textPixeHeight)/2;

    // 滚动判断
    if (!m_isScrolled) {
        m_positionAnimation->stop();
        // 字体对齐
        int x = 0;
        switch (m_textAlignment) {
        case AlignCenter:
            x = (width() - m_textPixelWidth)/2;
            p_label->move(x, y);
            break;
        case AlignLeft:
            p_label->move(0, y);
            break;
        case AlignRight:
            x = width() - m_textPixelWidth;
            p_label->move(x, y);
            break;
        default:
            break;
        }
    } else {
        p_label->move(0, y);
        textAnimation();
        m_positionAnimation->start();
    }
}

void TextWidget::handleEvents()
{
    // 动画结束
    connect(m_positionAnimation, &QPropertyAnimation::finished, [this] {
        initializeData();
    });
}

void TextWidget::textAnimation()
{
    // 滚动速度
    int nSpeed = 1000 * (100 - m_scrollSpeed);
    m_positionAnimation->setDuration(nSpeed);

    QFontMetrics fm(m_font);
    int textPixeHeight = fm.boundingRect(m_text).height();
    int y = (height() - textPixeHeight)/2;

    if (m_scrollDir == RightToLeft) {
        m_positionAnimation->setStartValue(QPoint(width(), y));
        m_positionAnimation->setEndValue(QPoint(-m_textPixelWidth, y));
    } else if (m_scrollDir == LeftToRight) {
        m_positionAnimation->setStartValue(QPoint(-m_textPixelWidth, y));
        m_positionAnimation->setEndValue(QPoint(width(), y));
    }
}
