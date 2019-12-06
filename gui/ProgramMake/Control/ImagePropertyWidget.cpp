#include "ImagePropertyWidget.h"
#include "ui_ImagePropertyWidget.h"
#include "ImageWidget.h"
#include "ImageListItemWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>

ImagePropertyWidget::ImagePropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePropertyWidget)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();
}

ImagePropertyWidget::~ImagePropertyWidget()
{
    delete ui;
}

void ImagePropertyWidget::setImageWidget(QWidget *widget)
{
    if (qobject_cast<ImageWidget *>(widget)) {
        m_pImageWidget = qobject_cast<ImageWidget *>(widget);
        getData();
    }
}

void ImagePropertyWidget::initializeUi()
{
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(8);

    //    ui->lineEdit_name->setEnabled(false);
    //    ui->lineEdit_name->setText(tr("图片"));

    ui->comboBox->addItem(tr("从左到右"));
    ui->comboBox->addItem(tr("从右到左"));
    ui->comboBox->addItem(tr("透明度"));

    ui->spinBox_time->setMinimum(1);
    ui->spinBox_time->setMaximum(60);
}

void ImagePropertyWidget::handleEvents()
{
    // 控件名称
    connect(ui->lineEdit_name, &QLineEdit::textChanged, [this] {
        if (m_pImageWidget != nullptr) {
            m_pImageWidget->setControlName(ui->lineEdit_name->text());
        }
    });

    // 增加图片
    connect(ui->btn_add, &QPushButton::clicked, [this] {
        QStringList files = QFileDialog::getOpenFileNames( this, tr("选择需要添加的图片"),
                                                           "/home", "Images (*.png *.xpm *.jpg  *.jpeg)");

        if (m_pImageWidget != nullptr) {
            m_pImageWidget->addList(files);
            getData();
        }
    });

    // 移除图片
    connect(ui->btn_remove, &QPushButton::clicked, [this] {
        if (ui->listWidget->currentRow() == -1) {
            QMessageBox::warning(this, tr("移除图片警告"),
                                 tr("没有选中要移除的图片行,\n"
                                    "请选中要移除的图片行后按移除按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pImageWidget->remove(ui->listWidget->currentRow());
            getData();
        }
    });

    // 清空图片
    connect(ui->btn_clean, &QPushButton::clicked, [this] {
        int ret = QMessageBox::warning(this, tr("清空警告"),
                                       tr("您是否确实要清空图片列表?"),
                                       QMessageBox::Yes | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes) {
            m_pImageWidget->clearList();
            getData();
        }
    });

    // 上移图片
    connect(ui->btn_upper, &QPushButton::clicked, [this] {
        int nCurrentRow = ui->listWidget->currentRow();
        if (nCurrentRow == -1) {
            QMessageBox::warning(this, tr("移动图片警告"),
                                 tr("没有选中要移动的图片行,\n"
                                    "请选中要移动的图片行后按移动按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pImageWidget->moveLayerUP(nCurrentRow);
            getData();
            if (nCurrentRow > 0) {
                ui->listWidget->setCurrentRow(nCurrentRow-1);
            } else if(nCurrentRow == 0) {
                ui->listWidget->setCurrentRow(0);
            }
        }
    });

    // 下移图片
    connect(ui->btn_lower, &QPushButton::clicked, [this] {
        int nCurrentRow = ui->listWidget->currentRow();
        if (nCurrentRow == -1) {
            QMessageBox::warning(this, tr("移动图片警告"),
                                 tr("没有选中要移动的图片行,\n"
                                    "请选中要移动的图片行后按移动按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pImageWidget->moveLayerDown(nCurrentRow);
            getData();
            if (nCurrentRow < (ui->listWidget->count()-1)) {
                ui->listWidget->setCurrentRow(nCurrentRow+1);
            } else if(nCurrentRow == (ui->listWidget->count()-1)) {
                ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            }
        }
    });

    // 切换模式
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated),[this](int index) {
        if (m_pImageWidget != nullptr) {
            m_pImageWidget->setImageChangeType((ImageWidget::ImageChangeType)index);
        }
    });

    // 切换时间
    connect(ui->spinBox_time, QOverload<int>::of(&QSpinBox::valueChanged),[this](int i) {
        if (m_pImageWidget != nullptr) {
            qDebug() << "changeTime:" << i;
            m_pImageWidget->setImageChangeTime(i);
        }
    });
}

void ImagePropertyWidget::getData()
{
    if (m_pImageWidget != nullptr) {
        ui->lineEdit_name->setText(m_pImageWidget->getControlName());
        ui->spinBox_time->setValue(m_pImageWidget->getImageChangeTime());
        ui->comboBox->setCurrentIndex(m_pImageWidget->getImageChangeType());

        // 全部删除图片列表
        while (ui->listWidget->count() > 0) {
            ui->listWidget->removeItemWidget(ui->listWidget->item(0));
            ui->listWidget->takeItem(0);
        }

        // 这里可能有内存泄漏
        QStringList list = m_pImageWidget->getImageList();

        for(int i=0; i<list.size(); i++) {
            auto *pImageListItemWidget = new ImageListItemWidget;
            pImageListItemWidget->setImageFilePath(list.at(i));

            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            ui->listWidget->addItem(pItem);
            ui->listWidget->setItemWidget(pItem, pImageListItemWidget);
            pItem->setSizeHint(QSize(0, 40));
        }
    }
}
