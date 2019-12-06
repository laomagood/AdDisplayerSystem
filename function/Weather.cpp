#include "Weather.h"
#include "util/Json.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QTimer>

#define WeatherUpdataTime 5*60*1000 // 5分钟

Weather::Weather(QWidget *parent) : QWidget(parent)
{
    m_naManager = new QNetworkAccessManager(this);
    connect(m_naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    getWebWeather();

    mTimer = new QTimer(this);
    mTimer->start(WeatherUpdataTime);
    connect(mTimer, &QTimer::timeout, [this] {
        getWebWeather();
    });
}

void Weather::requestFinished(QNetworkReply *reply)
{
    QByteArray byteArray = reply->readAll();
    parseJson(byteArray);
}

void Weather::getWebWeather()
{
    QNetworkRequest quest;
    quest.setUrl(QUrl("https://www.tianqiapi.com/api/?version=v1&appid=[45665466]&appsecret=[aJT4zy5x]"));
    m_naManager->get(quest);
}

void Weather::parseJson(QByteArray byteArray)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) {
            QJsonObject object = doucment.object();
            if (object.contains("city")) {
                QJsonValue value = object.value("city");
                if (value.isString()) {
                    m_nCity = value.toString();
                }
            }
            if (object.contains("cityEn")) {
                QJsonValue value = object.value("cityEn");
                if (value.isString()) {
                    m_nCityEn = value.toString();
                }
            }
            if (object.contains("data")) {
                QJsonValue value = object.value("data");
                if (value.isArray()) {
                    QJsonArray array = value.toArray();
                    QJsonValue value = array.at(0);
                    QJsonObject object = value.toObject();
                    if (object.contains("wea")) {
                        QJsonValue value = object.value("wea");
                        if (value.isString()) {
                            m_nWeather = value.toString();
                        }
                    }
                    if (object.contains("wea_img")) {
                        QJsonValue value = object.value("wea_img");
                        if (value.isString()) {
                            m_nWeatherImage = value.toString();
                        }
                    }
                    if (object.contains("tem1")) {
                        QJsonValue value = object.value("tem1");
                        if (value.isString()) {
                            m_nTemperatureH = value.toString();
                        }
                    }
                    if (object.contains("tem2")) {
                        QJsonValue value = object.value("tem2");
                        if (value.isString()) {
                            m_nTemperatureL = value.toString();
                        }
                    }
                }
            }
        }
    }

//    qDebug() << m_nCity << m_nWeather << m_nTemperatureH << m_nTemperatureL;
    emit updateWeather();
}
