#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Sensitivity Analysis");

    this->setStyleSheet(
    "QWidget { background-color: lightblue; }"

    "QLineEdit {"
    "  border: 2px solid #4CAF50;"
    "  border-radius: 8px;"
    "  padding: 8px;"
    "  font-size: 16px;"
    "  color: #333;"
    "  background-color: #f0f0f0;"
    "}"

    "QLabel {"
    "  font-size: 12px;"
    "  color: #444;"
    "}"

    );

    //QMainWindow only supports one centralWidget

    QWidget *centralWidget = new QWidget(this); //makes a blank container
    this->setCentralWidget(centralWidget);


    QVBoxLayout *layout = new QVBoxLayout(centralWidget); // creates vertical box layout

    auto addInputWithLabel = [&](const QString &placeholder, const QString &labelText) {

        //creates textbox and label
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText(placeholder);
        QLabel *label = new QLabel(labelText, this);

        //makes its own vertical box layout to put inside the centralWidget: texbox and label in one layout
        QVBoxLayout *onelayout = new QVBoxLayout();
        onelayout->addWidget(lineEdit, 0, Qt::AlignLeft);
        onelayout->addWidget(label, 0, Qt::AlignLeft);

        //add this layout to main layout
        layout->addLayout(onelayout);
        return lineEdit; // return lineEdit to use later
    };

    lineEdit_m1 = addInputWithLabel("Enter value for m1", "m1");
    lineEdit_m2 = addInputWithLabel("Enter value for m2", "m2");
    lineEdit_m3 = addInputWithLabel("Enter value for m3", "m3");
    lineEdit_ground = addInputWithLabel("Enter value for ground", "Ground");

    layout->addStretch();
    layout->setAlignment(Qt::AlignTop);

}

MainWindow::~MainWindow()
{

}


