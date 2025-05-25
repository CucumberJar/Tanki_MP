#include <QApplication>
#include <QFile>
#include "Game.h"
#include "TankClient.h"

int main(int argc, char *argv[]) {
    QFile logFile("log.txt");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream logStream(&logFile);
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &, const QString &msg) {
        QFile f("log.txt");
        if (f.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream s(&f);
            s << msg << '\n';
            f.close();
        }
    });

    QApplication app(argc, argv);
    Game game;
    game.start();
    return app.exec();
}
