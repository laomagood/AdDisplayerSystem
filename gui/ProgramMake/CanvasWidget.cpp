#include "CanvasWidget.h"
#include "Control/ControlWidget.h"
#include "CanvasLayout.h"
#include "util/Json.h"
#include "global.h"
#include <QDebug>
#include <QApplication>
#include <QFileInfo>
#include <QDesktopWidget>
#include <QMenu>
#include <QJsonArray>
#include <QJsonObject>
#include <QBuffer>
#include <QMouseEvent>

CanvasWidget::CanvasWidget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(800, 500);
    this->setFocusPolicy(Qt::ClickFocus);    // 焦点策略

    initializeUi();
    handleEvents();
}

QList<ControlWidget *> CanvasWidget::getListControlWidget() const
{
    return m_listControlWidget;
}

void CanvasWidget::addControlWidget(ControlWidget *controlWidget)
{
    this->layout()->addWidget(controlWidget);
    controlWidget->move(100, 100);
    m_listControlWidget.append(controlWidget);

    // 删除
    connect(controlWidget, &ControlWidget::removeControl, [this] {
//        qDebug() << "remove";
        ControlWidget *widget = qobject_cast<ControlWidget*>(sender());
        removeControlWidget(widget);
    });

    // 发送控件信号，用于切换属性框
    connect(controlWidget, &ControlWidget::select, [this](QWidget* widget) {
        emit focusWidget(widget);
    });

    emit updateControlList(this);
}

void CanvasWidget::removeControlWidget(ControlWidget *controlWidget)
{
//    qDebug() << "删除部件";
    m_listControlWidget.removeOne(controlWidget);
    this->layout()->removeWidget(controlWidget);
    controlWidget->deleteLater();
    emit updateControlList(this);
}

void CanvasWidget::moveLayerUP(ControlWidget *controlWidget)
{
//    qDebug() << "上移一层";
    int index = m_listControlWidget.indexOf(controlWidget);
    if (index == (m_listControlWidget.size()-1)) {
        return;
    } else {
        m_listControlWidget.at(index+1)->stackUnder(controlWidget);
        m_listControlWidget.swap(index, index+1);
    }
    emit updateControlList(this);
}

void CanvasWidget::moveLayerDown(ControlWidget *controlWidget)
{
//    qDebug() << "下移一层";
    int index = m_listControlWidget.indexOf(controlWidget);
    if (index == 0) {
        return;
    } else {
        controlWidget->stackUnder(m_listControlWidget.at(index-1));
        m_listControlWidget.swap(index, index-1);
    }
    emit updateControlList(this);
}

void CanvasWidget::moveLayerTop(ControlWidget *controlWidget)
{
//    qDebug() << "移动到顶层";
    controlWidget->raise();
    int index = m_listControlWidget.indexOf(controlWidget);
    int last = m_listControlWidget.size()-1;
    if (index != last) {
         m_listControlWidget.swap(index, last);
    }
    emit updateControlList(this);
}

void CanvasWidget::moveLayerBottom(ControlWidget *controlWidget)
{
//    qDebug() << "移动到底层";
    controlWidget->lower();
    int index = m_listControlWidget.indexOf(controlWidget);
    if (index != 0) {
         m_listControlWidget.swap(index, 0);
    }
    emit updateControlList(this);
}

void CanvasWidget::setBackgroundColor(const QColor &backgroundColor)
{
    m_nBackgroundColor = backgroundColor;
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, m_nBackgroundColor);
    this->setPalette(palette);
    setAutoFillBackground(true);
    m_nBackgroundImage.clear();
}

void CanvasWidget::setBackgroundImage(const QString &imageFilePath, FillStyle fillStyle)
{
    m_nBackgroundImage = imageFilePath;
    m_nFillStyle = fillStyle;
    if (!imageFilePath.isEmpty()) {
        QPixmap pixmap(imageFilePath);
        QPalette palette(this->palette());

        if (fillStyle == FillStyle::STRETCH) {
            palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(this->size())));
        } else if(fillStyle == FillStyle::TILE) {
            palette.setBrush(QPalette::Window, QBrush(pixmap));
        }

        this->setPalette(palette);
        setAutoFillBackground(true);
    }
}

