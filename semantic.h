#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <vector>
#include "parser.h"
#include "symbolTable.h"

class SemanticAnalyzer {
private:
  TablaSimbolos simbols;

  TipoDato getTipe(TokenType t);
public:
  void verificar(std::vector<Token> &tokens);
};

#endif