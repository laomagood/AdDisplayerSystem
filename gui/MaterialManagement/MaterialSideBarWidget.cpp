#include "MaterialSideBarWidget.h"
#include "global.h"
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QFile>


MaterialSideBarWidget::MaterialSideBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
    this->p_btnAll->click();
}

void MaterialSideBarWidget::initializeUi()
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50, 65, 90));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setFixedWidth(180);

    p_btnAll        = new QPushButton(QIcon("./image/materialManagement/All.png"), tr("所有"));
    p_btnProgram    = new QPushButton(QIcon("./image/materialManagement/Program.png"), tr("节目分类"));
    p_btnImage      = new QPushButton(QIcon("./image/materialManagement/Image.png"), tr("图片分类"));
    p_btnVideo      = new QPushButton(QIcon("./image/materialManagement/Video.png"), tr("视频分类"));
    p_btnAudio      = new QPushButton(QIcon("./image/materialManagement/Audio.png"), tr("音频分类"));
    p_btnDocument   = new QPushButton(QIcon("./image/materialManagement/Document.png"), tr("文档分类"));
    p_btnOther      = new QPushButton(QIcon("./image/materialManagement/Other.png"), tr("其他"));

    auto vBtnLayout = new QVBoxLayout;
    vBtnLayout->addWidget(p_btnAll);
    vBtnLayout->addWidget(p_btnProgram);
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
    p_groupButtons->addButton(p_btnProgram);
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
    m_nCurrentType = FileType::AllType;

    // 设置样式表
    QFile file("./resources/qss/MaterialSideBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void MaterialSideBarWidget::handleEvents()
{
    // 点击分组按钮，显示与隐藏相应的组员
    connect(this->p_groupButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
        QPushButton *qButton = qobject_cast<QPushButton*> (button);
        if (qButton == p_btnAll) {
            m_nCurrentType = FileType::AllType;
            emit typeClicked(FileType::AllType);
        } else if (qButton == p_btnProgram) {
            m_nCurrentType = FileType::ProgramType;
            emit typeClicked(FileType::ProgramType);
        } else if (qButton == p_btnImage) {
            m_nCurrentType = FileType::ImageType;
            emit typeClicked(FileType::ImageType);
        } else if (qButton == p_btnVideo) {
            m_nCurrentType = FileType::VideoType;
            emit typeClicked(FileType::VideoType);
        } else if (qButton == p_btnAudio) {
            m_nCurrentType = FileType::AudioType;
            emit typeClicked(FileType::AudioType);
        } else if (qButton == p_btnDocument) {
            m_nCurrentType = FileType::DocumentType;
            emit typeClicked(FileType::DocumentType);
        }else {
            m_nCurrentType = FileType::OtherType;
            emit typeClicked(FileType::OtherType);
        }
    });
}
