#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
class QPushButton;
class QButtonGroup;

class SideBarWidget : public QWidget
{
    Q_OBJECT
public:
    enum FileType{                          // 文件类型
        AllType,
        ImageType,
        VideoType,
        AudioType,
        DocumentType,
        OtherType
    };

    explicit SideBarWidget(QWidget *parent = nullptr);
    inline FileType getCurrentType() const { return m_nCurrentType; }

signals:
    void typeClicked(FileType FileType);    // 发送选择类型

private:
    void initializeUi();                    // 初始化界面
    void handleEvents();                    // 信号与槽处理

    QPushButton *p_btnAll;                  // 所有分类按钮
    QPushButton *p_btnImage;                // 图片分类按钮
    QPushButton *p_btnVideo;                // 视频分类按钮
    QPushButton *p_btnAudio;                // 音频分类按钮
    QPushButton *p_btnDocument;             // 文档分类按钮
    QPushButton *p_btnOther;                // 其它分类按钮
    QButtonGroup *p_groupButtons;           // 分类按钮组

    FileType m_nCurrentType;                // 当前按钮选中的类型
};

#endif // SIDEBARWIDGET_H
