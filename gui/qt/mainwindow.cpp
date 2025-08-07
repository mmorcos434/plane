#include "mainwindow.h"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  border-radius: 8px;"
        "  padding: 8px;"
        "  color: black;"
        "}"
        "QLabel {"
        "  font-size: 12px;"
        "  color: #444;"
        "}"
        );

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    auto addInputWithLabel = [&](const QString &placeholder, const QString &labelText) {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText(placeholder);
        lineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        QLabel *label = new QLabel(labelText, this);

        QVBoxLayout *onelayout = new QVBoxLayout();
        onelayout->addWidget(label, 0, Qt::AlignLeft);
        onelayout->addWidget(lineEdit, 0, Qt::AlignLeft);

        layout->addLayout(onelayout);
        return lineEdit;
    };

    lineEdit_m1 = addInputWithLabel("Enter value for m1", "m1");
    lineEdit_m2 = addInputWithLabel("Enter value for m2", "m2");
    lineEdit_m3 = addInputWithLabel("Enter value for m3", "m3");

    // Ground row
    QHBoxLayout *groundRowLayout = new QHBoxLayout();
    QVBoxLayout *groundInputLayout = new QVBoxLayout();
    QLabel *groundLabel = new QLabel("Ground", this);
    lineEdit_ground = new QLineEdit(this);
    lineEdit_ground->setPlaceholderText("Enter value for ground");
    lineEdit_ground->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    // Create the button properly with connection
    QPushButton *paraButton = new QPushButton("Extra Parameters", this);
    paraButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    paraButton->setFixedHeight(45);  // Match your original height
    connect(paraButton, &QPushButton::clicked, this, &MainWindow::onSetParametersClicked);

    // Create container for textbox and button
    QWidget *inputButtonContainer = new QWidget();
    QHBoxLayout *containerLayout = new QHBoxLayout(inputButtonContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(10);  // Space between textbox and button

    // Add widgets to container
    containerLayout->addWidget(lineEdit_ground);
    containerLayout->addWidget(paraButton);  // Using the properly connected button

    // Set up main layout
    groundInputLayout->addWidget(groundLabel, 0, Qt::AlignLeft);
    groundInputLayout->addWidget(inputButtonContainer, 0, Qt::AlignLeft);

    groundRowLayout->addLayout(groundInputLayout);
    layout->addLayout(groundRowLayout);

    // Equation row
    QHBoxLayout *equationRowLayout = new QHBoxLayout();
    QLabel *equLabel = new QLabel("Equation", this);
    layout->addWidget(equLabel, 0, Qt::AlignLeft);

    lineEdit_equ = new QLineEdit(this);
    lineEdit_equ->setPlaceholderText("Enter Equation");
    lineEdit_equ->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    QPushButton *variablesButton = new QPushButton("Set Variables", this);
    variablesButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    variablesButton->setFixedHeight(40);
    connect(variablesButton, &QPushButton::clicked, this, &MainWindow::onSetVariablesClicked);

    equationRowLayout->addWidget(lineEdit_equ);
    equationRowLayout->addWidget(variablesButton);
    layout->addLayout(equationRowLayout);

    layout->addStretch();
    layout->setAlignment(Qt::AlignTop);
}

