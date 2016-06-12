/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include "mainwindow.h"
#include "operation.h"
#include "data_type.h"
#include "programm.h"
#include <QVector>

class QTextDocument;
class Operation;
class RegistrRegistrOperation;
class Programm;

struct HighlightingRule{
    QRegExp pattern;
    QTextCharFormat format;
	TokenType type;
};

class Token{
	public:
		QString text;
		TokenType type;
};

class Variable{
	public:
		QString name;
		int value;
		QString toString(){
			return name + " = " + itos(value);
		};
};

class MainWindow;

class Highlighter : public QSyntaxHighlighter{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);
	QVector<HighlightingRule> highlightingRules;
	void setParent(MainWindow*);
	static QString toString(TokenType);
	QString toString();
	
	Programm* programm;

protected:
	void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
	MainWindow* parent;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;
	QRegExp commentOneExpression;

	QTextCharFormat wordFormat;
	QTextCharFormat registrFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
	QTextCharFormat operationFormat;
	QTextCharFormat adressFormat;
	QTextCharFormat alertFormat;
	QTextCharFormat numberFormat;

	QMap<int, Token>::iterator current;
	int currentLine;
	QMap<int, QMap<int, Token>> Tokens;
	QMap<int, QTextBlock> Blocks;
	
	Token *next();
	void setBegin();
	int nextLine();
	void setBeginLine();
	Token* token(int);

private slots:
	void clearTokens();
};
#endif // HIGHLIGHTER_H
