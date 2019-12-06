#include "WeatherWidget.h"
#include "util/Json.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>

WeatherWidget::WeatherWidget(QWidget *parent) : ControlWidget(parent)
{
    initializeUi();
    handleEvents();
}

void WeatherWidget::initializeUi()
{
    getWeather();
//    QTextCodec *codec = QTextCodec::codecForName("utf8");
//    QString replyData = codec->toUnicode( reply->readAll() );
}

void WeatherWidget::handleEvents()
{

}

void WeatherWidget::getWeather()
{
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    Q_ASSERT(connRet);

    // 天气api不能写在这里，要写在json配置文件中
    request.setUrl(QUrl("https://tianqiapi.com/api.php?style=te"));
    QNetworkReply* reply = naManager->get(request);

    reply->deleteLater();
    reply = nullptr;

}


void WeatherWidget::requestFinished(QNetworkReply* reply)
{
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    } else {
        // 获取返回内容
        qDebug() << reply->readAll();
    }
}
