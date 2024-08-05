#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sendWidget = new SendWidget(ui->sendTab);
    logWidget = new LogWidget(ui->logTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}


