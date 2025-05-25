#include "TankClient.h"
#include <QDebug>

TankClient::TankClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &TankClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &TankClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TankClient::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &TankClient::onErrorOccurred);
    connect(socket, &QTcpSocket::stateChanged, this, &TankClient::onStateChanged);

    qDebug() << "[TankClient] Initialized.";
}

void TankClient::connectToServer(const QString &ip, quint16 port) {
    qDebug() << "[TankClient] Connecting to server at" << ip << ":" << port;
    socket->connectToHost(ip, port);
}

void TankClient::sendJoinMessage(const QString &id) {
    localPlayerId = id;
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QString message = QString("JOIN;%1\n").arg(id);
        socket->write(message.toUtf8());
        socket->flush();
    } else {
        qWarning() << "[TankClient] Not connected to server!";
    }
}

void TankClient::onConnected() {
    qDebug() << "[TankClient] Connected.";
    sendJoinMessage(localPlayerId);
}

void TankClient::onReadyRead() {
    while (socket->canReadLine()) {
        QString msg = QString::fromUtf8(socket->readLine()).trimmed();
        if (msg.startsWith("SPAWN;")) {
            QStringList parts = msg.split(';');
            if (parts.size() == 4) {
                QString id = parts[1];
                int x = parts[2].toInt();
                int y = parts[3].toInt();
                emit spawnTank(id, x, y);
            }
        } else if (msg.startsWith("MOVE;")) {
            QStringList parts = msg.split(';');
            if (parts.size() == 5) {
                QString id = parts[1];
                qreal x = parts[2].toDouble();
                qreal y = parts[3].toDouble();
                qreal angle = parts[4].toDouble();
                emit moveTank(id, x, y, angle);
            }
        }
    }
}

void TankClient::onDisconnected() {
    qDebug() << "[TankClient] Disconnected.";
}

void TankClient::sendRawMessage(const QString &msg) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(msg.toUtf8());
        socket->flush();
    }
}
void TankClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qWarning() << "[TankClient] Socket error:" << socketError;
}

void TankClient::onStateChanged(QAbstractSocket::SocketState state) {
    qDebug() << "[TankClient] State changed to:" << state;
}
