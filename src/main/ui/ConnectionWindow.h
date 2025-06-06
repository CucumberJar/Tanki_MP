#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class ConnectionWindow : public QWidget {
Q_OBJECT
public:
    explicit ConnectionWindow(QWidget *parent = nullptr);
    QString getNickname() const { return nicknameEdit->text(); }
    QString getServerIP() const { return ipEdit->text(); }

signals:
    void startGame();

private:
    QLineEdit *nicknameEdit;
    QLineEdit *ipEdit;
    QPushButton *connectButton;
    QPushButton *exitButton;

    void paintEvent(QPaintEvent *event);
};

#endif // CONNECTIONWINDOW_H
