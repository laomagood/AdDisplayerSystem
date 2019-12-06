#ifndef PROGRAMTOOLBARWIDGET_H
#define PROGRAMTOOLBARWIDGET_H

#include <QToolBar>
class QAction;

/**
 * @brief The ProgramToolbarWidget class
 * 界面制作界面的工具栏，提供工具按钮
 */

class ProgramToolbarWidget : public QToolBar
{
    Q_OBJECT
public:
    explicit ProgramToolbarWidget(QWidget *parent = nullptr);

private:
    void initializeUi();    // 初始化界面
    void handleEvents();    // 信号与槽处理

    QAction *p_actNew;
    QAction *p_actOpen;
    QAction *p_actSave;
    QAction *p_actSaveAs;

    QAction *p_actText;
    QAction *p_actSubtitle;

    QAction *p_actImage;
    QAction *p_actMusic;
    QAction *p_actVideo;

    QAction *p_actTime;
    QAction *p_actTimer;
    QAction *p_actWeather;

    QAction *p_actPreview;
    QAction *p_actRelease;
};

#endif // PROGRAMTOOLBARWIDGET_H
