#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
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

//Continuar....

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
	match(LR, "{ expected after class declaration");
	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();
	match(RR, "} expected closing class body");
}

// Tabela de Simbolos necessaria
void
Parser::varDeclListOpt()
{
	if (lToken->name == ID)
		varDeclList();
}

// Tabela de Simbolos necessaria
void
Parser::varDeclList()
{
	varDecl();
	if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
	{
		varDeclList();
	}
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
			match(RB, "Incorrect type declaration, ] expected after '['");
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

void
Parser::constructDeclListOpt()
{ }

void
Parser::methodDeclListOpt()
{ }

void
Parser::error(string str)
{
	cout << "Line " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
