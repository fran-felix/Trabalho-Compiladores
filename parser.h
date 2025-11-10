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
	
	void methodDeclListOpt();
	
	void error(string);
};