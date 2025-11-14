#include "parser.h"

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSymbolTable();

	scanner = new Scanner(input, globalST);
}

Parser::~Parser()
{
	delete scanner;
	delete globalST;
	delete entry;
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
	if (lToken->tokenName() == t || lToken->name == t)
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
	if (lToken->name == CLASS)
		classList();
}


// BEGIN: Class section
void
Parser::classList()
{
	classDecl();
	if (lToken->name == CLASS)
	{
		classList();
	}
	else if (lToken->name == END_OF_FILE)
		;
	else
		error("Keyword 'class' expected");
}

void
Parser::classDecl()
{
	if (lToken->name == CLASS)
	{
		advance();

		entry = new STEntry(lToken);
		entry->token->attribute = TYPE;
		if(!currentST->add(entry))
			error("attempted class type redefinition");
		match(TYPE, "Class name expected after 'class'");

		if (lToken->name == EXTENDS)
		{
			advance();

			if (!currentST->get(lToken->lexeme))
				error("super class does not exist");

			match(TYPE, "Parent class name expected after 'extends'");
			classBody();
		}
		classBody();
	}
}

void
Parser::classBody()
{
	currentST = new SymbolTable(currentST);
	match(LR, "'{' expected after class declaration");
	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();
	match(RR, "'}' expected closing class body");
	currentST = currentST->getParent();
}
// END: Class section


// BEGIN: Variable section
void
Parser::varDeclListOpt()
{
	if(lToken->name == ID)
		lToken->attribute = TYPE;
	if (lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE)
		varDeclList();
}

void
Parser::varDeclList()
{
	varDecl();
	if (lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE)
		varDeclList();
}

void
Parser::varDecl()
{
	if (lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE)
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "Incorrect type declaration, ']' expected after '['");
		}

		entry = new STEntry(lToken);
		if (!currentST->add(entry))
		{
			if (entry->token->attribute == TYPE)
				error("type name cannot be used as variable name");
			else
				error("variable already defined");
		}
		match(ID, "variable declaration expected after type");

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
		entry = new STEntry(lToken);
		if (!currentST->add(entry))
		{
			if (entry->token->attribute == TYPE)
				error("type name cannot be used as variable name");
			else
				error("variable already defined");
		}
		match(ID, "Variable list expected");
		varDeclOpt();
	}
}

void
Parser::type()
{
	if (lToken->name == INT_TYPE)
		advance();
	else if (lToken->name == STRING_TYPE)
		advance();
	else if (lToken->name == ID) // ID que eh um tipo definido pelo usuario
	{
		lToken->attribute = TYPE;
		if (!currentST->get(lToken->lexeme))
			error("type does not exist");
		advance();
	}
	else
		error("Unknown type used");
}
// END: Variable section

// BEGIN: Construct section
void
Parser::constructDeclListOpt()
{
	if (lToken->name == CONSTRUCTOR)
		constructDeclList();
}

void
Parser::constructDeclList()
{
	constructDecl();
	if (lToken->name == CONSTRUCTOR)
		constructDeclList();
}

void
Parser::constructDecl()
{
	if (lToken->name == CONSTRUCTOR)
	{
		advance();
		methodBody();
	}
	else
		error("Malformed Constructor declaration");
}
// END: Construct section


// BEGIN: Method section
void
Parser::methodDeclListOpt()
{ 
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
		methodDeclList();
}

void
Parser::methodDeclList()
{
	methodDecl();
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
		methodDeclList();
}

// Tabela de Simbolos necessaria
void
Parser::methodDecl()
{
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "incorrect type declaration, ] expected after '['");
		}
		
		entry = new STEntry(lToken);
		if (!currentST->add(entry))
		{
			if (entry->token->attribute == TYPE)
				error("type name cannot be used as method name");
			else
				error("method already defined");
		}
		match(ID, "method name expected after type");

		methodBody();
	}
}

void
Parser::methodBody()
{
	currentST = new SymbolTable(currentST);
	match(LP, "'(' expected after method signature");
	paramListOpt();
	match(RP, "')' expected after parameter list");

	match(LR, "'{' expected after ')'");
	statementListOpt();
	match(RR, "'}' expected closing method body");
	currentST = currentST->getParent();
}
// END: Method section


