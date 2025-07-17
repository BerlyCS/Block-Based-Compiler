#ifndef GENERADOR_H
#define GENERADOR_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include "lexer.h"

namespace fs = std::filesystem;

class Generador{
private:
  std::vector<Token> tokens;
  std::string nameFile;
  std::vector<Token>::iterator it;
  std::unordered_set<std::string> bocinas;
  std::ofstream file;
  size_t indentLevel;
  void escribirSangria();

  void traducePin();
  void traduceConfigFun();
  void traducePrincipalFun();
  void traduceInstruction(TokenType tokenFin);
  void traduceFor();
  void traduceWhile();
  void traduceIf();
  void traduceDefVar(std::string tipe);
  void traduceInitVar();
  void tradiceDigitalWrite(std::string value);
public:
  Generador(std::vector<Token> &t, std::string name);
  void traducir();
};

#endif