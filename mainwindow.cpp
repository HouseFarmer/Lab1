#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 数字按钮连接
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

    // 一元运算符连接
    connect(ui->btnPercentage, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnInverse, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnSquare, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);
    connect(ui->btnSqrt, &QPushButton::clicked, this, &MainWindow::btnUnaryOperatorClicked);

    // 二元运算符连接
    connect(ui->btnPlus, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnMultiple, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);
    connect(ui->btnDivide, &QPushButton::clicked, this, &MainWindow::btnBinaryOperatorClicked);

    connect(ui->btnEqual,&QPushButton::clicked, this, &MainWindow::on_btnEquals_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        // 如果当前显示为0或者刚完成一次计算，清空后再添加新数字
        if (ui->display->text() == "0" || opcode == "=") {
            operand.clear();
            opcode.clear();
        }

        operand += btn->text();
        ui->display->setText(operand);
        ui->statusbar->showMessage(btn->text() + " clicked");
    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    if (operand.isEmpty() && ui->display->text().isEmpty()) {
        ui->statusbar->showMessage("请先输入数字");
        return;
    }

    // 如果没有当前操作数，从显示获取
    if (operand.isEmpty()) {
        operand = ui->display->text();
    }

    double result = operand.toDouble();
    QString op = qobject_cast<QPushButton *>(sender())->text();

    if (op == "%") {
        result /= 100.0;
    }
    else if (op == "1/x") {
        if (qFuzzyCompare(result, 0.0)) {
            ui->statusbar->showMessage("错误：除数不能为零");
            return;
        }
        result = 1 / result;
    }
    else if (op == "x²") {
        result *= result;
    }
    else if (op == "√x") {
        if (result < 0) {
            ui->statusbar->showMessage("错误：不能对负数开平方");
            return;
        }
        result = sqrt(result);
    }

    operand = QString::number(result);
    ui->display->setText(operand);
    ui->statusbar->showMessage(op + " 运算完成");
}

void MainWindow::on_btnPeriod_clicked()
{
    if (operand.isEmpty() || opcode == "=") {
        // 如果没有操作数或刚完成计算，从0开始
        operand = "0.";
    } else if (!operand.contains(".")) {
        operand += ".";
    }
    ui->display->setText(operand);
}

void MainWindow::on_btnDel_clicked()
{
    if (!operand.isEmpty()) {
        operand.chop(1); // 移除最后一个字符
        if (operand.isEmpty()) {
            operand = "0";
        }
        ui->display->setText(operand);
    }
}

void MainWindow::btnBinaryOperatorClicked()
{
    if (operand.isEmpty()) {
        ui->statusbar->showMessage("请先输入数字");
        return;
    }

    // 如果操作数栈不为空，先计算之前的运算
    if (!operands.isEmpty() && !opcodes.isEmpty()) {
        double num2 = operand.toDouble();
        double num1 = operands.pop().toDouble();
        QString op = opcodes.pop();

        double result = calculate(num1, num2, op);
        operand = QString::number(result);
        ui->display->setText(operand);
    }

    // 将当前操作数和运算符入栈
    operands.push(operand);
    opcode = qobject_cast<QPushButton *>(sender())->text();
    opcodes.push(opcode);


    // 清空当前操作数，准备接收新的操作数
    operand.clear();
    ui->statusbar->showMessage(opcode + " 已选择");
}

void MainWindow::on_btnEquals_clicked()
{
    if (operands.isEmpty() || opcodes.isEmpty() || operand.isEmpty()) {
        ui->statusbar->showMessage("输入不完整");
        return;
    }

    // 执行计算
    double num2 = operand.toDouble();
    double num1 = operands.pop().toDouble();
    QString op = opcodes.pop();

    double result = calculate(num1, num2, op);
    operand = QString::number(result);
    ui->display->setText(operand);

    opcode = "=";// 标记计算完成

    ui->statusbar->showMessage("计算完成");
}

void MainWindow::on_btnClear_clicked()
{
    // 清除所有状态
    operand.clear();
    opcode.clear();
    while (!operands.isEmpty()) operands.pop();
    while (!opcodes.isEmpty()) opcodes.pop();
    ui->display->setText("0");
    ui->statusbar->showMessage("已清除");
}

double MainWindow::calculate(double num1, double num2, const QString &op)
{
    if (op == "+") {
        return num1 + num2;
    } else if (op == "-") {
        return num1 - num2;
    } else if (op == "×") {  // 注意与按钮文本匹配
        return num1 * num2;
    } else if (op == "÷") {  // 注意与按钮文本匹配
        if (qFuzzyCompare(num2, 0.0)) {
            ui->statusbar->showMessage("错误：除数不能为零");
            return 0;
        }
        return num1 / num2;
    }
    return 0;
}

void MainWindow::on_btnClearAlll_clicked()
{
    operand.clear();
    ui->display->setText(operand);
    ui->statusbar->showMessage("已清除");
}


void MainWindow::on_btnSign_clicked()
{
    // 安全获取发送信号的按钮（若不是按钮触发，直接返回）
    QPushButton *clickedBtn = qobject_cast<QPushButton*>(sender());
    if (clickedBtn == nullptr) {
        return;
    }

    // 处理“空操作数”：视为 0
    if (operand.isEmpty()) {
        operand = "0";
    }

    // 切换正负号
    if (operand.startsWith('-')) {
        // 若已有负号，移除负号（变正）
        operand = operand.mid(1); // 从索引1开始截取（跳过负号）
    } else {
        // 若无负号，添加负号（变负）
        operand = "-" + operand;
    }

    // 更新界面显示（假设用 ui->resultDisplay 显示操作数）
    ui->display->setText(operand);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 处理数字键
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        // 将键盘数字转换为对应按钮的文本
        QString num = QString::number(event->key() - Qt::Key_0);
        operand += num;
        ui->display->setText(operand);
        ui->statusbar->showMessage("键盘输入: " + num);
        return;
    }

    // 处理运算符
    switch (event->key()) {
    case Qt::Key_Plus:      // 加号
        ui->btnPlus->click();
        break;
    case Qt::Key_Minus:     // 减号
        ui->btnMinus->click();
        break;
    case Qt::Key_Asterisk:  // 乘号 (*)
        ui->btnMultiple->click();
        break;
    case Qt::Key_Slash:     // 除号 (/)
        ui->btnDivide->click();
        break;
    case Qt::Key_Equal:     // 等号
    case Qt::Key_Return:    // 回车键
    case Qt::Key_Enter:     // 小键盘回车键
        ui->btnEqual->click();
        break;
    case Qt::Key_Period:    // 小数点
    case Qt::Key_Comma:     // 逗号(部分键盘)
        ui->btnPeriod->click();
        break;
    case Qt::Key_Backspace: // 退格键
        ui->btnDel->click();
        break;
    case Qt::Key_Escape:    // ESC键清除
        ui->btnClear->click();
        break;
    default:
        // 不处理的按键交给父类处理
        QMainWindow::keyPressEvent(event);
    }
}