// BEGIN: Parameter section
void
Parser::paramListOpt()
{
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
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

void
Parser::param()
{
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
	{
		type();
		if (lToken->tokenName() == LB)
		{
			advance();
			match(RB, "incorrect type declaration, ']' expected after '['");
		}

		entry = new STEntry(lToken);
		if (!currentST->add(entry))
		{
			if (entry->token->attribute == TYPE)
				error("type name cannot be used as variable name");
			else
				error("variable in parameters already defined");
		}
		match(ID, "parameter name expected");
	}
}

// END: Paramenter section


// BEGIN: Statement section
void
Parser::statementListOpt()
{
	if (
		lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE
		|| lToken->name == ID
		|| lToken->name == PRINT
		|| lToken->name == READ
		|| lToken->name == RETURN
		|| lToken->name == SUPER
		|| lToken->name == IF
		|| lToken->name == FOR
		|| lToken->name == BREAK
		|| lToken->tokenName() == SC
		)
			statementList();
}

void
Parser::statementList()
{
	statement();
	if (
		lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE
		|| lToken->name == ID
		|| lToken->name == PRINT
		|| lToken->name == READ
		|| lToken->name == RETURN
		|| lToken->name == SUPER
		|| lToken->name == IF
		|| lToken->name == FOR
		|| lToken->name == BREAK
		|| lToken->tokenName() == SC
		)
		statementList();
}

void
Parser::statement()
{
	if (lToken->tokenName() == TYPE || lToken->name == INT_TYPE || lToken->name == STRING_TYPE)
		varDeclList();
	else if (lToken->name == ID)
	{
		atribStat();
		match(SC, "';' expected at the end of atribution statement");
	}
	else if (lToken->name == PRINT)
	{
		printStat();
		match(SC, "';' expected at the end of print statement");
	}
	else if (lToken->name == READ)
	{
		readStat();
		match(SC, "';' expected at the end of read statement");
	}
	else if (lToken->name == RETURN)
	{
		returnStat();
		match(SC, "';' expected at the end of return statement");
	}
	else if (lToken->name == SUPER)
	{
		superStat();
		match(SC, "';' expected at the end of super statement");
	}
	else if (lToken->name == IF)
		ifStat();
	else if (lToken->name == FOR)
		forStat();
	else if (lToken->name == BREAK)
	{
		advance();
		match(SC, "';' expected at the end of break statement");
	}
	else
		match(SC, "';' expected");
}

void
Parser::atribStat()
{ 
	lValue();
	match(AS, "'=' operator expected");
	if (lToken->tokenName() == AN
		|| lToken->tokenName() == SN
		|| (lToken->name == INT)	// First de Factor vvv
		|| (lToken->name == STRING)
		|| lToken->tokenName() == LP
		|| lToken->name == ID)
		expression();
	else if (lToken->name == NEW)
		allocExpression();
	else
		error("attribuition statement malformed");
}

void
Parser::printStat()
{
	if (lToken->name == PRINT)
	{
		advance();
		expression();
	}
}

void
Parser::readStat()
{ 
	if (lToken->name == READ)
	{
		advance();
		lValue();
	}
}

void
Parser::returnStat()
{ 
	if (lToken->name == RETURN)
	{
		advance();
		expression();
	}
}

void
Parser::superStat()
{ 
	if (lToken->name == SUPER)
	{
		advance();
		match(LP, "'(' expected after 'super'");
		argListOpt();
		match(RP, "')' expected");
	}
}

void
Parser::ifStat()
{ 
	if (lToken->name == IF)
	{
		advance();
		currentST = new SymbolTable(currentST);
		match(LP, "'(' expected after 'if' statement");
		expression();
		match(RP, "')' expected closing 'if' statement");
		match(LR, "'{' expected after 'if' block");
		statementList();
		match(RR, "'}' expected closing 'if' block");
		currentST = currentST->getParent();
		if (lToken->name == ELSE)
		{
			advance();
			currentST = new SymbolTable(currentST);
			match(LR, "'{' expected after 'else' statement");
			statementList();
			match(RR, "'}' expected closing 'else' block");
			currentST = currentST->getParent();
		}
	}
}

void
Parser::forStat()
{ 
	if (lToken->name == FOR)
	{
		advance();
		currentST = new SymbolTable(currentST);
		match(LP, "'(' expected after 'for' statement");
		atribStatOpt();
		match(SC, "';' expected after loop variable");
		expressionOpt();
		match(SC, "';' expected after loop expression");
		atribStatOpt();
		match(RP, "')' expected closing 'for' statement");
		match(LR, "'{' expected after 'for' block");
		statementList();
		match(RR, "'}' expected closing 'for' block");
		currentST = currentST->getParent();
	}
}
// END: Statement section


// BEGIN: Last section

void
Parser::atribStatOpt()
{
	if (lToken->name == ID)
	{
		if (lToken->attribute == TYPE)
			error("type name cannot be used as variable name in for loop");
		atribStat();
	}
}

void
Parser::expressionOpt()
{
	if (lToken->tokenName() == AN
		|| lToken->tokenName() == SN
		|| (lToken->name == INT)	// First de Factor vvv
		|| (lToken->name == STRING)
		|| lToken->tokenName() == LP
		|| lToken->name == ID)
		expression();
}

void
Parser::lValue()
{
	if (lToken->name == ID)
	{
		if (!currentST->get(lToken->lexeme))
				error("variable used but not initialized");
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
		if (!currentST->get(lToken->lexeme))
			error("such . field does not exist");
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
		lValueComp();
	}
	else if (lToken->tokenName() == LB)
	{
		advance();
		expression();
		match(RB, "']' expected");
		lValueComp();
	}
}

void
Parser::expression()
{
	numExpression();
	if (lToken->name == RELOP)
	{
		match(RELOP, "relational operator expected");
		numExpression();
	}
}

void
Parser::allocExpression()
{
	if (lToken->name == NEW)
	{
		advance();

		if (lToken->name == INT_TYPE)
			match(INT_TYPE, "'new' statement incomplete, type expected");
		else if (lToken->name == STRING_TYPE)
			match(STRING_TYPE, "'new' statement incomplete, type expected");
		else if (lToken->name == ID)
		{
			if (!currentST->get(lToken->lexeme))
				error("type does not exist");
			match(ID, "'new' statement incomplete, type expected");
		}

		match(LP, "'(' expected after 'new' statement");
		argListOpt();
		match(RP, "')' expected closing 'new' statement");
	}
	else if
		(lToken->tokenName() == TYPE
		|| lToken->name == INT_TYPE
		|| lToken->name == STRING_TYPE)
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
	factor();
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
	if (lToken->tokenName() == AN || lToken->tokenName() == SN)
		argList();
}

void
Parser::argList()
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
Parser::initSymbolTable()
{
	Token* t;

	t = new Token(CLASS, "class");
	globalST->add(new STEntry(t, true));
	t = new Token(EXTENDS, "extends");
	globalST->add(new STEntry(t, true));
	t = new Token(CONSTRUCTOR, "constructor");
	globalST->add(new STEntry(t, true));
	t = new Token(BREAK, "break");
	globalST->add(new STEntry(t, true));
	t = new Token(PRINT, "print");
	globalST->add(new STEntry(t, true));
	t = new Token(READ, "read");
	globalST->add(new STEntry(t, true));
	t = new Token(RETURN, "return");
	globalST->add(new STEntry(t, true));
	t = new Token(SUPER, "super");
	globalST->add(new STEntry(t, true));
	t = new Token(IF, "if");
	globalST->add(new STEntry(t, true));
	t = new Token(ELSE, "else");
	globalST->add(new STEntry(t, true));
	t = new Token(FOR, "for");
	globalST->add(new STEntry(t, true));
	t = new Token(NEW, "new");
	globalST->add(new STEntry(t, true));
	t = new Token(INT_TYPE, "int");
	globalST->add(new STEntry(t, true));
	t = new Token(STRING_TYPE, "string");
	globalST->add(new STEntry(t, true));
}

void
Parser::error(string str)
{
	cout << "Line " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}