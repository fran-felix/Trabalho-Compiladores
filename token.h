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
    OP, // 4 - Operator
    LE, // 5 - Lesser or equal
    LT, // 6 - Lesser than
    GE, // 7 - Greater or equal
    GT, // 8 - Greater than
    EQ, // 9 - Equal to
    AS, // 10 - Assignment
    NE, // 11 - Not equal to
    AN, // 12 - Addition
    SN, // 13 - Subtraction
    MN, // 14 - Multiplication
    DN, // 15 - Division
    RN, // 16 - Remainder
    SEP, // 17 - Separator
    LP, // 18 - Left Parentheses
    RP, // 19 - Right Parentheses
    LB, // 20 - Left Bracket
    RB, // 21 - Right Bracket
    LR, // 22 - Left Brace
    RR, // 23 - Right Brace
    SC, // 24 - Semi-colon
    P, // 25 - Period
    C, // 26 - Comma
    END_OF_FILE // 27
};

class Token 
{
public: 
    int name;
    int attribute;
    string lexeme;
    
    Token(int name)
    {
        this->name = name;
        attribute = UNDEF;
    }

    Token(int name, string l)
    {
        this->name = name;
        attribute = UNDEF;
        lexeme = l;
    }
    
    Token(int name, int attr)
    {
        this->name = name;
        attribute = attr;
    }
};
