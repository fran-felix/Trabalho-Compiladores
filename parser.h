#include "scanner.h"

class Parser
{
private:
	Scanner* scanner;
	Token* lToken{nullptr};

	void advance();
	void match(int, string);

public:
	Parser(string);
	~Parser();

	void run();
	void program();

	void classList();
	//Continuar....
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

	void error(string);
};