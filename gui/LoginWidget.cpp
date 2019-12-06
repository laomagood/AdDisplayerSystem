#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "gui/MainWidget.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
// 返回登陆状态
bool LoginWidget::isLoginSuccess() const
{
    return loginSuccess;
}
// 静态登陆窗口
bool LoginWidget::login()
{
    LoginWidget *loginWidget = new LoginWidget();
    return loginWidget->isLoginSuccess();
}
// 初始化
void LoginWidget::initializeUi()
{
    this->setFixedSize(700, 375);
    this->setWindowTitle(tr("用户登录"));

    // 设置背景图片
    QImage image2;
    image2.load("./image/User/loginwidget/LoginBackground.png");
    setAutoFillBackground(true);   // 这个属性一定要设置
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(image2.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);

    // 设置边距
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setMargin(0);

    // 界面布局
    ui->nameLabel->setText(tr(""));
    ui->loginLabel->setText(tr("账户登陆"));
    ui->companyLabel->setText(tr("xxxxxx有限公司版权所有"));
    ui->logoLabel->setFixedSize(150, 35);
    ui->logoLabel->setHidden(true);
    ui->horizontalLayoutCenter->layout()->setContentsMargins(30, 0, 0, 0);
    ui->UserLoginWidget->layout()->setContentsMargins(30, 15, 30, 40);
    ui->UserLoginWidget->layout()->setSpacing(30);
    ui->horizontalLayout->layout()->setSpacing(0);
    ui->horizontalLayout_2->layout()->setSpacing(0);
    ui->horizontalLayoutBottom->layout()->setContentsMargins(10, 0, 0, 0);

    QFont font;
    font.setLetterSpacing(QFont::PercentageSpacing, 110);
    ui->nameLabel->setFont(font);

    // 输入限制
    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->userLineEdit->setValidator( validator );
    ui->passwordLineEdit->setValidator( validator );

    // 账号密码
    ui->userLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->userLineEdit->setPlaceholderText(tr("账号"));
    ui->userLineEdit->setMaxLength(16);
    ui->passwordLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit->setPlaceholderText(tr("密码"));
    ui->passwordLineEdit->setMaxLength(16);
    ui->loginButton->setText(tr("登    录"));
    ui->userLineEdit->setFocus();

    // 设置样式表
    QFile file("./resources/qss/LoginWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    } else {
        qDebug() << "样式表无法加载···";
    }
}

// 事件处理
void LoginWidget::handleEvents()
{
    // 输入密码，按回车
    connect(ui->userLineEdit, &QLineEdit::returnPressed, [this] { ui->passwordLineEdit->setFocus();});
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, [this]{ui->loginButton->click();});
    connect(ui->loginButton, &QPushButton::clicked, [this]{
        QString username = ui->userLineEdit->text();
        QString password = ui->passwordLineEdit->text();
        // 账号密码不可为空
        if (username == NULL || password == NULL) {
            QMessageBox::warning(this, QObject::tr("登陆失败"), QObject::tr("账号或密码为空，请重新输入"));
            return;
        }
        // 登陆逻辑
        loginSuccess = login(username, password);

        // 登陆判断
        if (loginSuccess) {
            auto pWidget = new MainWidget;
            pWidget->show();
            this->close();
        } else {
            QMessageBox::warning(this, QObject::tr("登陆失败"), QObject::tr("账号或密码不正确，请检查您的输入是否输入有误"));
        }
    });
}
// 登陆逻辑
bool LoginWidget::login(const QString &username, const QString &password)
{
    // 加密密码
    // [1] 使用加密将密匙保存在配置文件中，读取并解密
    QString key = "ma10010";
    // [2] 密码+密匙
    QString pwdKey = password + key;
    // [3] 使用MD5加密
    QByteArray bytePwdMd5 = QCryptographicHash::hash(pwdKey.toLatin1(), QCryptographicHash::Md5);
    QString PwdMd5 = bytePwdMd5.toHex();
    qDebug() << PwdMd5;

    // [4] 匹配数据库中的账号密码
    if (username == "0" && password == "0") {
        return true;
    } else {
        return false;
    }
}
