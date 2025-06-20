#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLineEdit *lineEdit_m1;
    QLineEdit *lineEdit_m2;
    QLineEdit *lineEdit_m3;
    QLineEdit *lineEdit_ground;
    QLineEdit *lineEdit_equ;
    QPushButton *addParameterButton;

};
#endif // MAINWINDOW_H

