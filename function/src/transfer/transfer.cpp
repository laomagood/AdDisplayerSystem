/*
    LANShare - LAN file transfer.
    Copyright (C) 2016 Abdul Aris R. <abdularisrahmanudin10@gmail.com>

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
*/

#include "transfer.h"

Transfer::Transfer(QTcpSocket* socket, QObject* parent)
    : QObject(parent), mFile(nullptr), mSocket(nullptr),
      mPacketSize(-1)
{
    mInfo = new TransferInfo(this, this);
    setSocket(socket);
    mHeaderSize = sizeof(PacketType) + sizeof(mPacketSize);
}

void Transfer::resume()
{
    
}

void Transfer::pause()
{
    
}

void Transfer::cancel()
{
    
}

void Transfer::onReadyRead()
{
    // 如果状态转移为“取消”，则忽略接收到的数据
    if (mInfo->getState() == TransferState::Cancelled)
        return;

    mBuff.append(mSocket->readAll());

    // 如果缓冲区大小是> =标头大小，则缓冲区包含我们可以提取为标头的数据标头->数据包大小（4个字节）和数据包类型（1个字节）
    while (mBuff.size() >= mHeaderSize) {
        if (mPacketSize < 0) {
            memcpy(&mPacketSize, mBuff.constData(), sizeof(mPacketSize));
            mBuff.remove(0, sizeof(mPacketSize));
        }

        if (mBuff.size() > mPacketSize) {
            PacketType type = static_cast<PacketType>(mBuff.at(0));
            QByteArray data = mBuff.mid(1, mPacketSize);

            processPacket(data, type);
            mBuff.remove(0, mPacketSize + 1);

            mPacketSize = -1;
        }
        else {
            break;
        }
    }
}

void Transfer::writePacket(qint32 packetDataSize, PacketType type, const QByteArray &data)
{
    if (mSocket) {
        mSocket->write(reinterpret_cast<const char*>(&packetDataSize), sizeof(packetDataSize));
        mSocket->write(reinterpret_cast<const char*>(&type), sizeof(type));
        mSocket->write(data);
    }
}

void Transfer::processPacket(QByteArray &data, PacketType type)
{
    switch (type) {
    case PacketType::Header : processHeaderPacket(data); break;
    case PacketType::Data : processDataPacket(data); break;
    case PacketType::Finish : processFinishPacket(data); break;
    case PacketType::Cancel : processCancelPacket(data); break;
    case PacketType::Pause : processPausePacket(data); break;
    case PacketType::Resume : processResumePacket(data); break;
    }
}

void Transfer::processHeaderPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processDataPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processFinishPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processCancelPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processPausePacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processResumePacket(QByteArray& data)
{
    Q_UNUSED(data);
}


void Transfer::clearReadBuffer()
{
    mBuff.clear();
    mPacketSize = -1;
}

void Transfer::setSocket(QTcpSocket *socket)
{
    if (socket) {
        mSocket = socket;
        connect(mSocket, &QTcpSocket::readyRead, this, &Transfer::onReadyRead);
    }
}

