#ifndef IMAGEVIEWERATTRIBUTES_H
#define IMAGEVIEWERATTRIBUTES_H

#include <QWidget>

namespace Ui {
class ImageViewerAttributes;
}

class ImageViewerAttributes : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewerAttributes(QWidget *parent = 0);
    ~ImageViewerAttributes();

    void setImageName(const QString &name);
    void setImageSize(int size);
    void setUploadTime(const QString &time);
    void setUploader(const QString &uploader);
    void setAuditTime(const QString &time);
    void setAuditor(const QString &auditor);

private:
    Ui::ImageViewerAttributes *ui;

    void initializeUi();                            // 初始化界面
    void initializeData();                          // 初始化数据
    void handleEvents();                            // 信号与槽处理
};

#endif // IMAGEVIEWERATTRIBUTES_H
