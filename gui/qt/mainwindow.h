#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QMap>
#include <QVector>


struct Variable {
    QString name;
    double value;
    bool monteCarlo;
    QString distribution;
    double min, max;
    double mean, std;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSetVariablesClicked();
    void onSetParametersClicked();

private:
    QLineEdit *lineEdit_m1;
    QLineEdit *lineEdit_m2;
    QLineEdit *lineEdit_m3;
    QLineEdit *lineEdit_ground;
    QLineEdit *lineEdit_equ;

    QMap<QString, double> parameters;
    QVector<Variable> variables;
};

