#include "token.h"

class STEntry 
{
public:
  Token* token{nullptr};
  bool reserved{false};
    
  STEntry();
  STEntry(Token*);   
  STEntry(Token*, bool);
};
