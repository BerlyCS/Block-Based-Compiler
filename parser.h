#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "lexer.h"

class Parser {
private:
  std::vector<Token> tokens;
  int pos;

  Token peek() {return tokens[pos]; }
  Token advance() {return tokens[pos++];}
  bool match(TokenType tipo);
  void expect(TokenType tipo);
  void parseConfigurationBlock();
  void parseBlock();
  void parseDefinePin();
  void parseInstruction(TokenType stopToken);
  void parseListFunction();
  void parseConditional();
  void parseWhile();
  void parseFor();
  void parseTipePin();
  void parseDeclaration(TokenType stopToken);
  void parseBoolOperation();

  void parsePrender(TokenType stopToken);
  void parseApagar(TokenType stopToken);
  void parseEsperar(TokenType stopToken);
  void parseVariableDeclaration(TokenType stopToken);
  void parseDefinition();
  void parseVariableAsign(TokenType stopToken);
  void parseReadPin();
  void parseSino();
  void parsePrenderBocina();
public:
  Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}
  void analizar();
};

#endif