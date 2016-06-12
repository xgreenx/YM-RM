#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#define itos(x) QString::number(x)
#define htos(x) QString::number(x, 16)
#define mod 0xFFFF

enum TokenType{
	ADRESS,
	NUMBER,
	WORD,
	REGISTR,
	OPERATION,
	
	RROPERATION,
	RMOPERATION,
	TAOPERATION,
	INOPERATION
};

#endif // DATA_TYPE_H
