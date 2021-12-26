#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clean_clicked()
{
    ui->gsWidget->cleanPoints();
}

void MainWindow::on_end_clicked()
{
    ui->gsWidget->endPolygon();
}
