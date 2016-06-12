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

#include "highlighter.h"
#include <QMap>
#include <QException>
#include "myexception.h"
#include <QVector>

#define itos(x) QString::number(x)
#define htos(x) QString::number(x, 16)

Highlighter::Highlighter(QTextDocument *_parent)
	: QSyntaxHighlighter(_parent){
	
	Tokens = *(new QMap<int, QMap<int, Token>>());
	Blocks = *(new QMap<int, QTextBlock>());
	currentLine = 0;
	current = Tokens[currentLine].begin();
	
	programm = new Programm();
	
    HighlightingRule rule;

	connect(document(), SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(clearTokens()));
	
	wordFormat.setFontWeight(QFont::Bold);
	wordFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\b[A-Za-z1-9]([A-Za-z0-9])+\\b");
	rule.format = wordFormat;
	rule.type = WORD;
    highlightingRules.push_back(rule);
	
	operationFormat.setFontWeight(QFont::Bold);
	operationFormat.setForeground(Qt::darkYellow);
	rule.pattern = QRegExp("\\b\\d{2}\\b");
	rule.format = operationFormat;
	rule.type = TokenType::OPERATION;
	highlightingRules.push_back(rule);

	registrFormat.setFontWeight(QFont::Bold);
	registrFormat.setForeground(Qt::green);
	rule.pattern = QRegExp("\\b(\\d|[A-F])\\b");
	rule.format = registrFormat;
	rule.type = TokenType::REGISTR;
	highlightingRules.push_back(rule);

	multiLineCommentFormat.setForeground(Qt::gray);

    commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
	commentOneExpression = QRegExp("//[^\n]*");
	
	adressFormat.setForeground(Qt::darkBlue);
	adressFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegExp("\\b(\\d|[A-F]){4}\\b");
	rule.format = adressFormat;
	rule.type = TokenType::ADRESS;
	highlightingRules.push_back(rule);
	
	numberFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("\\-?0x((\\d|[A-F])+)\\b");
	rule.format = numberFormat;
	rule.type = NUMBER;
	highlightingRules.push_back(rule);

	alertFormat.setFontWeight(QFont::Bold);
	alertFormat.setForeground(Qt::red);
}

void Highlighter::setParent(MainWindow * _parent){
	parent = _parent;
	programm->setParent(parent);
}

QString Highlighter::toString(TokenType value){
	switch (value) {
	case WORD:
		return QString("Word");
		break;
	case NUMBER:
		return QString("Number");
		break;
	case ADRESS:
		return QString("Adress");
		break;
	case OPERATION:
		return QString("Operation");
		break;
	case REGISTR:
		return QString("R");
		break;
	default:
		return "";
		break;
	}
}

QString Highlighter::toString(){
	QString str = "";
	foreach (auto m, Tokens) {
		foreach (Token t, m) {
			str += Highlighter::toString(t.type) + " " + t.text + " ";
		}
		str += '\n';
	}
	return str;
}

Token* Highlighter::next(){
	try{
		if(current == Tokens[currentLine].end()) return NULL;
		Token* a = new Token(current.value());
		++current;
		return a;
	} catch (...){
		return NULL;
	}
}

void Highlighter::setBegin(){
	current = Tokens[currentLine].begin();
}

int Highlighter::nextLine(){
	++currentLine;
	if(currentLine >= Tokens.size())
		currentLine = -1;
	return currentLine;
}

void Highlighter::setBeginLine(){
	currentLine = 0;
}

Token *Highlighter::token(int x){
	try{
		if(current == Tokens[currentLine].constEnd()) return NULL;
		Token* a = new Token((current + x).value());
		return a;
	} catch (...){
		return NULL;
	}
}

