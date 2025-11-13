#ifndef __Scanner_h
#define __Scanner_h


#include <fstream>
#include "token.h"
#include "symboltable.h"


class Scanner
{
public:
  //Construtor
  Scanner(string, SymbolTable*);

  int getLine();

  //Método que retorna o próximo token da entrada
  Token* nextToken();

  //Método para manipular erros
  void lexicalError(string);

private:
  string input;//Armazena o texto de entrada
  int pos;//Posição atual
  int line;
  SymbolTable* st;
};


#endif // __Scanner_h