#include "operation.h"
#include "myexception.h"
#include <QDebug>
#include "data_type.h"

void Operation::eval(Programm*){
	//basic function
}

QString Operation::toString(Token* t){
	return t == NULL?"NULL":t->text;
}

QString Operation::operationToString(){
	QString str = "Basic operation";
	return str;
}
bool Operation::valid(Token* t){
	return t == NULL?0:1;
}

QString Operation::toAdress(QString s, int x){
	int shex = s.toInt(NULL, 16);
	shex += x;
	shex %= mod;
	s = htos(shex);
	int need = 4 - s.size();
	
	while(need){
		s = "0" + s;
		--need;
	}
	
	s.toUpper();
	return s;
}

bool Operation::contains(const QString& str, Programm* prog){
	foreach (auto a, prog->steps) {
		if((a.first == str) || toAdress(a.first, 1) == str) return true;
	}
	return false;
}

QVector<std::pair<QString, Operation*>>::iterator Operation::find(const QString & str, Programm* prog){
	for(auto i = prog->steps.begin(); i != prog->steps.end(); ++i) {
		if(i->first == str)
			return i;
	} 
	
	return prog->steps.end();
}

RegistrRegistrOperation::RegistrRegistrOperation(Token* op, Token* r1, Token* r2, QTextBlock _block){
	operation = op; reg1 = r1; reg2 = r2;
	typeOfOperation = RROPERATION;
	block = _block;
	
	if(valid(operation) && valid(reg1)  && valid(reg2) && reg1->type == REGISTR && reg2->type == REGISTR) return;
	
	else throw new MyException("Error '" + toString(operation) + " " + toString(reg1) + " " +
							   toString(reg2) + "' when must be '" + toString(operation) + " REGISTR REGISTR'");
}

QString RegistrRegistrOperation::operationToString(){
	if(operation->text == "20"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " := " + Highlighter::toString(reg2->type) + reg2->text  + ";";
		
	}else if(operation->text == "99"){
		
		return "stop";
	
	}else if(operation->text == "21"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " += " + Highlighter::toString(reg2->type) + reg2->text + ";";
	
	}else if(operation->text == "22"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " -= " + Highlighter::toString(reg2->type) + reg2->text + ";";
	
	}else if(operation->text == "23"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " *= " + Highlighter::toString(reg2->type) + reg2->text + ";";
	
	}else if(operation->text == "33"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " = |" + Highlighter::toString(reg1->type) + reg1->text 
				+ "| * |" + Highlighter::toString(reg2->type) + reg2->text + "|;";
	
	}else if(operation->text == "24"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " /= " + Highlighter::toString(reg2->type) + reg2->text + "; "
				+ Highlighter::toString(reg1->type) + toAdress(reg1->text, 1)[4 - 1] + " %= " 
				+ Highlighter::toString(reg2->type) + reg2->text + ";";
		
	}else if(operation->text == "34"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " = |" + Highlighter::toString(reg1->type) + reg1->text + "| / |" 
				+ Highlighter::toString(reg2->type) + reg2->text + "|; "
				+ Highlighter::toString(reg1->type) + toAdress(reg1->text, 1)[4 - 1] + " = |" + Highlighter::toString(reg1->type) + reg1->text 
				+ "| % |" + Highlighter::toString(reg2->type) + reg2->text + "|;";
		
	}else if(operation->text == "25"){
		
		return "Регистр сравнения = " + Highlighter::toString(reg1->type) + reg1->text + " - " + Highlighter::toString(reg2->type) + reg2->text + ";";
	};
	return "Error";
}