void MainWindow::onSetVariablesClicked() {
    int varCount;
    if (variables.isEmpty()) {
        bool ok;
        varCount = QInputDialog::getInt(this, tr("Variable Count"),
                                        tr("How many variables?"), 1, 1, 100, 1, &ok);
        if (!ok) return;
    } else {
        varCount = variables.size();
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Variables");
    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

    QVector<QLineEdit*> nameEdits, valueEdits, minEdits, maxEdits, meanEdits, stdEdits;
    QVector<QCheckBox*> monteCarloChecks;
    QVector<QComboBox*> distBoxes;

    for (int i = 0; i < varCount; ++i) {
        QHBoxLayout *row = new QHBoxLayout();

        QLineEdit *nameEdit = new QLineEdit(); nameEdit->setPlaceholderText("Name");
        QLineEdit *valueEdit = new QLineEdit(); valueEdit->setPlaceholderText("Value");
        QCheckBox *monteCheck = new QCheckBox("Monte Carlo?");
        QComboBox *distBox = new QComboBox(); distBox->addItems({"Uniform", "Normal"});

        QLineEdit *minEdit = new QLineEdit(); minEdit->setPlaceholderText("Min");
        QLineEdit *maxEdit = new QLineEdit(); maxEdit->setPlaceholderText("Max");
        QLineEdit *meanEdit = new QLineEdit(); meanEdit->setPlaceholderText("Mean");
        QLineEdit *stdEdit = new QLineEdit(); stdEdit->setPlaceholderText("Std");

        if (i < variables.size()) {
            const Variable &v = variables[i];
            nameEdit->setText(v.name);
            valueEdit->setText(QString::number(v.value));
            monteCheck->setChecked(v.monteCarlo);
            distBox->setCurrentText(v.distribution);
            if (v.distribution == "Uniform") {
                minEdit->setText(QString::number(v.min));
                maxEdit->setText(QString::number(v.max));
            } else {
                meanEdit->setText(QString::number(v.mean));
                stdEdit->setText(QString::number(v.std));
            }
        }

        bool isUniform = (distBox->currentText() == "Uniform");
        minEdit->setVisible(isUniform);
        maxEdit->setVisible(isUniform);
        meanEdit->setVisible(!isUniform);
        stdEdit->setVisible(!isUniform);

        connect(distBox, &QComboBox::currentTextChanged, [=](const QString &text){
            bool isUni = (text == "Uniform");
            minEdit->setVisible(isUni);
            maxEdit->setVisible(isUni);
            meanEdit->setVisible(!isUni);
            stdEdit->setVisible(!isUni);
        });

        row->addWidget(nameEdit);
        row->addWidget(valueEdit);
        row->addWidget(monteCheck);
        row->addWidget(distBox);
        row->addWidget(minEdit);
        row->addWidget(maxEdit);
        row->addWidget(meanEdit);
        row->addWidget(stdEdit);

        nameEdits.append(nameEdit);
        valueEdits.append(valueEdit);
        monteCarloChecks.append(monteCheck);
        distBoxes.append(distBox);
        minEdits.append(minEdit);
        maxEdits.append(maxEdit);
        meanEdits.append(meanEdit);
        stdEdits.append(stdEdit);

        dialogLayout->addLayout(row);
    }

    QPushButton *okBtn = new QPushButton("Save Changes");
    dialogLayout->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        variables.clear();
        for (int i = 0; i < varCount; ++i) {
            Variable var;
            var.name = nameEdits[i]->text();
            var.value = valueEdits[i]->text().toDouble();
            var.monteCarlo = monteCarloChecks[i]->isChecked();
            var.distribution = distBoxes[i]->currentText();

            if (var.distribution == "Uniform") {
                var.min = minEdits[i]->text().toDouble();
                var.max = maxEdits[i]->text().toDouble();
            } else {
                var.mean = meanEdits[i]->text().toDouble();
                var.std = stdEdits[i]->text().toDouble();
            }

            variables.append(var);
        }

        qDebug() << "Updated variables:";
        for (const auto &v : variables) {
            qDebug() << v.name << v.value << v.monteCarlo << v.distribution
                     << "Min:" << v.min << "Max:" << v.max << "Mean:" << v.mean << "Std:" << v.std;
        }
    }
}

void MainWindow::onSetParametersClicked() {
    bool ok;
    int paramCount = QInputDialog::getInt(this, tr("Parameter Count"),
                                          tr("How many extra parameters?"), 1, 1, 100, 1, &ok);
    if (!ok) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Extra Parameters");
    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

    QVector<QLineEdit*> nameEdits, valueEdits;

    for (int i = 0; i < paramCount; ++i) {
        QHBoxLayout *row = new QHBoxLayout();
        QLineEdit *nameEdit = new QLineEdit(); nameEdit->setPlaceholderText("Name");
        QLineEdit *valueEdit = new QLineEdit(); valueEdit->setPlaceholderText("Value");
        row->addWidget(nameEdit);
        row->addWidget(valueEdit);
        nameEdits.append(nameEdit);
        valueEdits.append(valueEdit);
        dialogLayout->addLayout(row);
    }

    QPushButton *okBtn = new QPushButton("Save Parameters");
    dialogLayout->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        parameters.clear();
        for (int i = 0; i < paramCount; ++i) {
            QString name = nameEdits[i]->text();
            double value = valueEdits[i]->text().toDouble();
            parameters[name] = value;
        }

        qDebug() << "Saved parameters:";
        for (auto it = parameters.begin(); it != parameters.end(); ++it) {
            qDebug() << it.key() << ":" << it.value();
        }
    }
}

MainWindow::~MainWindow() {}
