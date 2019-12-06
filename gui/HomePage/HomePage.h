#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include "dao/MaterialDao.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = 0);
    ~HomePage();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

private:
    Ui::HomePage *ui;
    QTimer *m_pTimer;
    MaterialDao m_materialDao;
};

#endif // HOMEPAGE_H
