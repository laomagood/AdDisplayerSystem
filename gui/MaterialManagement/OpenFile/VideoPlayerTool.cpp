/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "VideoPlayerTool.h"
#include "Slider.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QStackedLayout>
#include <QResizeEvent>
#include <QApplication>

using namespace QtAV;
const qreal VOLUME_INTERVAL = 0.04;
const int UNITE = 1000;

VideoPlayerTool::VideoPlayerTool(QWidget *parent) : QWidget(parent),
    m_strVideoCurrentTime("00:00:00"),
    m_nVolume(50)
{
    initializeUi();
    handleEvents();
    // 获取列表
//    QStringList files = QFileDialog::getOpenFileNames(this, "打开文件", "/home", "Videos (*.mp4 *.rmvb *.mp3)");
//    setFilePaths(files);
}

void VideoPlayerTool::setFilePaths(QStringList filePaths)
{
    if (filePaths.isEmpty()) {
        if (m_pPlayer->isPlaying()) {
            m_pPlayer->pause();
        }
        return;
    }

    m_strListFiles = filePaths;
    m_strCurrentFiles = m_strListFiles.at(0);
    m_pPlayer->play(m_strCurrentFiles);
    setVolume(m_nVolume);
}

void VideoPlayerTool::setTWidgetVisible(bool visible)
{
    m_pTWidget->setVisible(visible);
    this->resize(200, 200);
}

void VideoPlayerTool::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_pTWidget->setGeometry(0, 0, width(), height());
}

