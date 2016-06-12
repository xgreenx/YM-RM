#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include <QMainWindow>
#include <QSlider>
#include "highlighter.h"

namespace Ui {
class MainWindow;
}

class Highlighter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CodeEditor* field, *logger;//указатели на наш класс codeEditor (field-поле вводa,logger-jib,rb
    QPlainTextEdit* out, *pseudoEditor;//out- вывод переменных и значений, pesudoEditor-поле для псевдокода
	QSlider* speedSlider;
    explicit MainWindow(QWidget *parent = 0);
    void saveText();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
	

private:
    Ui::MainWindow *ui;
    Highlighter *highlighter;
};

#endif // MAINWINDOW_H
