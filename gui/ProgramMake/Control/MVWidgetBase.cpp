#include "MVWidgetBase.h"
#include "../../MaterialManagement/OpenFile/VideoPlayerTool.h"
#include "ControlBorderWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>
#include "util/Json.h"

MVWidgetBase::MVWidgetBase(QWidget *parent) : ControlWidget(parent)
{
    initializeUi();
    handleEvents();
}

void MVWidgetBase::addList(const QStringList &filePaths)
{
    m_strListFiles += filePaths;
    initializeData();
}

bool MVWidgetBase::remove(int index)
{
    if (m_strListFiles.count() >= index) {
        m_strListFiles.removeAt(index);
        initializeData();
        return true;
    }
    return false;
}

void MVWidgetBase::clearList()
{
    m_strListFiles.clear();
    initializeData();
}

void MVWidgetBase::moveLayerUP(int index)
{
    if ((m_strListFiles.count() > 0) && (m_strListFiles.count() >= index)) {
        if (index > 0) {
            m_strListFiles.move(index, index-1);
        }
    }
    initializeData();
}

void MVWidgetBase::moveLayerDown(int index)
{
    if ((m_strListFiles.count() > 0) && (m_strListFiles.count() >= index)) {
        if (index < m_strListFiles.count()-1) {
            m_strListFiles.move(index, index+1);
        }
    }
    initializeData();
}

void MVWidgetBase::setVolume(int volume)
{
    if (volume >= 0 && volume <= 100) {
        m_pVideoPlayerTool->setVolume(volume);
    }
}

int MVWidgetBase::getVolume() const
{
    return m_pVideoPlayerTool->getVolume();
}

void MVWidgetBase::initializeUi()
{
    m_pVideoPlayerTool = new VideoPlayerTool;
    m_pVideoPlayerTool->setTWidgetVisible(false);
    m_pVideoPlayerTool->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pVideoPlayerTool);
    pVLayout->setContentsMargins(0, 0, 0, 0);
    pVLayout->setSpacing(0);
    this->setLayout(pVLayout);
    // 将边框放在顶层
    this->raiseControlBorder();
}

void MVWidgetBase::initializeData()
{
    if (m_strListFiles.isEmpty()){
        this->setControlBorderBackgroundVisible(true);
    } else {
        this->setControlBorderBackgroundVisible(false);
    }

    m_pVideoPlayerTool->setFilePaths(m_strListFiles);
}

void MVWidgetBase::handleEvents()
{

}

QJsonDocument MVWidgetBase::toJson()
{
    // 列表
    QJsonObject listObject;
    QStringList list = getList();
    if (!list.isEmpty()) {
        for(int i = 0; i < list.size(); i++) {
           listObject.insert(QString::number(i), list.at(i));
        }
    }

    QJsonObject json;
    json.insert("Name", getControlName());
    json.insert("ListSize", list.size());
    json.insert("List", QJsonObject(listObject));
    json.insert("Volume", getVolume());

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    return document;
}

bool MVWidgetBase::toBean(const QJsonDocument &doc)
{
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    Json nJson(strJson, false);

    this->setControlName(nJson.getString("Name"));
    this->setVolume(nJson.getInt("Volume"));

    int size = nJson.getInt("ListSize");
    QStringList list;
    for (int i=0; i<size; i++) {
        QString strVideo = "List." + QString::number(i);
        list.append(nJson.getString(strVideo));
    }
    addList(list);
    initializeData();
}