void VideoPlayerTool::initializeUi()
{
    this->resize(800, 500);

    // 底层视频显示----------------------------------------------------
    m_pPlayer = new AVPlayer(this);
    m_pVideoOutput = new VideoOutput(this);
    if (!m_pVideoOutput->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_pPlayer->setRenderer(m_pVideoOutput);

    // 表层按钮操作----------------------------------------------------
    m_pPlayPauseBtn = new QPushButton();
    m_pPlayPauseBtn->setCheckable(true);
    m_pPlayPauseBtn->setChecked(true);
    m_pPlayPauseBtn->setFixedSize(30, 30);

    m_pVolumeBtn = new QPushButton();
    m_pVolumeBtn->setCheckable(true);
    m_pVolumeBtn->setFixedSize(25, 25);

    m_pTitleLabel = new QLabel("标题");
    m_pVideoTimeLabel = new QLabel("00:00:00/00:00:00");
    m_pTimeSlider = new Slider();
    m_pVolumeSlider = new Slider();

    m_pVolumeSlider->setOrientation(Qt::Horizontal);
    m_pTimeSlider->setOrientation(Qt::Horizontal);
    m_pVolumeSlider->setRange(0, 100);
    m_pVolumeSlider->setFixedWidth(80);
    m_pVolumeSlider->setValue(m_nVolume);

    auto *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addSpacing(10);
    hBoxLayout->addWidget(m_pPlayPauseBtn);
    hBoxLayout->addSpacing(10);
    hBoxLayout->addWidget(m_pVideoTimeLabel);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(m_pVolumeBtn);
    hBoxLayout->addWidget(m_pVolumeSlider);

    // 加一个渐变背景
    auto *pFrame = new QFrame;
    pFrame->setFixedHeight(50);
    QPixmap pixmap("./image/VideoViewerTool/frameBackground.png");
    QPalette paletteFrame;
    paletteFrame.setBrush(pFrame->backgroundRole(), QBrush(pixmap));
    pFrame->setPalette(paletteFrame);
    pFrame->setAutoFillBackground(true);
    pFrame->setLayout(hBoxLayout);

    auto *vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(m_pTitleLabel);
    vBoxLayout->addStretch();
    vBoxLayout->addWidget(m_pTimeSlider);
    vBoxLayout->addWidget(pFrame);
    vBoxLayout->setContentsMargins(0, 10, 0, 0);
    vBoxLayout->setSpacing(0);

    m_pTWidget = new QWidget;
    m_pTWidget->setLayout(vBoxLayout);

    // 表层底层布局----------------------------------------------------
    auto *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(m_pVideoOutput->widget());
    this->setLayout(vBoxLayoutMain);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    m_pTWidget->setParent(this);
    m_pTWidget->setGeometry(0, 0, this->width(), this->height());
    this->setMouseTracking(true);

    // 设置样式
    m_pTitleLabel->setFont(QFont("微软雅黑", 12));
    m_pVideoTimeLabel->setFont(QFont("微软雅黑", 8));
    m_pPlayPauseBtn->setObjectName("PlayPauseBtn");
    m_pVolumeBtn->setObjectName("VolumeBtn");

    QFile file("./resources/qss/VideoViewerTool.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void VideoPlayerTool::handleEvents()
{
    // 声音Slider拖动
    connect(m_pVolumeSlider, &QSlider::sliderMoved, [this](int value) {
        if (m_pVolumeBtn->isChecked()) {
            m_pVolumeBtn->setChecked(false);
        }
        setVolume(value);
    });

    // 声音，静音
    connect(m_pVolumeBtn, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            setVolume(0);
        } else {
            setVolume(m_pVolumeSlider->value());
        }
    });

    // 播放Slider拖动
    connect(m_pTimeSlider, &QSlider::sliderMoved, [this](int value) {
        if (!m_pPlayer->isPlaying())
            return;
        m_pPlayer->seek(qint64(value * UNITE));
    });

    // 暂停/播放
    connect(m_pPlayPauseBtn, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            m_pPlayer->pause(false);
        } else {
            if (!m_pPlayer->isPlaying()) {
                m_pPlayer->play();
                return;
            }
            m_pPlayer->pause(true);
        }
    });

    // 视频播放更新进度条
    connect(m_pPlayer, &AVPlayer::positionChanged, [this](qint64 value) {
        m_pTimeSlider->setRange(0, int(m_pPlayer->duration() / UNITE));
        m_pTimeSlider->setValue(int(value / UNITE));

        m_strVideoCurrentTime = QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss"));
        m_pVideoTimeLabel->setText(m_strVideoCurrentTime + "/" + m_strVideoTotalTime);
    });

    // 视频播放完毕
    connect(m_pPlayer, &AVPlayer::stopped, [this] {
        int index = m_strListFiles.indexOf(m_strCurrentFiles) + 1;
        if (index > (m_strListFiles.size()-1)) {
            index = 0;
        }

        m_strCurrentFiles = m_strListFiles.at(index);
        m_pPlayer->play(m_strCurrentFiles);
    });

    // 视频播放(初始化数据)
    connect(m_pPlayer, &AVPlayer::started, [this] {
        m_strVideoTotalTime = QTime(0, 0, 0).addMSecs(m_pPlayer->mediaStopPosition()).toString(QString::fromLatin1("HH:mm:ss"));
        m_pVideoTimeLabel->setText(m_strVideoCurrentTime + "/" + m_strVideoTotalTime);
        // 标题
        QString strFilaName = QFileInfo(m_strCurrentFiles).fileName();
        m_pTitleLabel->setText(strFilaName);
        setWindowTitle(tr("正在播放: %1").arg(strFilaName));
    });
}

void VideoPlayerTool::setVolume(int value)
{
    if (value < 0 || value > 100) {
        return;
    }
    m_nVolume = value;
    AudioOutput *ao = m_pPlayer ? m_pPlayer->audio() : 0;
    qreal v = qreal(value) * VOLUME_INTERVAL;
    if (ao) {
        // 超过间隔，调节声音
        if (qAbs(int(ao->volume()/VOLUME_INTERVAL) - value) >= int(0.1/VOLUME_INTERVAL)) {
            ao->setVolume(v);
        }
    }
}
