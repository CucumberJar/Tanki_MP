#include "TankClient.h"
#include <QDebug>

TankClient::TankClient(Game *parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &TankClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &TankClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TankClient::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &TankClient::onErrorOccurred);
    connect(socket, &QTcpSocket::stateChanged, this, &TankClient::onStateChanged);

    qDebug() << "[TankClient] Initialized.";
}

void TankClient::connectToServer(const QString &ip, quint16 port) {
    qDebug() << "[TankClient] Attempting to connect to server at" << ip << ":" << port;
    socket->connectToHost(ip, port);
}

void TankClient::sendJoinMessage(const QString &id) {
    localPlayerId = id;
    qDebug() << "[TankClient] Preparing to send JOIN message. Socket state:" << socket->state();
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QString message = QString("JOIN;%1\n").arg(id);
        qDebug() << "[TankClient] Sending join message:" << message.trimmed();
        qint64 written = socket->write(message.toUtf8());
        bool flushed = socket->flush();
        qDebug() << "[TankClient] Bytes written:" << written << ", flush status:" << flushed;
    } else {
        qWarning() << "[TankClient] Cannot send JOIN, socket not connected!";
    }
}

void TankClient::onConnected() {
    qDebug() << "[TankClient] Connected to server.";
    if (!localPlayerId.isEmpty()) {
        qDebug() << "[TankClient] Sending join message on connection.";
        sendJoinMessage(localPlayerId);
    } else {
        qWarning() << "[TankClient] localPlayerId is empty at connection time!";
    }
}

void TankClient::onReadyRead() {
    qDebug() << "[TankClient] Data available to read.";
    while (socket->canReadLine()) {
        QString msg = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << "[TankClient] Received message:" << msg;

        if (msg.startsWith("SPAWN;")) {
            QStringList parts = msg.split(';');
            if (parts.size() == 4) {
                QString id = parts[1];
                int x = parts[2].toInt();
                int y = parts[3].toInt();
                qDebug() << "[TankClient] Emitting spawnTank signal with id:" << id << "x:" << x << "y:" << y;
                emit spawnTank(id, x, y);
            }
        } else if (msg.startsWith("MOVE;")) {
            QStringList parts = msg.split(';');
            if (parts.size() == 5) {
                QString id = parts[1];
                qreal x = parts[2].toDouble();
                qreal y = parts[3].toDouble();
                qreal angle = parts[4].toDouble();
                qDebug() << "[TankClient] Emitting moveTank signal with id:" << id << "x:" << x << "y:" << y << "angle:" << angle;
                emit moveTank(id, x, y, angle);
            }
        } else if (msg.startsWith("PLAYERS;")) {
            // Формат: PLAYERS;id1;nick1;inBattle1;kills1;deaths1;id2;nick2;inBattle2;kills2;deaths2;...
            QStringList parts = msg.split(';');
            QList<PlayerInfo> players;

            // Пропускаем "PLAYERS" в parts[0], читаем далее по 5 элементов
            for (int i = 0; i + 4 < parts.size(); i += 5) {
                PlayerInfo p;
                p.id = parts[i];
                p.nickname = parts[i + 1];
                p.inBattle = (parts[i + 2] == "1");
                p.kills = parts[i + 3].toInt();
                p.deaths = parts[i + 4].toInt();
                players.append(p);
            }
            qDebug() << "[TankClient] Parsed players list, emitting playersListUpdated with" << players.size() << "players.";
            emit playersListUpdated(players);
        }
    }
}

void TankClient::onDisconnected() {
    qDebug() << "[TankClient] Disconnected from server.";
}

void TankClient::sendRawMessage(const QString &msg) {
    qDebug() << "[TankClient] Preparing to send raw message. Socket state:" << socket->state();
    if (socket->state() == QAbstractSocket::ConnectedState) {
        qint64 written = socket->write(msg.toUtf8());
        bool flushed = socket->flush();
        qDebug() << "[TankClient] Raw message sent. Bytes written:" << written << ", flush status:" << flushed;
    } else {
        qWarning() << "[TankClient] Cannot send raw message, socket not connected!";
    }
}

void TankClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qWarning() << "[TankClient] Socket error:" << socketError
               << "-" << socket->errorString();
}

void TankClient::onStateChanged(QAbstractSocket::SocketState state) {
    qDebug() << "[TankClient] Socket state changed to:" << state;
}

void TankClient::setLocalPlayerId(const QString &id) {
    localPlayerId = id;
}
