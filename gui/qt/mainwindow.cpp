#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Sensitivity Analysis");
    this->setStyleSheet("background-color: lightblue;");


    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);


    QVBoxLayout *layout = new QVBoxLayout(centralWidget);


    QLineEdit *lineEdit_m1 = new QLineEdit(this);
    lineEdit_m1->setPlaceholderText("Enter value of m1");

    lineEdit_m1->setStyleSheet(
        "QLineEdit {"
        "  border: 2px solid #4CAF50;"
        "  border-radius: 8px;"
        "  padding: 8px;"
        "  font-size: 16px;"
        "  color: #333;"
        "  background-color: #f0f0f0;"
        "}"
    );


    layout->addWidget(lineEdit_m1);


    connect(lineEdit_m1, &QLineEdit::returnPressed, this, [lineEdit_m1]() {
        double m1 = lineEdit_m1->text().toDouble();
        QMessageBox::information(nullptr, "Result", "Value entered: " + QString::number(m1));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


