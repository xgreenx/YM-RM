#ifndef OPERATION_H
#define OPERATION_H

#include "highlighter.h"
#include "data_type.h"
#include <QVector>

class Token;
class Programm;

class Operation{
public:
	virtual void eval(Programm*);
	virtual QString operationToString();
	TokenType typeOfOperation = TokenType::OPERATION;
	
	QTextBlock block;
protected:
	Token* operation;
	QString toString(Token*);
	bool valid(Token*);
	QString toAdress(QString, int x);
	bool contains(const QString &, Programm *prog);
	QVector<std::pair<QString, Operation*>>::iterator find(const QString&, Programm*);
};

class RegistrRegistrOperation: public Operation{
	public:
		RegistrRegistrOperation(Token* op, Token* r1, Token* r2, QTextBlock _block);
		QString operationToString();
	protected:
		Token* reg1, *reg2;
		void eval(Programm* prog);
};

class RegistrMemoryOperation: public Operation{
	public:
		RegistrMemoryOperation(Token* op, Token* r1, Token* r2, Token* mem, QTextBlock _block);
		QString operationToString();
	protected:
		Token* reg1, *reg2, *memory;
		void eval(Programm* prog);
};

class TransitionOperation: public Operation{
	public:
		TransitionOperation(Token* op, Token* r1, Token* r2, Token* mem, QTextBlock _block);
		QString operationToString();
		QString getAdress();
		QString getShift();
	protected:
		Token* reg1, *reg2, *memory;
		void eval(Programm* prog);
};

class InitializationOperation: public Operation{
	public:
		InitializationOperation(Token* adress, Token* value, Token* name, QTextBlock _block);
		QString operationToString();
		QString getOperation();
		QString getName();
	protected:
		Token* value, *name;
		void eval(Programm* prog);
};

#endif // OPERATION_H