void RegistrRegistrOperation::eval(Programm* prog){//"99 20 21 22 23 33 24 34 25"
	if(operation->text == "99"){
		prog->CA = (prog->steps.end() - 1);
		
	}else if(operation->text == "20"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		prog->variabls[reg1->text].value = prog->variabls[reg2->text].value;
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "21"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value + prog->variabls[reg2->text].value;	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		
	}else if(operation->text == "22"){
		if(reg1->text != reg2->text){
			
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
			
			prog->variabls[reg1->text].value = prog->variabls[reg1->text].value - prog->variabls[reg2->text].value;
			
		}else{
			prog->variabls[reg1->text].value = 0;
			
		}
		
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "23"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value * prog->variabls[reg2->text].value;	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		
	}else if(operation->text == "33"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		prog->variabls[reg1->text].value = qAbs(prog->variabls[reg1->text].value) * qAbs(prog->variabls[reg2->text].value);	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		
	}else if(operation->text == "24"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		else if(prog->variabls[reg2->text].value == 0) throw new MyException("division by zero " + reg2->text + " = 0");
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value / prog->variabls[reg2->text].value;	
		int s = reg1->text.toInt(NULL, 16);
		++s;
		s %= (0x10);
		QString str = htos(s);
		
		prog->variabls[str.toUpper()].value = prog->variabls[reg1->text].value % prog->variabls[reg2->text].value;	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		prog->variabls[str.toUpper()].name = "Registr " + str.toUpper();
		
	}else if(operation->text == "34"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		else if(prog->variabls[reg2->text].value == 0) throw new MyException("division by zero " + reg2->text + " = 0");
		
		prog->variabls[reg1->text].value = qAbs(prog->variabls[reg1->text].value) / qAbs(prog->variabls[reg2->text].value);	
		int s = reg1->text.toInt(NULL, 16);
		++s;
		s %= (0x10);
		QString str = htos(s);
		
		prog->variabls[str.toUpper()].value = qAbs(prog->variabls[reg1->text].value) % qAbs(prog->variabls[reg2->text].value);	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		prog->variabls[str.toUpper()].name = "Registr " + str.toUpper();	
		
	}else if(operation->text == "25"){
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		else if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		prog->RK[0] = prog->variabls[reg1->text].value; prog->RK[1] = prog->variabls[reg2->text].value;
		
	}
	
	++prog->CA;
}

RegistrMemoryOperation::RegistrMemoryOperation(Token *op, Token *r1, Token *r2, Token *mem, QTextBlock _block){
	operation = op; reg1 = r1; reg2 = r2; memory = mem;
	typeOfOperation = RMOPERATION;
	block = _block;
	
	
	if(valid(operation) && valid(reg1) && valid(reg2) && valid(memory) 
			&& reg1->type == REGISTR && reg2->type == REGISTR && memory->type == ADRESS) return;
	
	else throw new MyException("Error '" + toString(operation) + " " + toString(reg1) + " " +
							   toString(reg2) + " " + toString(mem) + "' when must be '" + toString(operation) + " REGISTR REGISTR ADRESS'");
}

QString RegistrMemoryOperation::operationToString(){
	if(operation->text == "00"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " <- "
				+ memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
		
	}else if(operation->text == "10"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " -> "
				+ memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
	
	}else if(operation->text == "01"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " += " + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
	
	}else if(operation->text == "02"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " -= " + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
	
	}else if(operation->text == "03"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " *= " + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
	
	}else if(operation->text == "13"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " = |" + Highlighter::toString(reg1->type) + reg1->text 
				+ "| * |" + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "]|;";
	
	}else if(operation->text == "04"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " /= " + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "]; "
				+ Highlighter::toString(reg1->type) + toAdress(reg1->text, 1)[4 - 1] + " %= " 
				+ memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "];";
		
	}else if(operation->text == "14"){
		
		return Highlighter::toString(reg1->type) + reg1->text + " = |" + Highlighter::toString(reg1->type) + reg1->text + "| / |"
				+ memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "]|; "
				+ Highlighter::toString(reg1->type) + toAdress(reg1->text, 1)[4 - 1] + " = |" + Highlighter::toString(reg1->type) + reg1->text 
				+ "| % |" + memory->text + '[' + Highlighter::toString(reg2->type) + reg2->text + "]|;";
		
	}else if(operation->text == "05"){
		
		return "Регистр сравнения = " + Highlighter::toString(reg1->type) + reg1->text + " - " + memory->text + 
				'[' + Highlighter::toString(reg2->type) + reg2->text + "];";
	};
	return "Error";
}