void Highlighter::highlightBlock(const QString &text){

	int lineIndex = this->currentBlock().blockNumber();
	Blocks[lineIndex] = currentBlock();

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);

	while (startIndex >= 0) {
		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
							+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}

	if (previousBlockState() != 1)
		startIndex = commentOneExpression.indexIn(text);
	if(startIndex != -1){
		int length = text.size() - startIndex;
		setFormat(startIndex, length, multiLineCommentFormat);
	}

	setCurrentBlockState(0);

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			if(format(index) != multiLineCommentFormat){
				Token temp;
				temp.type = rule.type;
				temp.text = text.mid(index, length);
				Tokens[lineIndex].insert(index, temp);
				setFormat(index, length, rule.format);
			}
            index = expression.indexIn(text, index + length);
        }
	}
}

void Highlighter::clearTokens(){
	QString ErrorMessgae = "";

	parent->field->extraSelections->clear();
	Tokens.clear();
	parent->logger->clear();
	programm->clearSteps();

	this->rehighlight();
	
	
	QTextEdit::ExtraSelection selection;

	QColor lineColor = QColor(Qt::red).lighter(180);

	selection.format.setBackground(lineColor);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	
	setBeginLine();
	do{
		setBegin();
		Token* c;
		QString currentAdress = "";
		while((c = next()) != NULL){
			try{
				switch(c->type){
					case ADRESS :{
						if(c->text < "0000" || c->text > "FFFF") throw new MyException("Adress must be from 0000 to FFFF");
						auto t = token(0);
						currentAdress = c->text;
						if(t != NULL){
							if(t->type == TokenType::OPERATION){
							}else if(t->type == ADRESS){
								programm->addStep(std::make_pair(currentAdress, new InitializationOperation(next(), next(), next(), Blocks[currentLine])));
							}else throw new MyException("Need operation for adress " + c->text);
						}else throw new MyException("Need more arguments for " + c->text);
						break;
					}
					case TokenType::OPERATION :{
						if(currentAdress == ""){
							
							throw new MyException("Need adress for operation " + c->text);
							
						}else if(QString("99 20 21 22 23 33 24 34 25").contains(c->text)){
							
							programm->addStep(std::make_pair(currentAdress, new RegistrRegistrOperation(c, next(), next(), Blocks[currentLine])));
							
						}else if(QString("00 10 01 02 03 13 04 14 05").contains(c->text)){
							
							programm->addStep(std::make_pair(currentAdress, new RegistrMemoryOperation(c, next(), next(), next(), Blocks[currentLine])));
							
						}else if(QString("80 81 82 83 84 85 86 93 94 95 96").contains(c->text)){
							
							programm->addStep(std::make_pair(currentAdress, new TransitionOperation(c, next(), next(), next(), Blocks[currentLine])));
							
						}else{
							throw new MyException("Unknown operation");
						}
						
						currentAdress = "";
						break;
					}
					default:
						throw new MyException("Unknown error " + c->text);
						break;
				}
				
				if(programm->size() > 1){
					auto prev = programm->getElement(programm->size());
					--prev;
					
					int cur = prev->first.toInt(NULL, 16);
					--prev;
					
					int pre = prev->first.toInt(NULL, 16);
					
					if(prev->second->typeOfOperation == RROPERATION){
						if((pre + 1) != cur) throw new MyException("Error procedure addresses the previous operation was a register-to-register or initialization");
					}else{
						if((pre + 2) != cur) throw new MyException("Error procedure addresses the previous operation was a register-to-memory or transition or initialization");									
					}
				}
				
			}catch (const MyException*& e){
				currentAdress = "";
				ErrorMessgae += "Line " + itos(currentLine + 1) + " " + e->whatHappend() + '\n';
				selection.cursor = QTextCursor(Blocks[currentLine]);
				parent->field->extraSelections->append(selection);
			}catch(...){
				currentAdress = "";
				ErrorMessgae += "Line " + itos(currentLine + 1) + " System Error" + '\n';
			}
		}
	}while(nextLine() != -1);
	
	parent->pseudoEditor->clear();
	parent->pseudoEditor->appendPlainText(programm->toPseudoCode());
	
	parent->logger->appendPlainText(ErrorMessgae);

	emit parent->field->cursorPositionChanged();
}
