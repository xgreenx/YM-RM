#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data_type.h"

#include "codeeditor.h"
#include <QTextStream>
#include <QLayout>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
	
    ui->setupUi(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(12);

	QGridLayout *layout = new QGridLayout;
	
    field = new CodeEditor();
	field->setFont(font);
	
	layout->addWidget(field, 0, 0);
	
	out = new QPlainTextEdit();
	out->setReadOnly(true);
	layout->addWidget(out, 0, 1);
	
	pseudoEditor = new QPlainTextEdit();	
	pseudoEditor->setReadOnly(true);
	layout->addWidget(pseudoEditor, 0, 2);
	
	logger = new CodeEditor();	
	logger->setReadOnly(true);
	layout->addWidget(logger, 1, 0, 1, 2);
	
	QGridLayout *layoutMiniBar = new QGridLayout;
	
	speedSlider = new QSlider;
	speedSlider->setMaximum(2000);
	layoutMiniBar->addWidget(speedSlider, 0, 0, 0, 10);
	
	QPushButton * start = new QPushButton;
	connect(start, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
	start->setText("Start");
	layoutMiniBar->addWidget(start, 0, 1);
	
	
	layout->addLayout(layoutMiniBar, 1, 2);
	
	QWidget * window = new QWidget;
	window->setLayout(layout);
	
	this->setCentralWidget(window);
	
	highlighter = new Highlighter(field->document());
	highlighter->setParent(this);

    QFile inputFile("temp.txt");

    QString text;
    if (inputFile.open(QIODevice::ReadOnly)){
       QTextStream in(&inputFile);

       text = in.readAll();

       inputFile.close();
    }

    field->appendPlainText(text);
}

void MainWindow::saveText(){
    QFile outputFile("temp.txt");

    if (outputFile.open(QIODevice::WriteOnly)){
       QTextStream out(&outputFile);

       out << field->toPlainText();

       outputFile.close();
    }
}

MainWindow::~MainWindow(){
    saveText();

    delete ui;
}

void MainWindow::on_pushButton_clicked(){
	logger->appendPlainText(highlighter->programm->excecute());
	//QWidget *d = new QWidget(NULL);
	//d->show();
}
