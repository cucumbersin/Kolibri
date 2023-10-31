#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFileInfo>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    std::unordered_map<std::string,int> hashmap;
    QDir dir_sours_out;
    QDir dir_sours;
    QStringList mask_list;
    QTimer* timer;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

    void mainsignal();

private slots:

    void on_timer_operation_pressed();

    void on_one_time_operation_pressed();

    void on_directory_selection_clicked();

    void on_start_button_clicked();

    void on_directory_selection_out_clicked();

    void main_operation();

    void on_stop_button_clicked();

private:

    void interface_blocking(bool block);
};
#endif // MAINWINDOW_H
