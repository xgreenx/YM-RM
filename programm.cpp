#include "Programm.h"
#include <QThread>
#include <QEventLoop>
#include "myexception.h"
#include <QTimer>
#include "data_type.h"
#include <qdebug.h>

Programm::Programm(){
	CA = steps.begin();
	
	selection = *(new QTextEdit::ExtraSelection);

	QColor lineColor = QColor(Qt::green).lighter(180);

	selection.format.setBackground(lineColor);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	selection.cursor.clearSelection();
}

void Programm::clearSteps(){
	steps.clear();
}

void Programm::addStep(std::pair<QString, Operation *> step){
	steps.push_back(step);
}

QVector<std::pair<QString, Operation*>>::iterator Programm::getElement(int number){
	return (steps.begin() + number);
}

size_t Programm::size(){
	return steps.size();
}

QString Programm::showVariables(){
	QString str = "";
	foreach (Variable a, variabls) {
		str += a.toString() + "\n";
	}	
	return str;
}

QString Programm::toPseudoCode(){
	QMap<QString, QString> code;
	
	QMap<QString, QString> nameOfVariabls;
	
	int j = 0;
	
	QString currentEnd = "Repeat_" + itos(j);
	
	foreach (auto var, steps) {
		
		code[var.first] = var.second->operationToString();
		
		if(var.second->typeOfOperation == INOPERATION){
			
			nameOfVariabls[((InitializationOperation*)var.second)->getOperation()] = ((InitializationOperation*)var.second)->getName();
			
		}else if(var.second->typeOfOperation == TAOPERATION){
			
			code[var.first] += " " + currentEnd + ";";
			
			QString currentBegin = ((TransitionOperation*)var.second)->getAdress();
			
			QMap<QString, QString>::iterator begin = code.find(currentBegin), end = code.find(currentEnd);
			//делаем строку = табуляция + метка конца + остальной текст
			int pos = begin.value().indexOf(QRegExp("\\S"));
			
			begin.value() = begin.value().mid(0, pos) + currentEnd 
					+ '[' + ((TransitionOperation*)var.second)->getShift() + "] " + begin.value().mid(pos);
			
			if(currentBegin > currentEnd){
				qSwap(begin, end);				
			}
			++begin;--end;
			
			for(; begin != end;++begin)
				begin.value() = "      " + begin.value();
			
			++j;
			currentEnd = currentEnd.mid(0, currentEnd.indexOf('_')) + "_" + itos(j);
			
		}else{
			for(auto i = nameOfVariabls.begin(); i != nameOfVariabls.end(); ++i){
				
				code[var.first].replace(i.key(), i.value());
			}
			
		}
		
	}
	
	QString finalCode = "";
	foreach (QString var, code) {
		finalCode += var + "\n";
	}
	
	return finalCode;
}

QString Programm::excecute(){
	CA = steps.begin();
	variabls.clear();
	QString err = "";
	for(;CA != steps.end();){
		try{
			
			sleep(parent->speedSlider->value());
			
			highlighter();
			CA->second->eval(this);
			
			parent->out->clear();
			parent->out->appendPlainText(showVariables());
			
		}catch (const MyException*& e){
			err += "Operation " + CA->first + " " + e->whatHappend() + '\n';
			return err;
		}catch(...){
			err += "Operation " + CA->first + " unkown error" + '\n';
			return err;
		}
	}
	
	err += "All is good\n";
	return err;
}

void Programm::setParent(MainWindow *m){
	parent = m;
}

void Programm::highlighter(){
	auto a = parent->field->extraSelections->begin();

	for(;a != parent->field->extraSelections->end(); ++a){
		if(a->format.background() == QColor(Qt::green).lighter(180)){
			break;
		}
	}

	if(parent->field->extraSelections->end() != a){
		parent->field->extraSelections->erase(a);
	}
	
	selection.cursor = QTextCursor(CA->second->block);
	parent->field->extraSelections->append(selection);
	parent->field->setExtraSelections(*parent->field->extraSelections);
}

 
void Programm::sleep(qint64 msec){
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
