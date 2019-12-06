#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QWidget>
#include "ControlWidget.h"
class QNetworkReply;

class WeatherWidget : public ControlWidget
{
public:
    WeatherWidget(QWidget *parent = nullptr);

private:
    void initializeUi();            // 初始化数据
    void handleEvents();            // 信号与槽处理
    void getWeather();
    void requestFinished(QNetworkReply* reply);

    QString m_nCity;                // 城市
    QString m_nDate;                // 日期
    QString m_nWeek;                // 星期
    QString m_nWeather;             // 天气
    QString m_nWeatherImage;        // 天气图标
    QString m_nTemperature;         // 温度
};

#endif // WEATHERWIDGET_H
