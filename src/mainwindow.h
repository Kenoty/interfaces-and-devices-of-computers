#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BackgroundWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public BackgroundWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLab1();  // Монитор батареи
    void showLab2();  // Заглушка для лабы 2
    void showLab3();  // Заглушка для лабы 3
    void showLab4();  // Заглушка для лабы 4
    void showLab5();  // Заглушка для лабы 5
    void showLab6();  // Заглушка для лабы 6
    void showLabWindow(const QString &title);

private:
    Ui::MainWindow *ui;
    void setupMainMenu();
};
#endif
