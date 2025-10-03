#include <QApplication>
#include "Game.h"
#include "ConnectionWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ConnectionWindow window;

    QObject::connect(&window, &ConnectionWindow::startGame, [&]() {
        QString nickname = window.getNickname();
        QString serverIP = window.getServerIP();
        Game* game = new Game(nickname, serverIP, nullptr);

        game->start();
    });

    window.show();
    return app.exec();
}
