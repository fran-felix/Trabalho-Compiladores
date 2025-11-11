#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

Parser::~Parser()
{
	delete scanner;
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t, string str)
{
	//if (lToken->name == t || lToken->attribute == t)
	if (lToken->tokenName() == t)
		advance();
	else
		error(str);
}

void
Parser::run()
{
	advance();

	program();

	cout << "Compilation successful!\n";
}

// Tabela de Simbolos necessaria
void
Parser::program()
{
	//TODO
	if (/*lToken->name == CLASS*/ lToken->lexeme == "class")
		classList();
	else if (lToken->tokenName() != END_OF_FILE)
		error("Keyword 'class' expected");
}


// BEGIN: Class section
// Tabela de Simbolos necessaria
void
Parser::classList()
{
	classDecl();
	if (lToken->lexeme == "class")
	{
		classList();
	}
	else if (lToken->tokenName() == END_OF_FILE)
		;
	else
		error("Keyword 'class' expected");
}

// Tabela de Simbolos necessaria
void
Parser::classDecl()
{
	if (lToken->lexeme == "class")
	{
		advance();
		match(ID, "Class name expected after 'class'");
		if (lToken->lexeme == "extends")
		{
			advance();
			match(ID, "Parent class name expected after 'extends'");
			classBody();
		}
		classBody();
	}
}

void
Parser::classBody()
{
	match(LR, "'{' expected after class declaration");
	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();
	match(RR, "'}' expected closing class body");
}
// END: Class section


// BEGIN: Variable section
// Tabela de Simbolos necessaria
void
Parser::varDeclListOpt()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		varDeclList();
}

// Tabela de Simbolos necessaria
void
Parser::varDeclList()
{
	varDecl();
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		varDeclList();
}

// Tabela de Simbolos necessaria
void
Parser::varDecl()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "Incorrect type declaration, ']' expected after '['");
		}
		match(ID, "Variable declaration expected after type");
		varDeclOpt();
		match(SC, "syntax error: missing ';'");
	}
}

// Tabela de Simbolos necessaria
void
Parser::varDeclOpt()
{
	if (lToken->tokenName() == C)
	{
		advance();
		match(ID, "Variable list expected");
		varDeclOpt();
	}
}

// Tabela de Simbolos necessaria
void
Parser::type()
{
	if (lToken->lexeme == "int")
		advance();
	else if (lToken->lexeme == "string")
		advance();
	/*else if (lToken->name == ID)
		advance();*/
	else
		error("Unknown type declaration");
}
// END: Variable section


// BEGIN: Construct section
void
Parser::constructDeclListOpt()
{
	if (lToken->lexeme == "constructor")
		constructDeclList();
}

void
Parser::constructDeclList()
{
	constructDecl();
	if (lToken->lexeme == "constructor")
		constructDeclList();
}

void
Parser::constructDecl()
{
	if (lToken->lexeme == "constructor")
	{
		advance();
		methodBody();
	}
	else
		error("Malformed Constructor declaration");
}
// END: Construct section


// BEGIN: Method section
// Tabela de simbolos necessaria
void
Parser::methodDeclListOpt()
{ 
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		methodDeclList();
}

// Tabela de simbolos necessaria
void
Parser::methodDeclList()
{
	methodDecl();
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		methodDeclList();
}

// Tabela de Simbolos necessaria
void
Parser::methodDecl()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "Incorrect type declaration, ] expected after '['");
		}
		match(ID, "Variable declaration expected after type");
		methodBody();
	}
}

void
Parser::methodBody()
{
	match(LP, "'(' expected after method signature");
	paramListOpt();
	match(RP, "')' expected after parameter list");

	match(LR, "'{' expected after ')'");
	statementListOpt();
	match(RR, "'}' expected closing method body");
}
// END: Method section


// BEGIN: Parameter section
// Tabela de Simbolos necessaria
void
Parser::paramListOpt()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		paramList();
}

void
Parser::paramList()
{
	param();
	if (lToken->tokenName() == C)
	{
		advance();
		param();
		paramList();
	}
}

// Tabela de Simbolos necessaria
void
Parser::param()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "incorrect type declaration, ']' expected after '['");
		}
		match(ID, "parameter name expected");
	}
}

// END: Paramenter section


// BEGIN: Statement section
// Tabela de Simbolos necessaria
void
Parser::statementListOpt()
{
	if (
		(lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		|| lToken->lexeme == "print"
		|| lToken->lexeme == "read"
		|| lToken->lexeme == "return"
		|| lToken->lexeme == "super"
		|| lToken->lexeme == "if"
		|| lToken->lexeme == "for"
		|| lToken->lexeme == "break"
		|| lToken->tokenName() == SC
		)
		statementList();
}

void
Parser::statementList()
{
	statement();
	if (
		(lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		|| lToken->lexeme == "print"
		|| lToken->lexeme == "read"
		|| lToken->lexeme == "return"
		|| lToken->lexeme == "super"
		|| lToken->lexeme == "if"
		|| lToken->lexeme == "for"
		|| lToken->lexeme == "break"
		|| lToken->tokenName() == SC
		)
		statementList();
}

// Tabela de Simbolos necessaria
void
Parser::statement()
{
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
		varDeclList();
	else if (lToken->name == ID)
	{
		atribStat();
		match(SC, "';' expected at the end of atribution statement");
	}
	else if (lToken->name == ID && lToken->lexeme == "print")
	{
		printStat();
		match(SC, "';' expected at the end of print statement");
	}
	else if (lToken->name == ID && lToken->lexeme == "read")
	{
		readStat();
		match(SC, "';' expected at the end of read statement");
	}
	else if (lToken->name == ID && lToken->lexeme == "return")
	{
		returnStat();
		match(SC, "';' expected at the end of return statement");
	}
	else if (lToken->name == ID && lToken->lexeme == "super")
	{
		superStat();
		match(SC, "';' expected at the end of super statement");
	}
	else if (lToken->name == ID && lToken->lexeme == "if")
		ifStat();
	else if (lToken->name == ID && lToken->lexeme == "for")
		forStat();
	else if (lToken->name == ID && lToken->lexeme == "break")
	{
		advance();
		match(SC, "';' expected at the end of break statement");
	}
	else
		match(SC, "';' expected");
}

void
Parser::atribStat()
{ }

void
Parser::printStat()
{ }

void
Parser::readStat()
{ }

void
Parser::returnStat()
{ }

void
Parser::superStat()
{ }

void
Parser::ifStat()
{ }

void
Parser::forStat()
{ }

// END: Statement section

void
Parser::error(string str)
{
	cout << "Line " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