void CanvasWidget::setCanvasSize(int w, int h)
{
    // 发布状态，在客户端--------------------------------------------
    if (g_isReleaseProgram) {
        resize(w, h);
        m_nControlWidgetScale = 1;
        return;
    }
    // 制作状态，在服务端--------------------------------------------
    // 获取屏幕可用分辨率：1920x1040 - 1500x880 = 420x160
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect nDeskRect = pDesktopWidget->availableGeometry();

    // 减去菜单栏，控件栏，属性栏，留空后的尺寸
    int nScreenW = nDeskRect.width() -420-100;
    int nScreenH = nDeskRect.height() -160-100;

    // 计算缩放比例
    float nScaleW = (float) w / nScreenW;
    float nScaleH =  (float) h / nScreenH;

    // 计算画布大小（四舍五入后强转）
    if (nScaleW > nScaleH) {
        nScreenH = (int) (h / nScaleW + 0.5);
    } else {
        nScreenW = (int) (w / nScaleH + 0.5);
    }
    // 调整画布大小
    if (nScreenW >0 && nScreenH >0) {
       resize(nScreenW, nScreenH);
    }

    m_nReleaseSize.setX(w);
    m_nReleaseSize.setY(h);

    m_nControlWidgetScale = (float)w/nScreenW;
    qDebug() << "目标大小：" << "w:" << w << "h:" << h;
    qDebug() << "实际大小：" << "w:" << nScreenW << "h:" << nScreenH;
    qDebug() << "缩放比例为" << m_nControlWidgetScale;
}

void CanvasWidget::setControlWidgetGeometry(ControlWidget *controlWidget, int x, int y, int w, int h)
{
    int nRealX = x / m_nControlWidgetScale + 0.5;
    int nRealY = y / m_nControlWidgetScale + 0.5;
    int nRealW = w / m_nControlWidgetScale + 0.5;
    int nRealH = h / m_nControlWidgetScale + 0.5;

    if (controlWidget->parentWidget() == this) {
        controlWidget->setGeometry(nRealX, nRealY, nRealW, nRealH);
    }

//    qDebug() << "Geometry:"<< x << y << w << h;
//    qDebug() << "RGeometry:"<< nRealX << nRealY << nRealW << nRealH;
}

QColor CanvasWidget::getBackgroundColor() const
{
    return m_nBackgroundColor;
}

QString CanvasWidget::getBackgroundImage() const
{
    return m_nBackgroundImage;
}

QList<QRect> CanvasWidget::getListRect()
{
    QList<QRect> listRect;
    for(int i=0; i<m_listControlWidget.size(); i++) {
        listRect.append(m_listControlWidget.at(i)->getRect());
    }
    listRect.append(rect());
    return listRect;
}

void CanvasWidget::selectCanvasWidget()
{
    emit focusWidget(this);
}

QJsonDocument CanvasWidget::toJson()
{
    // 画布发布大小
    QJsonObject releaseSizeObject;
    releaseSizeObject.insert("Width", m_nReleaseSize.x());
    releaseSizeObject.insert("Height", m_nReleaseSize.y());
    // 画布背景颜色
    QJsonObject bgColorObject;
    bgColorObject.insert("R", m_nBackgroundColor.red());
    bgColorObject.insert("G", m_nBackgroundColor.green());
    bgColorObject.insert("B", m_nBackgroundColor.blue());

    // 缩略图
    QString nThumbnail = generateThumbnail();

    // 控件列表
    QJsonObject controlListObject;
    for(int i=0; i<m_listControlWidget.size(); i++) {
        // Geometry(相对于父对象)
        int x = m_listControlWidget.at(i)->geometry().x();
        int y = m_listControlWidget.at(i)->geometry().y();
        int w = m_listControlWidget.at(i)->geometry().width();
        int h = m_listControlWidget.at(i)->geometry().height();

        float nScale = (float)m_nReleaseSize.x()/this->width();

        // 发布大小
        QJsonObject releaseGeometryObject;
        releaseGeometryObject.insert("X", (int)(x*nScale+0.5));
        releaseGeometryObject.insert("Y", (int)(y*nScale+0.5));
        releaseGeometryObject.insert("W", (int)(w*nScale+0.5));
        releaseGeometryObject.insert("H", (int)(h*nScale+0.5));

        QJsonDocument doc = m_listControlWidget.at(i)->toJson();

        QJsonObject json;

        // [1] 控件类型
        json.insert("Type", m_listControlWidget.at(i)->getControlType());
        // [2] 发布尺寸
        json.insert("ReleaseGeometry", releaseGeometryObject);
        // [3] 控件
        json.insert("ControlWidget", doc.object());

        controlListObject.insert(QString::number(i), json);
    }

    // 创建Json———————————————————————————————————————————————————
    QJsonObject json;
    json.insert("Version", "2019V01");
    json.insert("ReleaseSize", releaseSizeObject);

    json.insert("BackgroundColor", bgColorObject);
    json.insert("BackgroundImage", m_nBackgroundImage);
    json.insert("FillStyle", (int)m_nFillStyle);
    json.insert("ControlWidgetCount", m_listControlWidget.size());
    json.insert("ControlWidgetList", controlListObject);
    json.insert("Thumbnail", nThumbnail);

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
//    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
//    QString strJson(byteArray);
//    qDebug() << strJson;

    return document;
}

