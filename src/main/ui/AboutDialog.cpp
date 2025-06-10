#include "AboutDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Об игре");
    setFixedSize(400, 300);
    setStyleSheet("background-color: #222; color: white;");
    auto *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(R"(
        <h3>Многопользовательская игра "Танки"</h3>
        <p>Проект разработан в рамках курсовой работы.</p>
        <p>Клиент: Qt (C++)</p>
        <p>Сервер: Java</p>
        <p>Возможности:</p>
        <ul>
        <li>Сетевая игра</li>
        <li>Разрушаемое окружение</li>
        <li>Синхронизация танков</li>
        </ul>
    )");
    label->setWordWrap(true);
    label->setTextFormat(Qt::RichText);
    layout->addWidget(label);

    auto *closeBtn = new QPushButton("Закрыть");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);
}
