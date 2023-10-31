#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "longlongvalidator.h"

#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(main_operation()));
    connect(this,SIGNAL(mainsignal()),this, SLOT(main_operation()));
    ui->bit_val->setValidator(new LongLongValidator());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}





void MainWindow::on_timer_operation_pressed()
{
    ui->timer->setEnabled(true);
    ui->units_time->setEnabled(true);
}


void MainWindow::on_one_time_operation_pressed()
{
    ui->timer->setEnabled(false);
    ui->units_time->setEnabled(false);
}


void MainWindow::on_directory_selection_clicked()
{
    QString st = QFileDialog::getExistingDirectory();
    ui->dir->setText(st);
}


void MainWindow::on_start_button_clicked()
{
    QString mask_str = ui->mask->text();
    QString dir_sours_str = ui->dir->text();
    QString bit_val_str = ui->bit_val->text();
    QString dir_sours_out_str = ui->dir_out->text();
    if(mask_str.isEmpty()){
        ui->status->insertPlainText(QTime::currentTime().toString()+ ":" +"не задана маска\n");
        return;
    }
    if(bit_val_str.isEmpty()){
        ui->status->insertPlainText(QTime::currentTime().toString()+ ":" +"не задано 8 битное число\n");
        return;
    }
    dir_sours.setPath(dir_sours_str);
    if(dir_sours_str.isEmpty() || !dir_sours.exists()){
        ui->status->insertPlainText(QTime::currentTime().toString()+ ":" + "путь для чтения не валиден\n");
        return;
    }
    dir_sours_out.setPath(dir_sours_out_str);
    if(dir_sours_out_str.isEmpty() || !dir_sours_out.exists()){
        ui->status->insertPlainText(QTime::currentTime().toString()+ ":" + "путь для записи не валиден\n");
        return;
    }
    mask_list = mask_str.split(',', Qt::SkipEmptyParts);
    ui->status->insertPlainText(QTime::currentTime().toString()+ ":"
                                + "количество маск "
                                + QString::number(mask_list.size())
                                +":\n" );
    for(auto& elem : mask_list){
        elem = '*' + elem;
        ui->status->insertPlainText(elem + '\n');
    }
    bool one_time = ui->one_time_operation->isChecked();
    if(one_time){
        emit mainsignal();
    }else{
        long long timer_val = ui->timer->value();
        switch (ui->units_time->currentIndex()) {
            case 1:{
                timer_val *= 60;
                break;
            }
            case 2:{
                timer_val *= 60 * 60;
                break;
            }
        }
        timer_val *= 1000;
        timer->start(timer_val);
        interface_blocking(true);
    }

}


void MainWindow::on_directory_selection_out_clicked()
{
    QString st = QFileDialog::getExistingDirectory();
    ui->dir_out->setText(st);
}

void MainWindow::main_operation()
{
    QList<QFileInfo> file_list = dir_sours.entryInfoList(mask_list);
    for(auto& elem : file_list){
        ui->status->insertPlainText(elem.fileName() + '\n');
    }
    bool del_input_file = ui->del_file->isChecked();

    long long byte_val = ui->bit_val->text().toLongLong();
    QByteArray bit_val_array;
    QDataStream dataStream(&bit_val_array, QIODevice::WriteOnly);
    dataStream << byte_val;
    bool modification = ui->action_repetition->currentIndex();
    if(true){
        for(auto& file_info : file_list){
            QFile file(file_info.filePath());
            ui->status->insertPlainText(QTime::currentTime().toString()
                                        + ":читаемый файл " + file.fileName() + '\n');
            if(file.open(QIODevice::ReadOnly)){
                QFile file_out(dir_sours_out.absolutePath() + "/" + file_info.fileName());
                if(modification){
                    if(hashmap[file_info.fileName().toStdString()]){
                        file_out.setFileName(dir_sours_out.absolutePath() + "/"
                                             + QString::number(hashmap[file_info.fileName().toStdString()])
                                             + file_info.fileName());
                    }
                    ++hashmap[file_info.fileName().toStdString()];
                }
                if(!file_out.open(QIODevice::WriteOnly)){
                    ui->status->insertPlainText(QTime::currentTime().toString()
                                                + ":" + file_out.fileName()
                                                + " не получилось открыть\n");
                    continue;
                }
                while (!file.atEnd()) {
                    QByteArray input_byte_array = file.read(8);
                    for(size_t i = 0; i < input_byte_array.size();i++){
                        input_byte_array[i] ^= bit_val_array[i];
                    }
                    file_out.write(input_byte_array);
                }
                if(del_input_file){
                    if (file.remove()){
                        ui->status->insertPlainText(QTime::currentTime().toString()
                                                    + " файл ввода удалён\n");
                    }else{
                        ui->status->insertPlainText(QTime::currentTime().toString()
                                                    + " файл ввода не удалён\n");
                    }
                }
            }else{
                ui->status->insertPlainText(" не получилось открыть файл\n");
            }
        }
    }
}

void MainWindow::interface_blocking(bool block)
{
    ui->stop_button->setEnabled(block);

    ui->directory_selection->setEnabled(!block);
    ui->directory_selection_out->setEnabled(!block);
    ui->start_button->setEnabled(!block);
    ui->del_file->setEnabled(!block);
    ui->timer->setEnabled(!block);
    ui->action_repetition->setEnabled(!block);
    ui->timer_operation->setEnabled(!block);
    ui->bit_val->setEnabled(!block);
    ui->one_time_operation->setEnabled(!block);
    ui->dir->setEnabled(!block);
    ui->dir_out->setEnabled(!block);
    ui->mask->setEnabled(!block);
    ui->units_time->setEnabled(!block);
}

void MainWindow::on_stop_button_clicked()
{
    interface_blocking(false);
    timer->stop();
}

