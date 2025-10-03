#ifndef TANKCLIENT_H
#define TANKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "Game.h"
class TankClient;
class Game;
class TankClient : public QObject {
Q_OBJECT
public:
    void setLocalPlayerId(const QString &id);
    TankClient(Game *pGame);
    void connectToServer(const QString &ip, quint16 port);
    void sendJoinMessage(const QString &id);
    QTcpSocket* getSocket() const { return socket; }
signals:
    void spawnTank(const QString &id, int x, int y);
    void moveTank(const QString &id, qreal x, qreal y, qreal angle, qreal turretAngle);
    void removeBlock(int x, int y);
    void destroyTank(const QString &id);
private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onStateChanged(QAbstractSocket::SocketState state);
private:
    QTcpSocket *socket;
    QString localPlayerId;
};
#endif