bool CanvasWidget::toBean(const QJsonDocument &doc)
{
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    Json nJson(strJson, false);

    QColor color;
    color.setRed(nJson.getInt("BackgroundColor.R"));
    color.setGreen(nJson.getInt("BackgroundColor.G"));
    color.setBlue(nJson.getInt("BackgroundColor.B"));
    setBackgroundColor(color);

    this->m_nBackgroundImage = nJson.getString("BackgroundImage");
    this->m_nFillStyle = (FillStyle)nJson.getInt("FillStyle");

    int w = nJson.getInt("ReleaseSize.Width");
    int h = nJson.getInt("ReleaseSize.Height");
    this->setCanvasSize(w, h);
}

QString CanvasWidget::generateThumbnail()
{
    QPixmap pixmap;
    pixmap = this->grab();
    // 缩小尺寸
    if (pixmap.width() > pixmap.height()) {
        int nLength = pixmap.height() * 170 / pixmap.width() ;
        pixmap = pixmap.scaled(170, nLength);
    } else {
        int nLength = pixmap.width()  * 170 / pixmap.height();
        pixmap = pixmap.scaled(nLength, 170);
    }

//    pixmap.save("C:/Users/Technical/Desktop/Ma.png");
    return pixmapTostring(pixmap);
}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget *pWidget = QApplication::widgetAt(mapToGlobal(event->pos()));

    if (event->button() == Qt::LeftButton) {
        if (pWidget == this) {
            emit focusWidget(this);
        }
    }
}

void CanvasWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget *pWidget = QApplication::widgetAt(mapToGlobal(event->pos()));
    if (pWidget != this) {
        ControlWidget *pControlWidget = qobject_cast<ControlWidget *>(pWidget);
        if (pControlWidget == nullptr) {
            return;
        }

        QMenu *menu = new QMenu(this);
        QAction *removeAct = new QAction(tr("删除控件"));
        QAction *moveLayerUPAct = new QAction(tr("上移一层"));
        QAction *moveLayerDownAct = new QAction(tr("下移一层"));
        QAction *moveLayerTopAct = new QAction(tr("移动到顶层"));
        QAction *moveLayerBottomAct = new QAction(tr("移动到底层"));

        menu->addAction(removeAct);
        menu->addAction(moveLayerUPAct);
        menu->addAction(moveLayerDownAct);
        menu->addAction(moveLayerTopAct);
        menu->addAction(moveLayerBottomAct);

        connect(removeAct, &QAction::triggered, [=] {
            removeControlWidget(pControlWidget);
        });
        connect(moveLayerUPAct, &QAction::triggered, [=] {
            moveLayerUP(pControlWidget);
        });
        connect(moveLayerDownAct, &QAction::triggered, [=] {
            moveLayerDown(pControlWidget);
        });
        connect(moveLayerTopAct, &QAction::triggered, [=] {
            moveLayerTop(pControlWidget);
        });
        connect(moveLayerBottomAct, &QAction::triggered, [=] {
            moveLayerBottom(pControlWidget);
        });

        menu->exec(QCursor::pos());
    }
}

void CanvasWidget::initializeUi()
{
    auto *layout = new CanvasLayout;
    this->setLayout(layout);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->setBackgroundColor(QColor(255, 255, 255));
    this->setBackgroundImage("");
}

void CanvasWidget::handleEvents()
{
//    connect(this, &CanvasWidget::focusWidget, [this](QWidget *widget) {
//        qDebug() << "Canvas发出信号" << widget;
    //    });
}

QString CanvasWidget::pixmapTostring(const QPixmap &pixmap)
{
    QByteArray byte_array;
    QDataStream data_stream(&byte_array, QIODevice::WriteOnly);
    data_stream << pixmap;
    QString res = QString::fromLocal8Bit(byte_array.toBase64());
    return res;
}
