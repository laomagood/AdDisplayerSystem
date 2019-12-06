#include "HomePageWeatherWidget.h"
#include "ui_HomePageWeatherWidget.h"
#include "function/Weather.h"
#include <QDebug>

HomePageWeatherWidget::HomePageWeatherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePageWeatherWidget)
{
    ui->setupUi(this);
    m_pWeather = new Weather(this);

    initializeUi();
    initializeData();
    handleEvents();
}

HomePageWeatherWidget::~HomePageWeatherWidget()
{
    delete ui;
}

void HomePageWeatherWidget::initializeUi()
{
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
    ui->labelCity->setFont(QFont("微软雅黑", 20));
    ui->labelTemperature->setFont(QFont("微软雅黑", 20));

    QPalette pal;
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    ui->labelCity->setPalette(pal);
    ui->labelTemperature->setPalette(pal);
}

void HomePageWeatherWidget::initializeData()
{
    QString strWeaImg = "./image/HomePage/WeatherImage/";
    strWeaImg += m_pWeather->getWeatherImage();

    ui->labelWeaImg->setPixmap(QPixmap(strWeaImg));
    ui->labelCity->setText(m_pWeather->getCity());
    QString strTemperature = m_pWeather->getTemperatureL() + "~" + m_pWeather->getTemperatureH();
    ui->labelTemperature->setText(strTemperature);
}

void HomePageWeatherWidget::handleEvents()
{
    connect(m_pWeather, &Weather::updateWeather, [this] {
        initializeData();
    });
}
