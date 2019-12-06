#include "SystemSettingsWidget.h"
#include "ui_SystemSettingsWidget.h"

SystemSettingsWidget::SystemSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemSettingsWidget)
{
    ui->setupUi(this);
}

SystemSettingsWidget::~SystemSettingsWidget()
{
    delete ui;
}
