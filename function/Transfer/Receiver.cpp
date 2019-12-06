#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QMessageBox>
#include "util/Util.h"
#include "receiver.h"
#include "../settings.h"

Receiver::Receiver(const Device& sender, QTcpSocket* socket, QObject* parent)
    : Transfer(socket, parent), mSenderDev(sender), mFileSize(0), mBytesRead(0)
{
    mInfo->setState(TransferState::Waiting);
    connect(mSocket, &QTcpSocket::disconnected, this, &Receiver::onDisconnected);

    mInfo->setPeer(sender);
}

void Receiver::onDisconnected()
{
    mInfo->setState(TransferState::Disconnected);
    emit mInfo->errorOcurred("Sender disconnected");
}

void Receiver::processHeaderPacket(QByteArray& data)
{
    // 文件大小，文件名，文件夹名
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    mFileSize = obj.value("size").toVariant().value<qint64>();
    mInfo->setDataSize(mFileSize);

    QString fileName = obj.value("name").toString();
    QString folderName = obj.value("folder").toString();
    QString dstFolderPath = Singleton<Settings>::getInstance().getDownloadDir();
    if (!folderName.isEmpty())
        dstFolderPath = dstFolderPath + QDir::separator() + folderName;

    // 如果“下载目录”中的文件夹不存在，请创建该文件夹。
    QDir dir(dstFolderPath);
    if (!dir.exists()) {
        dir.mkpath(dstFolderPath);
    }

    QString dstFilePath = dstFolderPath + QDir::separator() + fileName;
    // 如果未选中覆盖选项，请重命名文件，以使其不重叠
    if (!Singleton<Settings>::getInstance().getReplaceExistingFile()) {
        dstFilePath = Util::getUniqueFileName(fileName, dstFolderPath);
    }

//    mInfo->setFilePath(dstFilePath);
    mFile = new QFile(dstFilePath, this);
    if (mFile->open(QIODevice::WriteOnly)) {
        mInfo->setState(TransferState::Transfering);
//        emit mInfo->fileOpened();
    }
    else {
        emit mInfo->errorOcurred(tr("Failed to write ") + dstFilePath);
    }
}

void Receiver::processDataPacket(QByteArray& data)
{
    if (mFile && mBytesRead + data.size() <= mFileSize) {
        mFile->write(data);
        mBytesRead += data.size();

        mInfo->setProgress( (int)(mBytesRead * 100 / mFileSize) );
    }
}

void Receiver::processFinishPacket(QByteArray& data)
{
    Q_UNUSED(data);

    mInfo->setState(TransferState::Finish);
    mFile->close();
    mSocket->disconnectFromHost();
    emit mInfo->done();
}
