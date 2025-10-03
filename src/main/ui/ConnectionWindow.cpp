#include "ConnectionWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QPainter>

ConnectionWindow::ConnectionWindow(QWidget *parent) : QWidget(parent) {
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    showFullScreen();

    QFont titleFont("Segoe UI", 32, QFont::Bold);
    QFont labelFont("Segoe UI", 20, QFont::Bold);
    QFont inputFont("Segoe UI", 18);
    QFont buttonFont("Segoe UI", 20, QFont::DemiBold);

    QWidget *panel = new QWidget(this);
    panel->setFixedSize(600, 520);
    panel->setStyleSheet(R"(
        background-color: rgba(33, 33, 33, 220);
        border-radius: 20px;
    )");

    auto *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(50);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 180));
    panel->setGraphicsEffect(shadow);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(panel);

    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(40, 40, 40, 40);
    panelLayout->setSpacing(20);
    panelLayout->setAlignment(Qt::AlignTop);

    QLabel *title = new QLabel("Подключение");
    title->setFont(titleFont);
    title->setMinimumHeight(60);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *nicknameLabel = new QLabel("Никнейм:");
    nicknameLabel->setFixedHeight(50);
    nicknameLabel->setFont(labelFont);
    nicknameLabel->setStyleSheet("color: white;");
    nicknameLabel->setAlignment(Qt::AlignCenter);

    nicknameEdit = new QLineEdit;
    nicknameEdit->setFont(inputFont);
    nicknameEdit->setFixedHeight(50);
    nicknameEdit->setStyleSheet(R"(
        padding: 8px;
        border-radius: 10px;
        background-color: white;
        color: black;
    )");

    QLabel *ipLabel = new QLabel("IP сервера:");
    ipLabel->setFixedHeight(50);
    ipLabel->setFont(labelFont);
    ipLabel->setStyleSheet("color: white;");
    ipLabel->setAlignment(Qt::AlignCenter);

    ipEdit = new QLineEdit;
    ipEdit->setFont(inputFont);
    ipEdit->setText("127.0.0.1");
    ipEdit->setFixedHeight(50);
    ipEdit->setStyleSheet(R"(
        padding: 8px;
        border: 2px solid #666;
        border-radius: 10px;
        background-color: white;
        color: black;
    )");

    connectButton = new QPushButton("Играть");
    connectButton->setFont(buttonFont);
    connectButton->setFixedHeight(60);
    connectButton->setStyleSheet(R"(
        QPushButton {
            background-color: #43A047;
            color: white;
            border-radius: 10px;
            padding: 12px;
        }
        QPushButton:hover {
            background-color: #66BB6A;
        }
    )");

    exitButton = new QPushButton("Выход");
    exitButton->setFont(buttonFont);
    exitButton->setFixedHeight(60);
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #E53935;
            color: white;
            border-radius: 10px;
            padding: 12px;
        }
        QPushButton:hover {
            background-color: #EF5350;
        }
    )");

    infoButton = new QPushButton("О игре");
    infoButton->setFont(buttonFont);
    infoButton->setFixedHeight(50);
    infoButton->setStyleSheet(R"(
    QPushButton {
        background-color: #1E88E5;
        color: white;
        border-radius: 10px;
        padding: 12px;
    }
    QPushButton:hover {
        background-color: #42A5F5;
    }
)");

    panelLayout->addWidget(title);
    panelLayout->addSpacing(50);
    panelLayout->addWidget(nicknameLabel);
    panelLayout->addWidget(nicknameEdit);
    panelLayout->addSpacing(30);
    panelLayout->addWidget(ipLabel);
    panelLayout->addWidget(ipEdit);
    panelLayout->addSpacing(50);
    panelLayout->addWidget(connectButton);
    panelLayout->addSpacing(50);
    panelLayout->addWidget(infoButton);
    panelLayout->addSpacing(100);
    panelLayout->addWidget(exitButton);



    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(connectButton, &QPushButton::clicked, [this]() {
        emit startGame();
        this->hide();
    });
}
void ConnectionWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 10, 30));
    gradient.setColorAt(1, QColor(30, 30, 60));
    painter.fillRect(rect(), gradient);
    QWidget::paintEvent(event);
}