void RegistrMemoryOperation::eval(Programm* prog){//"00 10 01 02 03 13 04 14 05"
	if(operation->text == "00"){
		
		QString s = memory->text;
		if(reg2->text == "0")
			if(!prog->variabls.contains(memory->text)) throw new MyException("undefined variable " + memory->text);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(memory->text, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " + 
																  memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->variabls[reg1->text].value = prog->variabls[s].value;
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "10"){
		
		QString s = memory->text;
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined Registr " + reg1->text);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(memory->text, prog->variabls[reg2->text].value);
		}
		
		if(contains(s, prog)) throw new MyException("bad ADRESS for " + s + ". Adress is a part of Programm");
		
		prog->variabls[s].value = prog->variabls[reg1->text].value;
		
		if(prog->variabls[s].name == "")	
			prog->variabls[s].name = "Unknown Variable of " + s;
		
	}else if(operation->text == "01"){
		
		QString s = memory->text;
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value + prog->variabls[s].value;
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "02"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value - prog->variabls[s].value;
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "03"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value * prog->variabls[s].value;
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
		
	}else if(operation->text == "13"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift "
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->variabls[reg1->text].value = qAbs(prog->variabls[reg1->text].value) * qAbs(prog->variabls[s].value);
		prog->variabls[reg1->text].name = "Registr " + reg1->text;
	}else if(operation->text == "04"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
			
			if(prog->variabls[s].value == 0) throw new MyException("division by zero " + prog->variabls[s].name + " = 0");
		}
		
		prog->variabls[reg1->text].value = prog->variabls[reg1->text].value / prog->variabls[s].value;
		int g = reg1->text.toInt(NULL, 16);
		++g;
		g %= (0x10);
		QString str = htos(g);
		
		prog->variabls[str.toUpper()].value = prog->variabls[reg1->text].value % prog->variabls[s].value;	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		prog->variabls[str.toUpper()].name = "Registr " + str.toUpper();	
		
	}else if(operation->text == "14"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
			
			if(prog->variabls[s].value == 0) throw new MyException("division by zero " + prog->variabls[s].name + " = 0");
		}
		
		prog->variabls[reg1->text].value = qAbs(prog->variabls[reg1->text].value) / qAbs(prog->variabls[s].value);
		int g = reg1->text.toInt(NULL, 16);
		++g;
		g %= (0x10);
		QString str = htos(g);
		
		prog->variabls[str.toUpper()].value = qAbs(prog->variabls[reg1->text].value) % qAbs(prog->variabls[s].value);	
		prog->variabls[reg1->text].name = "Registr " + reg1->text;	
		prog->variabls[str.toUpper()].name = "Registr " + str.toUpper();	
		
	}else if(operation->text == "05"){
		QString s = memory->text;
		
		if(!prog->variabls.contains(reg1->text)) throw new MyException("undefined variable Registr " + reg1->text);
		
		if(reg2->text == "0")
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s);
		
		if(reg2->text != "0"){
			if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
			
			s = toAdress(s, prog->variabls[reg2->text].value);
			
			if(!prog->variabls.contains(s)) throw new MyException("undefined variable " + s + " after sift " 
																  + memory->text + " + " + itos(prog->variabls[reg2->text].value));
		}
		
		prog->RK[0] = prog->variabls[reg1->text].value;prog->RK[0] = prog->variabls[s].value;
	}
	
	++prog->CA;
}

TransitionOperation::TransitionOperation(Token *op, Token *r1, Token *r2, Token *mem, QTextBlock _block){
	operation = op; reg1 = r1; reg2 = r2; memory = mem;
	typeOfOperation = TAOPERATION;
	block = _block;
	
	if(valid(operation) && valid(reg1) && valid(reg2) && valid(memory) && memory->type == ADRESS) return;
	
	else throw new MyException("Error '" + toString(operation) + " " + toString(reg1) + " " +
							   toString(reg2) + " " + toString(mem) + "' when must be '" + toString(operation) + " ANY(0) REGISTR ADRESS'");
}

