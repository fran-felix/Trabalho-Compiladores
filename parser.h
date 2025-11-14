#ifndef __Parser_h
#define __Parser_h


#include "scanner.h"


class Parser
{
private:
	Scanner* scanner;
	Token* lToken{nullptr};
	SymbolTable* globalST;
	SymbolTable* currentST;
	STEntry* entry;

	void advance();
	void match(int, string);
	void initSymbolTable();

public:
	Parser(string);
	~Parser();

	void run();
	void program();

	void classList();
	void classDecl();
	void classBody();
	
	void varDeclListOpt();
	void varDeclList();
	void varDecl();
	void varDeclOpt();
	void type();

	void constructDeclListOpt();
	void constructDeclList();
	void constructDecl();

	void methodDeclListOpt();
	void methodDeclList();
	void methodDecl();
	void methodBody();

	void paramListOpt();
	void paramList();
	void param();

	void statementListOpt();
	void statementList();
	void statement();

	void atribStat();
	void printStat();
	void readStat();
	void returnStat();
	void superStat();
	void ifStat();
	void forStat();

	void atribStatOpt();
	void expressionOpt();

	void lValue();
	void lValueComp();
	void expression();
	void allocExpression();
	void numExpression();
	void term();
	void unaryExpression();
	void factor();

	void argListOpt();
	void argList();

	void error(string);
};


#endif // __Parser_h