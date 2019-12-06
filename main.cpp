#include "gui/LoginWidget.h"
#include "util/qtsingleapplication/qtsingleapplication.h"
#include "gui/MaterialManagement/OpenFile/ImageViewerTool.h"
#include "gui/HomePage/HomePageWeatherWidget.h"
#include "gui/UserCornerMarker.h"
#include "function/Weather.h"
#include "util/TopWindow.h"
#include "util/Util.h"
#include <QApplication>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

// 自定义消息处理程序
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    QtSingleApplication  app(argc, argv);

    QCoreApplication::setOrganizationName("MySoft-AD");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Star Runner");

    // 安装消息处理程序
//    qInstallMessageHandler(myMessageOutput);

    //判断实例是否已经运行
    if(app.isRunning())
    {
        qDebug()<<"this is already running";
        app.sendMessage("raise_window_noop", 4000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }

    LoginWidget w;
    app.setActivationWindow(&w,1);
    w.show();

    return app.exec();
}
