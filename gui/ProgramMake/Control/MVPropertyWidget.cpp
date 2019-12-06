#include "MVPropertyWidget.h"
#include "ui_MVPropertyWidget.h"
#include "MVWidgetBase.h"
#include "MusicWidget.h"
#include "VideoWidget.h"
#include "MVListItemWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>

MVPropertyWidget::MVPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MVPropertyWidget)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();
}

MVPropertyWidget::~MVPropertyWidget()
{
    delete ui;
}

void MVPropertyWidget::setWidget(QWidget *widget)
{
    if (qobject_cast<MVWidgetBase *>(widget)) {
        m_pWidget = qobject_cast<MVWidgetBase *>(widget);
        getData();
    }
}

void MVPropertyWidget::initializeUi()
{
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(8);

    ui->volumeSpinBox->setRange(0, 100);
    ui->volumeSpinBox->setSingleStep(5);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSpinBox->setValue(50);
    ui->volumeSlider->setValue(50);
}

void MVPropertyWidget::handleEvents()
{
    // 控件名称
    connect(ui->lineEdit_name, &QLineEdit::textChanged, [this] {
        if (m_pWidget != nullptr) {
            m_pWidget->setControlName(ui->lineEdit_name->text());
        }
    });

    // 音量调节
    connect(ui->volumeSlider, &QSlider::valueChanged, [this](int value) {
        ui->volumeSpinBox->setValue(value);
        if (m_pWidget != nullptr) {
            m_pWidget->setVolume(value);
        }
    });

    connect(ui->volumeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->volumeSlider->setValue(value);
        if (m_pWidget != nullptr) {
            m_pWidget->setVolume(value);
        }
    });

    // 增加
    connect(ui->btn_add, &QPushButton::clicked, [this] {
        if (m_pWidget != nullptr) {
            QStringList files;
            if (qobject_cast<MusicWidget *>(m_pWidget)) {
                files = QFileDialog::getOpenFileNames( this, tr("选择需要添加的内容"),
                                                                   "/home", "Musics (*.mp3 *.wma *.wav)");
            } else if (qobject_cast<VideoWidget *>(m_pWidget)) {
                files = QFileDialog::getOpenFileNames( this, tr("选择需要添加的内容"),
                                                                   "/home", "Videos (*.avi *.rmvb *.mp4 *.flv)");
            }
            m_pWidget->addList(files);
            getData();
        }
    });

    // 移除
    connect(ui->btn_remove, &QPushButton::clicked, [this] {
        if (ui->listWidget->currentRow() == -1) {
            QMessageBox::warning(this, tr("移除内容警告"),
                                 tr("没有选中要移除的内容行,\n"
                                    "请选中要移除的内容行后按移除按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pWidget->remove(ui->listWidget->currentRow());
            getData();
        }
    });

    // 清空
    connect(ui->btn_clean, &QPushButton::clicked, [this] {
        int ret = QMessageBox::warning(this, tr("清空警告"),
                                       tr("您是否确实要清空内容列表?"),
                                       QMessageBox::Yes | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes) {
            m_pWidget->clearList();
            getData();
        }
    });

    // 上移
    connect(ui->btn_upper, &QPushButton::clicked, [this] {
        int nCurrentRow = ui->listWidget->currentRow();
        if (nCurrentRow == -1) {
            QMessageBox::warning(this, tr("移动内容警告"),
                                 tr("没有选中要移动的内容行,\n"
                                    "请选中要移动的内容行后按移动按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pWidget->moveLayerUP(nCurrentRow);
            getData();
            if (nCurrentRow > 0) {
                ui->listWidget->setCurrentRow(nCurrentRow-1);
            } else if(nCurrentRow == 0) {
                ui->listWidget->setCurrentRow(0);
            }
        }
    });

    // 下移
    connect(ui->btn_lower, &QPushButton::clicked, [this] {
        int nCurrentRow = ui->listWidget->currentRow();
        if (nCurrentRow == -1) {
            QMessageBox::warning(this, tr("移动内容警告"),
                                 tr("没有选中要移动的内容行,\n"
                                    "请选中要移动的内容行后按移动按钮。"),
                                 QMessageBox::Ok);
        } else {
            m_pWidget->moveLayerDown(nCurrentRow);
            getData();
            if (nCurrentRow < (ui->listWidget->count()-1)) {
                ui->listWidget->setCurrentRow(nCurrentRow+1);
            } else if(nCurrentRow == (ui->listWidget->count()-1)) {
                ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
            }
        }
    });
}

void MVPropertyWidget::getData()
{
    if (m_pWidget != nullptr) {
        ui->lineEdit_name->setText(m_pWidget->getControlName());

        if (qobject_cast<VideoWidget *>(m_pWidget)) {
            ui->volumeSlider->setValue(m_pWidget->getVolume());
            ui->volumeSpinBox->setValue(m_pWidget->getVolume());
        }

        // 全部删除列表
        while (ui->listWidget->count() > 0) {
            ui->listWidget->removeItemWidget(ui->listWidget->item(0));
            ui->listWidget->takeItem(0);
        }

        // 这里可能有内存泄漏
        QStringList list = m_pWidget->getList();

        for(int i=0; i<list.size(); i++) {
            auto *pMVListItemWidget = new MVListItemWidget;
            pMVListItemWidget->setFilePath(list.at(i));

            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            ui->listWidget->addItem(pItem);
            ui->listWidget->setItemWidget(pItem, pMVListItemWidget);
            pItem->setSizeHint(QSize(0, 40));
        }
    }
}
