#ifndef __Token_h
#define __Token_h

#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>


using namespace std;

enum Names
{
  UNDEF, // 0
  ID, // 1
  INT, // 2
  STRING, // 3
  // Nao precisa pra comentario, precisa?
  RELOP, // 4 - Relational Operator
  LE, // 5 - Lesser or equal
  LT, // 6 - Lesser than
  GE, // 7 - Greater or equal
  GT, // 8 - Greater than
  EQ, // 9 - Equal to
  NE, // 10 - Not equal to
  OP, // 11 - Operator
  AS, // 12 - Assignment
  AN, // 13 - Addition
  SN, // 14 - Subtraction
  MN, // 15 - Multiplication
  DN, // 16 - Division
  RN, // 17 - Remainder
  SEP, // 18 - Separator
  LP, // 19 - Left Parentheses
  RP, // 20 - Right Parentheses
  LB, // 21 - Left Bracket
  RB, // 22 - Right Bracket
  LR, // 23 - Left Brace
  RR, // 24 - Right Brace
  SC, // 25 - Semi-colon
  P, // 26 - Period
  C, // 27 - Comma
  END_OF_FILE, // 28 - EOF
  // Palavras reservadas vvv
  CLASS, // 29
  EXTENDS, // 30
  CONSTRUCTOR, // 31
  BREAK, // 32
  PRINT, // 33
  READ, // 34
  RETURN, // 35
  SUPER, // 36
  IF, // 37
  ELSE, // 38
  FOR, // 39
  NEW, // 40
  TYPE, // 41 Generic type created by the user
  INT_TYPE, // 42
  STRING_TYPE // 43
};

class Token
{
public:
  int name;
  int attribute;
  string lexeme;

  Token(int name) : name{name}, attribute{UNDEF}
  {
  }

  Token(int name, string l) : name{name}, attribute{UNDEF}
  {
    lexeme = l;
  }

  Token(int name, int attr) : name{name}, attribute{attr}
  {
  }

  Token(int name, int attr, string l) : name{name}, attribute{attr}
  {
    lexeme = l;
  }

  // Para usar junto do cout em main.cpp que imprime os tokens identificados
  int
  tokenName()
  {
    if (attribute != UNDEF)
      return attribute;
    return name;
  }
};


#endif // __Token_h