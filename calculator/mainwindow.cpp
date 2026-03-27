#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QStatusBar>
#include <QMessageBox>
#include <QStack>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUI();
    setFixedSize(400, 600);
    statusBar()->showMessage("Ready!");

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget;
    setCentralWidget(central);

    EditCal = new QLineEdit;
    Result = new QLabel("0");

    EditCal->setAlignment(Qt::AlignRight);
    Result->setAlignment(Qt::AlignRight);

    EditCal->setMinimumHeight(100);
    Result->setMinimumHeight(40);

    QFont inputFont("Segoe UI", 18);
    QFont resultFont("Segoe UI", 28, QFont::Bold);

    EditCal->setFont(inputFont);
    Result->setFont(resultFont);

    EditCal->setReadOnly(true);
    EditCal->setFocus();

    // Buttons
    btn0 = new QPushButton("0");
    btn1 = new QPushButton("1");
    btn2 = new QPushButton("2");
    btn3 = new QPushButton("3");
    btn4 = new QPushButton("4");
    btn5 = new QPushButton("5");
    btn6 = new QPushButton("6");
    btn7 = new QPushButton("7");
    btn8 = new QPushButton("8");
    btn9 = new QPushButton("9");

    additionBtn = new QPushButton("+");
    subtractionBtn = new QPushButton("−");
    multiplicationBtn = new QPushButton("×");
    divisionBtn = new QPushButton("÷");

    roundBracketopen = new QPushButton("(");
    roundBracketclose = new QPushButton(")");

    dotBtn = new QPushButton(".");
    calculatorBtn = new QPushButton("=");
    deleteBtn = new QPushButton("C");
    delete1Btn = new QPushButton("CE");

    auto connectBtn = [&](QPushButton* btn)
        {
            connect(btn, &QPushButton::clicked, this, [=]() 
                {
                appendText(btn->text());
                });
        };

    QList<QPushButton*> allBtns = {
        btn0,btn1,btn2,btn3,btn4,btn5,btn6,btn7,btn8,btn9,
        additionBtn,subtractionBtn,multiplicationBtn,divisionBtn,
        dotBtn,roundBracketopen,roundBracketclose
    };

    for (auto b : allBtns) connectBtn(b);

    connect(calculatorBtn, &QPushButton::clicked, this, &MainWindow::calculator);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteCalculator);
    connect(delete1Btn, &QPushButton::clicked, this, &MainWindow::deleteOneChar);

    // Style
    central->setStyleSheet(
        "QWidget { background-color: #121212; }"
        "QLineEdit { font-size:20px; padding:12px; background:#1e1e1e; color:#00ffcc; border-radius:10px; }"
        "QLabel { font-size:32px; font-weight:bold; background:black; color:white; padding:12px; border-radius:10px; }"
        "QPushButton { font-size:18px; min-width:70px; min-height:70px; border-radius:10px; background:#333; color:white; }"
        "QPushButton:hover { background:#555; }"
        "QPushButton:pressed { background:#222; }"
        "QPushButton#equalBtn { background:#2196F3; }"
        "QPushButton#deleteBtn { background:#e53935; }"
        "QPushButton#delete1Btn { background:#FFB74D; color:black; }"
    );

    calculatorBtn->setObjectName("equalBtn");
    deleteBtn->setObjectName("deleteBtn");
    delete1Btn->setObjectName("delete1Btn");

    // Layout
    QVBoxLayout* displayLayout = new QVBoxLayout;
    displayLayout->addWidget(EditCal);
    displayLayout->addWidget(Result);

    QGridLayout* grid = new QGridLayout;

    grid->addWidget(additionBtn, 0, 0);
    grid->addWidget(subtractionBtn, 0, 1);
    grid->addWidget(multiplicationBtn, 0, 2);
    grid->addWidget(divisionBtn, 0, 3);

    grid->addWidget(btn7, 1, 0); grid->addWidget(btn8, 1, 1); grid->addWidget(btn9, 1, 2); grid->addWidget(deleteBtn, 1, 3);
    grid->addWidget(btn4, 2, 0); grid->addWidget(btn5, 2, 1); grid->addWidget(btn6, 2, 2); grid->addWidget(delete1Btn, 2, 3);
    grid->addWidget(btn1, 3, 0); grid->addWidget(btn2, 3, 1); grid->addWidget(btn3, 3, 2); grid->addWidget(roundBracketclose, 3, 3);

    grid->addWidget(dotBtn, 4, 0);
    grid->addWidget(btn0, 4, 1);
    grid->addWidget(roundBracketopen, 4, 2);
    grid->addWidget(calculatorBtn, 4, 3);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(displayLayout);
    mainLayout->addLayout(grid);

    central->setLayout(mainLayout);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    QString text = event->text();

    if (key == Qt::Key_Return || key == Qt::Key_Enter) { calculator(); return; }
    if (key == Qt::Key_Backspace) { deleteOneChar(); return; }

    if (key >= Qt::Key_0 && key <= Qt::Key_9) { appendText(text); return; }

    if (text == "+" || text == "-" || text == "*" || text == "/")
    {
        appendText(text); return;
    }

    if (text == "(" || text == ")" || text == ".")
    {
        appendText(text); return;
    }
}

