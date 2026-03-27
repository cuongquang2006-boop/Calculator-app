#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QLabel;
class QLineEdit;
class QKeyEvent;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QPushButton* btn0;
    QPushButton* btn1;
    QPushButton* btn2;
    QPushButton* btn3;
    QPushButton* btn4;
    QPushButton* btn5;
    QPushButton* btn6;
    QPushButton* btn7;
    QPushButton* btn8;
    QPushButton* btn9;

    QPushButton* additionBtn;
    QPushButton* subtractionBtn;
    QPushButton* multiplicationBtn;
    QPushButton* divisionBtn;

    QPushButton* roundBracketopen;
    QPushButton* roundBracketclose;

    QPushButton* dotBtn;
    QPushButton* calculatorBtn;
    QPushButton* deleteBtn;
    QPushButton* delete1Btn;

    QLabel* Result;
    QLineEdit* EditCal;

    void setupUI();

private slots:
    double CalcPair(double a, double b, QChar c);
    void calculator();
    void deleteCalculator();
    void deleteOneChar();
    void appendText(QString val);
};

#endif // MAINWINDOW_H