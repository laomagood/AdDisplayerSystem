#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
class QNetworkAccessManager;
class QNetworkReply;

class Weather : public QWidget
{
    Q_OBJECT
public:
    explicit Weather(QWidget *parent = nullptr);

    inline QString getCity() const { return m_nCity; }
    inline QString getCityEn() const { return m_nCityEn; }
    inline QString getWeather() const { return m_nWeather; }
    inline QString getWeatherImage() const { return m_nWeatherImage; }
    inline QString getTemperatureL() const { return m_nTemperatureL; }
    inline QString getTemperatureH() const { return m_nTemperatureH; }

signals:
    void updateWeather();

private slots:
    void getWebWeather();                           // 获取天气
    void requestFinished(QNetworkReply* reply);     // Get

private:
    void parseJson(QByteArray byteArray);           // 解析Json

private:
    QNetworkAccessManager* m_naManager;
    QString m_nCity;                // 城市
    QString m_nCityEn;              // 城市
    QString m_nWeather;             // 天气
    QString m_nWeatherImage;        // 天气图标
    QString m_nTemperatureL;        // 温度低
    QString m_nTemperatureH;        // 温度高

    QTimer *mTimer;
};

#endif // WEATHER_H
