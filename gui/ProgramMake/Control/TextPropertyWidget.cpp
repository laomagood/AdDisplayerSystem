#include "TextPropertyWidget.h"
#include "ui_TextPropertyWidget.h"
#include "TextWidget.h"
#include <QDebug>
#include <QColorDialog>
#include <QFontDatabase>


TextPropertyWidget::TextPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextPropertyWidget),
    m_pTextWidget(nullptr)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();
}

TextPropertyWidget::~TextPropertyWidget()
{
    delete ui;
}

void TextPropertyWidget::setTextWidget(QWidget *widget)
{
    if (qobject_cast<TextWidget *>(widget)) {
        m_pTextWidget = qobject_cast<TextWidget *>(widget);
        getData();
    }
}

void TextPropertyWidget::initializeUi()
{
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(8);

    ui->frame_text->setFixedSize(20, 20);
    ui->frame_background->setFixedSize(20, 20);
    ui->frame_text->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frame_background->layout()->setContentsMargins(0, 0, 0, 0);

    ui->btn_textColor->setFixedSize(18, 18);
    ui->btn_BGColor->setFixedSize(18, 18);

    QPalette pal = ui->frame_text->palette();
    pal.setColor(QPalette::Window, Qt::black);
    ui->frame_text->setPalette(pal);
    ui->frame_text->setAutoFillBackground(true);
    ui->frame_background->setPalette(pal);
    ui->frame_background->setAutoFillBackground(true);

    ui->checkBox_transparent->setChecked(true);

    setTextBtnColor(Qt::black);
    setBGBtnColor(Qt::white);

    ui->textEdit->setFixedHeight(120);

    // 获取当前系统字体
    QFontDatabase fontDatabase;
    ui->comboBox_family->setMaximumWidth(117);
    ui->comboBox_family->addItems(fontDatabase.families());

    // 滚动方向
    ui->comboBox_dir->addItem(tr("从左到右"));
    ui->comboBox_dir->addItem(tr("从右到左"));

    // 滚动速度
    ui->slider_speed->setRange(1, 99);
    ui->slider_speed->setValue(50);
}

void TextPropertyWidget::handleEvents()
{
    // 控件名称
    connect(ui->lineEdit_name, &QLineEdit::textChanged, [this] {
        if (m_pTextWidget != nullptr) {
            m_pTextWidget->setControlName(ui->lineEdit_name->text());
        }
    });

    // 字体颜色
    connect(ui->btn_textColor, &QPushButton::clicked, [this] {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("字体颜色选择"), QColorDialog::ShowAlphaChannel);
        setTextBtnColor(color);
        m_pTextWidget->setTextColor(color);
    });

    // 字体大小
    connect(ui->spinBox_textSize, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        m_pTextWidget->setTextSize(i);
    });

    // 字体
    connect(ui->comboBox_family, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),[this](const QString &text){
        if (m_pTextWidget != nullptr) {
            m_pTextWidget->setTextFamily(text);
        }
    });

    // 背景颜色
    connect(ui->btn_BGColor, &QPushButton::clicked, [this] {
        QColor color = QColorDialog::getColor(Qt::white, this,  tr("背景颜色选择"), QColorDialog::ShowAlphaChannel);
        setBGBtnColor(color);
        m_pTextWidget->setBackgroundColor(color);
    });

    // 背景透明
    connect(ui->checkBox_transparent, &QCheckBox::stateChanged, [this](int state) {
        if (state == Qt::Checked) {
            m_pTextWidget->setBGTransparent(true);
        } else {
            m_pTextWidget->setBGTransparent(false);
        }
    });

    // 更改滚动
    connect(ui->checkBox_scroll, &QCheckBox::clicked, [this](bool checked) {
        if (m_pTextWidget != nullptr) {
            m_pTextWidget->setIsScrolled(checked);
            if (!checked) {
                ui->comboBox_dir->setEnabled(false);
                ui->slider_speed->setEnabled(false);
            } else {
                ui->comboBox_dir->setEnabled(true);
                ui->slider_speed->setEnabled(true);
            }
        }
    });

    // 滚动方向
    connect(ui->comboBox_dir, QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index){
        if (m_pTextWidget != nullptr) {
            m_pTextWidget->setScrollDirection(TextWidget::ScrollDirection(index));
        }
    });

    // 滚动速度
    connect(ui->slider_speed, &QSlider::valueChanged, [this](int value){
        if (m_pTextWidget != nullptr) {
            m_pTextWidget->setScrollSpeed(value);
        }
    });

    // 更改文本
    connect(ui->textEdit, &QTextEdit::textChanged, [this] {
        if (m_pTextWidget != nullptr) {
            qDebug() << ui->textEdit->toPlainText();
            m_pTextWidget->setText(ui->textEdit->toPlainText());
        }
    });
}

void TextPropertyWidget::setTextBtnColor(QColor color)
{
    QPalette pal = ui->btn_textColor->palette();
    pal.setColor(QPalette::Button, color);
    ui->btn_textColor->setPalette(pal);
    ui->btn_textColor->setAutoFillBackground(true);
    ui->btn_textColor->setFlat(true);
}

void TextPropertyWidget::setBGBtnColor(QColor color)
{
    QPalette pal = ui->btn_BGColor->palette();
    pal.setColor(QPalette::Button, color);
    ui->btn_BGColor->setPalette(pal);
    ui->btn_BGColor->setAutoFillBackground(true);
    ui->btn_BGColor->setFlat(true);
}

void TextPropertyWidget::getData()
{
    if (m_pTextWidget != nullptr) {
        // 控件名
        ui->lineEdit_name->setText(m_pTextWidget->getControlName());

        // 是否滚动
        bool bScroll = m_pTextWidget->getIsScrolled();
        ui->checkBox_scroll->setChecked(bScroll);
        if (!bScroll) {
            ui->comboBox_dir->setEnabled(false);
            ui->slider_speed->setEnabled(false);
        } else {
            ui->comboBox_dir->setEnabled(true);
            ui->slider_speed->setEnabled(true);
        }

        // 滚动方向
        ui->comboBox_dir->setCurrentIndex((int)m_pTextWidget->getScrollDirection());

        // 滚动速度
        ui->slider_speed->setValue(m_pTextWidget->getScrollSpeed());

        // 字体
        ui->comboBox_family->setCurrentIndex(ui->comboBox_family->findText(m_pTextWidget->getTextFamily()));

        // 字体颜色
        setTextBtnColor(m_pTextWidget->getTextColor());

        // 字体大小
        ui->spinBox_textSize->setValue(m_pTextWidget->getTextSize());

        // 文本内容
        ui->textEdit->setText(m_pTextWidget->getText());

        // 背景颜色
        setBGBtnColor(m_pTextWidget->getBackgroundColor());

        // 背景透明
        ui->checkBox_transparent->setChecked(m_pTextWidget->getBGTransparent());
    }
}
