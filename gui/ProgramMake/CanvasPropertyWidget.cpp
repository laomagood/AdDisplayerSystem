#include "CanvasPropertyWidget.h"
#include "ui_CanvasPropertyWidget.h"
#include "CanvasWidget.h"
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

CanvasPropertyWidget::CanvasPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasPropertyWidget)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();
}

CanvasPropertyWidget::~CanvasPropertyWidget()
{
    delete ui;
}

void CanvasPropertyWidget::setCanvasWidget(QWidget *widget)
{
    if (qobject_cast<CanvasWidget *>(widget)) {
        m_pCanvasWidget = qobject_cast<CanvasWidget *>(widget);
        getData();
    }
}

void CanvasPropertyWidget::initializeUi()
{
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(8);

    ui->lineEdit_name->setText(tr("画布"));
    ui->lineEdit_name->setEnabled(false);

    ui->btn_background->setFixedSize(20, 20);

    ui->frame->setFixedSize(20, 20);
    ui->frame->setContentsMargins(0, 0, 0, 0);
    QPalette pal = ui->frame->palette();
    pal.setColor(QPalette::Window, Qt::black);
    ui->frame->setPalette(pal);
    ui->frame->setAutoFillBackground(true);
    ui->btn_ColorSelect->setFixedSize(18, 18);

    ui->lineEdit_width->setEnabled(false);
    ui->lineEdit_height->setEnabled(false);

    // 分辨率限制输入
    QIntValidator *validator = new QIntValidator(100, 10000, this);
    ui->lineEdit_width->setValidator(validator);
    ui->lineEdit_height->setValidator(validator);

    // 虚构一些分辨率------------------------------------------------
    ui->comboBox_ScreenScale->addItem("800x600");
    ui->comboBox_ScreenScale->addItem("1024x768");
    ui->comboBox_ScreenScale->addItem("2048x1536");
    ui->comboBox_ScreenScale->addItem("1280x720");
    ui->comboBox_ScreenScale->addItem("1920x1080");
    ui->comboBox_ScreenScale->addItem("768x1024");
    ui->comboBox_ScreenScale->addItem("1080x1920");
    ui->comboBox_ScreenScale->addItem(tr("自定义"));

    ui->comboBox_fillStyle->addItem(tr("平铺"));
    ui->comboBox_fillStyle->addItem(tr("拉伸"));

    setBGBtnColor(Qt::white);
}

void CanvasPropertyWidget::handleEvents()
{
    // 颜色选择
    connect(ui->btn_ColorSelect, &QPushButton::clicked, [this] {
        QColor color = QColorDialog::getColor(Qt::white, this);
        ui->lineEdit_background->clear();
        setBGBtnColor(color);
        if (m_pCanvasWidget != nullptr) {
            m_pCanvasWidget->setBackgroundColor(color);
        }
    });

    // 背景图片
    connect(ui->btn_background, &QPushButton::clicked, [this] {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                        tr("Images (*.png *.xpm *.jpg)"));
        ui->lineEdit_background->clear();
        ui->lineEdit_background->setText(fileName);
        if (m_pCanvasWidget != nullptr) {
            QString text = ui->comboBox_fillStyle->currentText();
            if (text == tr("平铺")) {
                m_pCanvasWidget->setBackgroundImage(fileName, CanvasWidget::TILE);
            } else {
                m_pCanvasWidget->setBackgroundImage(fileName);
            }
        }
    });

    // 填充方式切换
    connect(ui->comboBox_fillStyle, QOverload<const QString &>::of(&QComboBox::activated),[this](const QString &text) {
        QString fileName = ui->lineEdit_background->text();
        if (!fileName.isEmpty()) {
            if (m_pCanvasWidget != nullptr) {
                if (text == tr("平铺")) {
                    m_pCanvasWidget->setBackgroundImage(fileName, CanvasWidget::TILE);
                } else {
                    m_pCanvasWidget->setBackgroundImage(fileName);
                }
            }
        }
    });

    connect(ui->lineEdit_background, &QLineEdit::textChanged, [this] {
        qDebug() << "背景图片变更";
    });

    // 分辨率切换
    connect(ui->comboBox_ScreenScale, QOverload<const QString &>::of(&QComboBox::activated),[this](const QString &text) {
        if (text == tr("自定义")) {
            int ret = QMessageBox::warning(this, tr("自定义分辨率"), tr("分辨率由系统自动从终端获取，如果自定义请确保与终端的分辨率保持一致，"
                                                                  "否则可能会导致无法正常显示，您是否还需自定义分辨率？"),
                                           QMessageBox::Yes|QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                ui->lineEdit_width->setEnabled(true);
                ui->lineEdit_height->setEnabled(true);
            } else if (ret == QMessageBox::No) {
                ui->lineEdit_width->setEnabled(false);
                ui->lineEdit_height->setEnabled(false);
                ui->comboBox_ScreenScale->setCurrentIndex(0);
            }
        } else {
            ui->lineEdit_width->setEnabled(false);
            ui->lineEdit_height->setEnabled(false);

            int nScreenW = text.section('x', 0, 0).toInt();
            int nScreenH = text.section('x', 1, 1).toInt();
            qDebug() << m_pCanvasWidget;
            if (m_pCanvasWidget != nullptr) {
                m_pCanvasWidget->setCanvasSize(nScreenW, nScreenH);
                ui->lineEdit_width->setText(QString::number(nScreenW));
                ui->lineEdit_height->setText(QString::number(nScreenH));
            }
        }
    });

    // 自定义分辨率
    connect(ui->lineEdit_height, &QLineEdit::textChanged, [this] {
        if (ui->comboBox_ScreenScale->currentText() != tr("自定义")) {
            return;
        }
        QString strW = ui->lineEdit_width->text();
        QString strH = ui->lineEdit_height->text();
        if (!strW.isEmpty() && !strH.isEmpty()) {
            qDebug() << "ok";
            int w = strW.toInt();
            int h = strH.toInt();
            if (w>0 && h>0) {
                if (m_pCanvasWidget != nullptr) {
                    m_pCanvasWidget->setCanvasSize(w, h);
                }
            }
        }
    });
    connect(ui->lineEdit_width, &QLineEdit::textChanged, [this] {
        if (ui->comboBox_ScreenScale->currentText() != tr("自定义")) {
            return;
        }
        QString strW = ui->lineEdit_width->text();
        QString strH = ui->lineEdit_height->text();
        if (!strW.isEmpty() && !strH.isEmpty()) {
            qDebug() << "ok";
            int w = strW.toInt();
            int h = strH.toInt();
            if (w>0 && h>0) {
                if (m_pCanvasWidget != nullptr) {
                    m_pCanvasWidget->setCanvasSize(w, h);
                }
            }
        }
    });
}

void CanvasPropertyWidget::setBGBtnColor(QColor color)
{
    QPalette pal = ui->btn_ColorSelect->palette();
    pal.setColor(QPalette::Button, color);
    ui->btn_ColorSelect->setPalette(pal);
    ui->btn_ColorSelect->setAutoFillBackground(true);
    ui->btn_ColorSelect->setFlat(true);
}

void CanvasPropertyWidget::getData()
{
    if (m_pCanvasWidget != nullptr) {
        // 背景颜色
        setBGBtnColor(m_pCanvasWidget->getBackgroundColor());
        // 背景图片
        ui->lineEdit_background->setText(m_pCanvasWidget->getBackgroundImage());
    }
}
