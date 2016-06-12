#include "myexception.h"

MyException::MyException(QString s){
	err = s;
}

QString MyException::whatHappend() const{
	return err;
}