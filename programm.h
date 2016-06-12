#ifndef PROGRAMM_H
#define PROGRAMM_H

#include "operation.h"
#include "highlighter.h"
#include "mainwindow.h"

class MainWindow;
class Variable;
class Operation;

class Programm{
	public:
		Programm();
		
		QVector<std::pair<QString, Operation*>> steps;
		QMap<QString, Variable> variabls;
		QVector<std::pair<QString, Operation*>>::iterator CA;
		int RK[2];
		
		void clearSteps();
		void addStep(std::pair<QString, Operation*> step);
		QVector<std::pair<QString, Operation*>>::iterator getElement(int number);
		size_t size();
		QString showVariables();
		QString toPseudoCode();
		
		QString excecute();
		void setParent(MainWindow* m);
	private:
		MainWindow* parent;
		QTextEdit::ExtraSelection selection;
		
		void highlighter();
		void sleep(qint64 msec);
};

#endif // PROGRAMM_H