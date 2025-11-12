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

// Tabela de Simbolos necessaria
void
Parser::atribStat()
{ 
	lValue();
	match(AS, "'=' operator expected");
	if (lToken->tokenName() == AN || lToken->tokenName() == SN)
		expression();
	else if (lToken->name == ID && lToken->lexeme == "new")
		allocExpression();
	else
		error("attribuition statement malformed");
}

// Tabela de Simbolos necessaria
void
Parser::printStat()
{
	if (lToken->name == ID && lToken->lexeme == "print")
	{
		advance();
		expression();
	}
}

// Tabela de Simbolos necessaria
void
Parser::readStat()
{ 
	if (lToken->name == ID && lToken->lexeme == "read")
	{
		advance();
		lValue();
	}
}

// Tabela de Simbolos necessaria
void
Parser::returnStat()
{ 
	if (lToken->name == ID && lToken->lexeme == "return")
	{
		advance();
		expression();
	}
}

// Tabela de Simbolos necessaria
void
Parser::superStat()
{ 
	if (lToken->name == ID && lToken->lexeme == "super")
	{
		advance();
		match(LP, "'(' expected after 'super'");
		argListOpt();
		match(RP, "')' expected");
	}
}

// Tabela de Simbolos necessaria
void
Parser::ifStat()
{ 
	if (lToken->name == ID && lToken->lexeme == "if")
	{
		advance();
		match(LP, "'(' expected after 'if' statement");
		expression();
		match(RP, "')' expected closing 'if' statement");
		match(LP, "'{' expected after 'if' block");
		statementList();
		match(RP, "'}' expected closing 'if' block");
		if (lToken->name == ID && lToken->lexeme == "else")
		{
			match(LP, "'{' expected after 'if' block");
			statementList();
			match(RP, "'}' expected closing 'if' block");
		}
	}
}

void
Parser::forStat()
{ 
	if (lToken->name == ID && lToken->lexeme == "for")
	{
		advance();
		match(LP, "'(' expected after 'for' statement");
		atribStatOpt();
		match(SC, "';' expected after loop variable");
		expressionOpt();
		match(SC, "';' expected after loop expression");
		atribStatOpt();
		match(RP, "')' expected closing 'for' statement");
		match(LP, "'{' expected after 'if' block");
		statementList();
		match(RP, "'}' expected closing 'if' block");
	}
}
// END: Statement section


// BEGIN: Last section

void
Parser::atribStatOpt()
{
	if (lToken->name == ID)
		atribStat();
}

void
Parser::expressionOpt()
{
	if (lToken->tokenName() == AN || lToken->tokenName() == SN)
		expression();
}

void
Parser::lValue()
{
	if (lToken->name == ID)
	{
		advance();
		lValueComp();
	}
	else
		error("lvalue expected");
}

void
Parser::lValueComp()
{
	if (lToken->tokenName() == P)
	{
		advance();
		match(ID, "identifier name expected after '.'");
		if (lToken->tokenName() == LB)
		{
			advance();
			expression();
			match(RB, "']' expected");
		}
		else if (lToken->tokenName() == LP)
		{
			advance();
			argListOpt();
			match(RP, "')' expected");
		}
	else if (lToken->tokenName() == LB)
	{
		advance();
		expression();
		match(RB, "']' expected");
	}
		lValueComp();
	}
}

void
Parser::expression()
{
	numExpression();
	if (lToken->name == OP)
	{
		match(OP, "relational operator expected");
		numExpression();
	}
}

// Tabela de Simbolos necessaria
void
Parser::allocExpression()
{
	if (lToken->name == ID && lToken->lexeme == "new")
	{
		advance();
		match(ID, "'new' statement incomplete");
		match(LP, "'(' expected after 'new' statement");
		argListOpt();
		match(RP, "')' expected closing 'new' statement");
	}
	else if (lToken->name == ID && (lToken->lexeme == "int" || lToken->lexeme == "string"))
	{
		type();
		match(LB, "'[' expected");
		expression();
		match(RB, "']' expected");
	}
}

void
Parser::numExpression()
{
	term();
	if (lToken->tokenName() == AN)
	{
		advance();
		term();
	}
	else if (lToken->tokenName() == SN)
	{
		advance();
		term();
	}
}

void
Parser::term()
{
	unaryExpression();
	if (lToken->tokenName() == MN)
	{
		advance();
		unaryExpression();
	}
	else if (lToken->tokenName() == DN)
	{
		advance();
		unaryExpression();
	}
	else if (lToken->tokenName() == RN)
	{
		advance();
		unaryExpression();
	}
}

void
Parser::unaryExpression()
{
	if (lToken->tokenName() == AN)
	{
		advance();
		factor();
	}
	else if (lToken->tokenName() == SN)
	{
		advance();
		factor();
	}
}

void
Parser::factor()
{
	if (lToken->name == INT)
		advance();
	else if (lToken->name == STRING)
		advance();
	else if (lToken->tokenName() == LP)
	{
		advance();
		expression();
		match(RP, "')' expected (factor)");
	}
	else
		lValue();
}

void
Parser::argListOpt()
{
	expression();
	if (lToken->tokenName() == C)
	{
		advance();
		expression();
		argList();
	}
}

// END: Last section

void
Parser::error(string str)
{
	cout << "Line " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
