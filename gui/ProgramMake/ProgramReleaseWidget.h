#ifndef ProgramReleaseWidget_H
#define ProgramReleaseWidget_H

#include <QWidget>
/**
 * @brief The ProgramReleaseWidget class
 * 节目发布
 */

class ProgramReleaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramReleaseWidget(QWidget *parent = 0);
    ~ProgramReleaseWidget();
    bool openFile(const QString filepath);
    bool toBean(QByteArray byteArray);

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理
};

#endif // ProgramReleaseWidget_H
