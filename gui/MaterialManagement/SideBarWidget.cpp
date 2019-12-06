#include "SideBarWidget.h"
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QFile>


SideBarWidget::SideBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
    this->p_btnAll->click();
}

void SideBarWidget::initializeUi()
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50, 65, 90));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setFixedWidth(180);

    p_btnAll        = new QPushButton(tr("所有"));
    p_btnImage      = new QPushButton(tr("图片分类"));
    p_btnVideo      = new QPushButton(tr("视频分类"));
    p_btnAudio      = new QPushButton(tr("音频分类"));
    p_btnDocument   = new QPushButton(tr("文档分类"));
    p_btnOther      = new QPushButton(tr("其他"));
    p_btnAll->setObjectName("btnAll");
    p_btnImage->setObjectName("btnImage");
    p_btnVideo->setObjectName("btnVideo");
    p_btnAudio->setObjectName("btnAudio");
    p_btnDocument->setObjectName("btnDocument");
    p_btnOther->setObjectName("btnOther");

    auto vBtnLayout = new QVBoxLayout;
    vBtnLayout->addWidget(p_btnAll);
    vBtnLayout->addWidget(p_btnImage);
    vBtnLayout->addWidget(p_btnVideo);
    vBtnLayout->addWidget(p_btnAudio);
    vBtnLayout->addWidget(p_btnDocument);
    vBtnLayout->addWidget(p_btnOther);
    vBtnLayout->addStretch();
    vBtnLayout->setContentsMargins(0, 0, 0, 0);
    vBtnLayout->setSpacing(0);
    this->setLayout(vBtnLayout);

    p_groupButtons = new QButtonGroup(this);
    p_groupButtons->addButton(p_btnAll);
    p_groupButtons->addButton(p_btnImage);
    p_groupButtons->addButton(p_btnVideo);
    p_groupButtons->addButton(p_btnAudio);
    p_groupButtons->addButton(p_btnDocument);
    p_groupButtons->addButton(p_btnOther);
    p_groupButtons->setExclusive(true);

    for (int i=0; i<p_groupButtons->buttons().size(); i++) {
        p_groupButtons->buttons().at(i)->setCheckable(true);
    }
    p_btnAll->setChecked(true);
    m_nCurrentType = AllType;

    // 设置样式表
    QFile file("./resources/qss/sideBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void SideBarWidget::handleEvents()
{
    // 点击分组按钮，显示与隐藏相应的组员
    connect(this->p_groupButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
        QPushButton *qButton = qobject_cast<QPushButton*> (button);
        if (qButton == p_btnAll) {
            m_nCurrentType = AllType;
            emit typeClicked(AllType);
        } else if (qButton == p_btnImage) {
            m_nCurrentType = ImageType;
            emit typeClicked(ImageType);
        } else if (qButton == p_btnVideo) {
            m_nCurrentType = VideoType;
            emit typeClicked(VideoType);
        } else if (qButton == p_btnAudio) {
            m_nCurrentType = AudioType;
            emit typeClicked(AudioType);
        } else if (qButton == p_btnDocument) {
            m_nCurrentType = DocumentType;
            emit typeClicked(DocumentType);
        }else {
            m_nCurrentType = OtherType;
            emit typeClicked(OtherType);
        }
    });
}