int compareOperator(QChar op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double MainWindow::CalcPair(double a, double b, QChar c)
{
    if (c == '+') return a + b;
    if (c == '-') return a - b;
    if (c == '*') return a * b;
    if (c == '/')
    {
        if (b == 0)
        {
            QMessageBox::warning(this, "Error", "Cannot divide by 0");
            return 0;
        }
        return a / b;
    }
    return 0;
}

void MainWindow::calculator()
{
    QString s = EditCal->text();

    QStack<double> v;
    QStack<QChar> ops;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ') continue;

        if ((s[i].isDigit()) || (s[i] == '-' && (i == 0 || s[i - 1] == '(')))
        {
            bool neg = false;
            if (s[i] == '-') { neg = true; i++; }

            double val = 0;
            while (i < s.length() && s[i].isDigit())
            {
                val = val * 10 + s[i].digitValue();
                i++;
            }

            if (i < s.length() && s[i] == '.')
            {
                i++;
                double frac = 0, base = 0.1;
                while (i < s.length() && s[i].isDigit())
                {
                    frac += s[i].digitValue() * base;
                    base *= 0.1;
                    i++;
                }
                val += frac;
            }

            if (neg) val = -val;
            v.push(val);
            i--;
        }
        else if (s[i] == '(') ops.push(s[i]);
        else if (s[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                double b = v.pop(), a = v.pop();
                v.push(CalcPair(a, b, ops.pop()));
            }
            if (!ops.empty()) ops.pop();
        }
        else
        {
            while (!ops.empty() &&
                compareOperator(ops.top()) >= compareOperator(s[i]))
            {
                double b = v.pop(), a = v.pop();
                v.push(CalcPair(a, b, ops.pop()));
            }
            ops.push(s[i]);
        }
    }

    while (!ops.empty())
    {
        double b = v.pop(), a = v.pop();
        v.push(CalcPair(a, b, ops.pop()));
    }

    if (!v.empty())
    {
        Result->setText(QString::number(v.top(), 'g', 10));

        //animation
        QPropertyAnimation* anim = new QPropertyAnimation(Result, "geometry");
        QRect r = Result->geometry();
        anim->setDuration(150);
        anim->setStartValue(QRect(r.x(), r.y() + 10, r.width(), r.height()));
        anim->setEndValue(r);
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::deleteCalculator()
{
    EditCal->clear();
    Result->setText("0");
}

void MainWindow::deleteOneChar()
{
    QString t = EditCal->text();
    if (!t.isEmpty())
    {
        t.chop(1);
        EditCal->setText(t);
    }
}

void MainWindow::appendText(QString val)
{
    if (val == "×") val = "*";
    if (val == "÷") val = "/";
    if (val == "−") val = "-";

    QString cur = EditCal->text();

    if (!cur.isEmpty())
    {
        QChar last = cur.back();
        if ((last == '+' || last == '-' || last == '*' || last == '/') &&
            (val == "+" || val == "-" || val == "*" || val == "/"))
            return;
    }

    EditCal->setText(cur + val);
}