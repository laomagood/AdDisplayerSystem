#ifndef SYSTEMSETTINGSWIDGET_H
#define SYSTEMSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SystemSettingsWidget;
}

class SystemSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSettingsWidget(QWidget *parent = 0);
    ~SystemSettingsWidget();

private:
    Ui::SystemSettingsWidget *ui;
};

#endif // SYSTEMSETTINGSWIDGET_H