QString TransitionOperation::operationToString(){
	QVector<QString> nop = {"81", "82", "83", "84", "85", "86", "93", "94", "95", "96"};
	QString op[] = {"=", "!=", "<", ">=", ">", "<=", "abs(<)", "abs(>=)", "abs(>)", "abs(<=)"};
	if(operation->text == "80"){
		return "go to ";
	}
	
	int i = nop.indexOf(operation->text);
	if(i != -1)
		return "if " + op[i] + " go to ";
	else
		return "Error with undefine operation" + operation->text + itos(nop.size());
}

QString TransitionOperation::getAdress(){
	return memory->text;
}

QString TransitionOperation::getShift(){
	return Highlighter::toString(reg1->type) + reg2->text;
}


void TransitionOperation::eval(Programm* prog){//"80 81 82 83 84 85 86 93 94 95 96"
	QString s = memory->text;
	QVector<std::pair<QString, Operation*>>::iterator next;
	if(reg2->text == "0"){
		next = find(s, prog);
	
		if(next == prog->steps.end()) throw new MyException("adress " + s + " not exist");
	}else{
		if(!prog->variabls.contains(reg2->text)) throw new MyException("undefined variable Registr " + reg2->text);
		
		s = toAdress(memory->text, prog->variabls[reg2->text].value);
		next = find(s, prog);
		
		if(next == prog->steps.end()) throw new MyException("adress " + s + " not exist" + " after sift " 
															+ memory->text + " + " + itos(prog->variabls[reg2->text].value));
	}
	if(operation->text == "80"){
		prog->CA = next;
		return;
	}else if(operation->text == "81"){
		if(prog->RK[0] == prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "82"){
		if(prog->RK[0] != prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "83"){
		if(prog->RK[0] < prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "84"){
		if(prog->RK[0] >= prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "85"){
		if(prog->RK[0] > prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "86"){
		if(prog->RK[0] <= prog->RK[1]){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "93"){
		if(qAbs(prog->RK[0]) < qAbs(prog->RK[1])){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "94"){
		if(qAbs(prog->RK[0]) >= qAbs(prog->RK[1])){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "95"){
		if(qAbs(prog->RK[0]) > qAbs(prog->RK[1])){
			prog->CA = next;
			return;
		}
	}else if(operation->text == "96"){
		if(qAbs(prog->RK[0]) <= qAbs(prog->RK[1])){
			prog->CA = next;
			return;
		}
	}
	
	++prog->CA;
}

InitializationOperation::InitializationOperation(Token *adress, Token *value, Token *name, QTextBlock _block){
	operation = adress;this->value = value;this->name = name;
	typeOfOperation = INOPERATION;
	block = _block;
	if(this->name == NULL){
		if(valid(this->value) && this->value->type == WORD){
			qSwap(this->value, this->name);
		}else{
			this->name = new Token();
			this->name->type = WORD;
			this->name->text = "Unknown Variable of " + adress->text;
		}
	}
	if(this->value == NULL){
		this->value = new Token();
		this->value->text = "0"; 
		this->value->type = NUMBER; 
	}
	
	if(this->value->type == WORD) qSwap(this->value, this->name);
	
	if(this->value->type == NUMBER && this->name->type == WORD) return;
	else throw new MyException("Error when initialization " + toString(operation) + 
							   " need value(0x0) and name for variable, now you have " + toString(this->value) + " " + toString(this->name));
}


QString InitializationOperation::operationToString(){
	QString str;
	str += operation->text + " : " + name->text + " := " + itos(value->text.toInt(NULL, 16)) + ";";
	return str;
}

QString InitializationOperation::getOperation(){
	return operation->text;
}

QString InitializationOperation::getName(){
	return name->text;
}

void InitializationOperation::eval(Programm* prog){
	for(auto i = prog->variabls.begin(); i != prog->variabls.end(); ++i){
		if(i.key().size() != 1 && toAdress(i.key(), 1) == operation->text) 
			throw new MyException("bad ADRESS for " + operation->text + ". Adress is a part of Programm");
	}
	
	prog->variabls[operation->text].name = name->text;
	prog->variabls[operation->text].value = value->text.toInt(NULL, 16) % mod;
	++prog->CA;
	
}
