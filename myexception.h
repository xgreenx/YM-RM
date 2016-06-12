#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <QException>

class MyException : QException{
	public:
		MyException(QString);
		QString whatHappend() const;
	private:
		QString err;
};

#endif // MYEXCEPTION_H