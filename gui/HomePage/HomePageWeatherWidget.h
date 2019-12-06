#ifndef HOMEPAGEWEATHERWIDGET_H
#define HOMEPAGEWEATHERWIDGET_H

#include <QWidget>
class Weather;

namespace Ui {
class HomePageWeatherWidget;
}

class HomePageWeatherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomePageWeatherWidget(QWidget *parent = 0);
    ~HomePageWeatherWidget();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

private:
    Ui::HomePageWeatherWidget *ui;
    Weather *m_pWeather;
};

#endif // HOMEPAGEWEATHERWIDGET_H
