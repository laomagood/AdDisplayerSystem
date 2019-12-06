#include "gui/ProgramMake/ProgramReleaseWidget.h"
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
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("MySoft-AD");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Star Runner");

    // 安装消息处理程序
//    qInstallMessageHandler(myMessageOutput);

    // 去掉鼠标
    ProgramReleaseWidget w;
    w.show();
    QApplication::setOverrideCursor(Qt::BlankCursor);

    return a.exec();
}
