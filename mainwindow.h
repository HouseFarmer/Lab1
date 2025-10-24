#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QKeyEvent>
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
    QString expression;
    QString operand;       // 当前操作数
    QString opcode;        // 当前运算符
    QStack<QString> operands;  // 操作数栈
    QStack<QString> opcodes;   // 运算符栈

private slots:
    void btnNumClicked();         // 数字按钮点击
    void btnUnaryOperatorClicked(); // 一元运算符点击
    void btnBinaryOperatorClicked(); // 二元运算符点击
    void on_btnEquals_clicked();   // 等号按钮点击
    void on_btnClear_clicked();    // 清除按钮点击
    void on_btnPeriod_clicked();   // 小数点按钮点击
    void on_btnDel_clicked();      // 删除按钮点击

    void on_btnClearAlll_clicked();

    void on_btnSign_clicked();
    void keyPressEvent(QKeyEvent *event) override;


private:
    Ui::MainWindow *ui;
    double calculate(double num1, double num2, const QString &op); // 计算函数
};
#endif // MAINWINDOW_H
