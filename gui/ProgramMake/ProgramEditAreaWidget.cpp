#include "ProgramEditAreaWidget.h"
#include "CanvasWidget.h"
#include <QDebug>
#include <QScrollArea>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QApplication>

ProgramEditAreaWidget::ProgramEditAreaWidget(QWidget *parent) :
    QTabWidget(parent),
    m_newFileNumber(1)
{
    initializeUi();
    handleEvents();

//    newCanvas();
}

CanvasWidget *ProgramEditAreaWidget::getCurrentCanvas() const
{
    if (count() == 0) {
        return nullptr;
    }
    return m_widgeCanvastHash.value(currentWidget());
}

void ProgramEditAreaWidget::newCanvas()
{
    // 这里必须手工布局，不然canvas大小设置
    auto pCanvas = new CanvasWidget;

    // 用一个QScrollArea嵌套一下画布
    auto pScrollArea  = new QScrollArea;
    pScrollArea->setWidget(pCanvas);
    pScrollArea->setAlignment(Qt::AlignCenter);
//    pScrollArea->widget()->resize(1200, 600);

    QPalette palette(pScrollArea->palette());
    palette.setColor(QPalette::Background, QColor(127, 127, 127));
    pScrollArea->setPalette(palette);
    pScrollArea->setAutoFillBackground(true);

    // 当tab为空时，发送CanvasWidget信号
    if (this->count() == 0) {
        emit focusWidget(pCanvas);
    }

    this->addTab(pScrollArea, tr("文档%1").arg(m_newFileNumber));
    m_newFileNumber++;
    m_listWidget.append(pScrollArea);

    m_widgeCanvastHash.insert(pScrollArea, pCanvas);

    // 将新建标签设为当前
    this->setCurrentIndex(this->count()-1);

    // 将CanvasWidget的信号转发出去
    connect(pCanvas, &CanvasWidget::focusWidget, [this](QWidget *widget) {
        emit focusWidget(widget);
    });
    connect(pCanvas, &CanvasWidget::updateControlList, [this](CanvasWidget *widget) {
        emit updateControlList(widget);
    });
}

void ProgramEditAreaWidget::openCanvas()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    qDebug() << fileName;
}

void ProgramEditAreaWidget::mousePressEvent(QMouseEvent *event)
{
    QString str = QApplication::widgetAt(mapToGlobal(event->pos()))->objectName();
    if (str == "qt_scrollarea_viewport") {
        emit focusWidget(m_widgeCanvastHash.value(currentWidget()));
    }
}

void ProgramEditAreaWidget::initializeUi()
{
    this->setMovable(true);
    this->setTabsClosable(true);
    emit focusWidget(nullptr);
}

void ProgramEditAreaWidget::handleEvents()
{
    // 关闭文档
    connect(this, &ProgramEditAreaWidget::tabCloseRequested, [this](int index) {
        QString tabText = this->tabText(index);
        int ret = QMessageBox::warning(this, tr("关闭文档"), tr("您正在关闭\"%1\"，您是否需要对其进行保存？").arg(tabText),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                       QMessageBox::Yes);
        switch (ret) {
        case QMessageBox::Yes:
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "/home/jana/untitled.png", tr("Images (*.png *.xpm *.jpg)"));
            qDebug() << fileName;
            if (fileName.isEmpty()){
                return;
            }
            // 直接穿透到下个case
        }
        case QMessageBox::No:
            m_widgeCanvastHash.remove(this->widget(index));
            m_listWidget.removeOne(this->widget(index));
            this->widget(index)->deleteLater();
            removeTab(index);
            break;
        default:
            break;
        }

        if (this->count() == 0) {
            emit focusWidget(nullptr);
            emit updateControlList(nullptr);
        }
    });

    // 切换tab发送CanvasWidget信号
    connect(this, &ProgramEditAreaWidget::currentChanged, [this] {
        if (!m_widgeCanvastHash.isEmpty()) {
            qDebug() << "m_widgeCanvastHash:" << m_widgeCanvastHash.count();
            emit focusWidget(m_widgeCanvastHash.value(currentWidget()));
        }
    });
}
