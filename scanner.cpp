#include "scanner.h"    


//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable* table)
{
  pos = 0;
  line = 1;

  st = table;

  ifstream inputFile(input, ios::in);
  string line;

  if (inputFile.is_open())
  {
    while (getline(inputFile, line))
    {
      this->input.append(line + '\n');
    }
    inputFile.close();
  }
  else
    cout << "Unable to open file\n";

  //A próxima linha deve ser comentada posteriormente.
  //Ela é utilizada apenas para verificar se o 
  //preenchimento de input foi feito corretamente.
  // cout << this->input;

}

int
Scanner::getLine()
{
  return line;
}

//Método que retorna o próximo token da entrada
Token*
Scanner::nextToken()
{
  Token* tok;
  string lexeme;
  int state{0}; // Estado inicial
  STEntry* obj;

  while (true)
  {
    switch (state)
    {
    case 0: // Equivalente aos estados 3, 6, 9, 15 e 28
      if (input[pos] == '\0')
      {
        tok = new Token(END_OF_FILE);
        return tok;
      }
      else if (input[pos] == '\n') // Quebra de linha
        state = 44;
      else if (isalpha(input[pos]) || input[pos] == '_') // Começo de um ID
      {
        state = 1;
        lexeme.push_back(input[pos]);
      }
      else if (isdigit(input[pos])) // Começo de um INT
      {
        state = 4;
        lexeme.push_back(input[pos]);
      }
      else if (input[pos] == '"') // Começo de uma STRING
        state = 7;
      else if (input[pos] == '/') // Comentario ou Division
        state = 10;
      else if (isspace(input[pos])) // Espaço em branco
        state = 16;
      else if (input[pos] == '<')
        state = 29;
      else if (input[pos] == '>')
        state = 32;
      else if (input[pos] == '=')
        state = 35;
      else if (input[pos] == '!')
        state = 38;
      else if (input[pos] == '+') // Estado 40
      {
        tok = new Token(OP, AN);
        pos++;
        return tok;
      }
      else if (input[pos] == '-') // Estado 41
      {
        tok = new Token(OP, SN);
        pos++;
        return tok;
      }
      else if (input[pos] == '*') // Estado 42
      {
        tok = new Token(OP, MN);
        pos++;
        return tok;
      }
      else if (input[pos] == '%') // Estado 44
      {
        tok = new Token(OP, RN);
        pos++;
        return tok;
      }
      else if (input[pos] == '(') // Estado 19
      {
        tok = new Token(SEP, LP);
        pos++;
        return tok;
      }
      else if (input[pos] == ')') // Estado 20
      {
        tok = new Token(SEP, RP);
        pos++;
        return tok;
      }
      else if (input[pos] == '[') // Estado 21
      {
        tok = new Token(SEP, LB);
        pos++;
        return tok;
      }
      else if (input[pos] == ']') // Estado 22
      {
        tok = new Token(SEP, RB);
        pos++;
        return tok;
      }
      else if (input[pos] == '{') // Estado 23
      {
        tok = new Token(SEP, LR);
        pos++;
        return tok;
      }
      else if (input[pos] == '}') // Estado 24
      {
        tok = new Token(SEP, RR);
        pos++;
        return tok;
      }
      else if (input[pos] == ';') // Estado 25
      {
        tok = new Token(SEP, SC);
        pos++;
        return tok;
      }
      else if (input[pos] == '.') // Estado 26
      {
        tok = new Token(SEP, P);
        pos++;
        return tok;
      }
      else if (input[pos] == ',') // Estado 27
      {
        tok = new Token(SEP, C);
        pos++;
        return tok;
      }
      else
        lexicalError("Token nao reconhecido.\n");

      pos++;
      break;

    case 1: // Identificacao de um ID    
      if (isalnum(input[pos]) || input[pos] == '_')
      {
        lexeme.push_back(input[pos]);
        pos++;
      }
      else
        state = 2;

      break;

    case 2: // Decide entre ID e Palavra Reservada
      obj = st->get(lexeme);
      if (!obj)
        tok = new Token(ID, lexeme); // Certeza que eh um ID *NOVO*
      else
        tok = new Token(obj->token->name, obj->token->attribute, obj->token->lexeme); // ID ja definido ou Palavra Reservada

      return tok;

    case 4:  // Identificacao de um INT
      if (isdigit(input[pos]))
      {
        lexeme.push_back(input[pos]);
        pos++;
      }
      else
        state = 5;

      break;

    case 5:
      tok = new Token(INT, lexeme);
      return tok;

    case 7: // Identificacao de uma STRING
      if (!(input[pos] == '"'))
      {
        lexeme.push_back(input[pos]);
        pos++;
      }
      else
      {
        state = 8;
        pos++;
      }

      break;

    case 8:
      tok = new Token(STRING, lexeme);
      return tok;

    case 10: // Identificacao de Comentarios
      if (input[pos] == '/') // Eh um comentario de uma linha
      {
        state = 11;
        pos++;
      }
      else if (input[pos] == '*') // Eh um comentario de multiplas linhas
      {
        state = 13;
        pos++;
      }
      else
        state = 43;

      break;

    case 11:
      if (!(input[pos] == '\n'))
        pos++;
      else
      {
        state = 0;
        line++;
        pos++;
      }

      break;

    case 13:
      if (input[pos] == '*') // Possivel fim de comentario de multiplas linhas
      {
        state = 14;
        pos++;
      }
      else if (input[pos] == '\n')
      {
        line++;
        pos++;
      }
      else
        pos++;

      break;

    case 14:
      if (input[pos] == '/') // Fim de comentario de multiplas linhas
      {
        state = 0;
        pos++;
      }
      else // Alarme falso
      {
        state = 13;
        pos++;
      }

      break;

    case 16: // Espaços em branco
      if (isspace(input[pos]))
        pos++;
      else
        state = 0;

      break;

    case 29:
      if (input[pos] == '=')
      {
        pos++;
        tok = new Token(RELOP, LE);
        return tok;
      }
      else
      {
        tok = new Token(RELOP, LT);
        return tok;
      }

      break;

    case 32:
      if (input[pos] == '=')
      {
        pos++;
        tok = new Token(RELOP, GE);
        return tok;
      }
      else
      {
        tok = new Token(RELOP, GT);
        return tok;
      }

      break;

    case 35:
      if (input[pos] == '=')
      {
        pos++;
        tok = new Token(RELOP, EQ);
        return tok;
      }
      else
      {
        tok = new Token(OP, AS);
        return tok;
      }

      break;

    case 38:
      if (input[pos] == '=')
      {
        pos++;
        tok = new Token(RELOP, NE);
        return tok;
      }
      else
        lexicalError("! sem =\n");
      // Tratar o que a mais?

      break;

    case 43: // Operador de Divisao
      tok = new Token(OP, DN);
      return tok;

    case 44: // Quebra de linha
      line++;
      if (input[pos] == '\n')
        pos++;
      else
        state = 0;

      break;

    }

  }

  return tok;

} // nextToken()

void
Scanner::lexicalError(string msg)
{
  cout << "Linha " << line << ": " << msg << endl;

  exit(EXIT_FAILURE);
}
