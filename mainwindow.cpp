#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 使用正确的clicked()信号
    connect(ui->btnNum0, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum1, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum2, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum3, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum4, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum5, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum6, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum7, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum8, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
    connect(ui->btnNum9, &QPushButton::clicked, this, &MainWindow::btnNumClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    // 获取发送信号的按钮
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        // 获取当前显示的文本，然后追加新的数字
        QString currentText = ui->display->text();
        currentText += btn->text();
        ui->display->setText(currentText);

        // 在状态栏显示信息
        ui->statusbar->showMessage(btn->text() + " clicked");
    }
}


void MainWindow::on_btnPeriod_clicked()
{
    QString str = ui->display->text();
    if(!str.contains("."))
        str+=qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(str);
}


void MainWindow::on_btnDel_clicked()
{
    QString str = ui->display->text();
    str = str.left(str.length()-1);
    ui->display->setText(str);

}

