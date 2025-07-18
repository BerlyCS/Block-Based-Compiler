#include<iostream>
#include "parser.h"

bool Parser::match(TokenType tipo) {
  if (peek().tipo == tipo) {
    advance();
    return true;
  }
  return false;
}

void Parser::expect(TokenType tipo) {
  if (!match(tipo)) {
    std::string tipe;
    switch (tipo) {
      case TokenType::FUNCION: tipe = "FUNCION"; break;
      case TokenType::CONFIGURACION: tipe = "CONFIGURACION"; break;
      case TokenType::PRINCIPAL: tipe = "PRINCIPAL"; break;
      case TokenType::DEFINIR_PIN: tipe = "DEFINIR_PIN"; break;
      case TokenType::SALIDA: tipe = "SALIDA"; break;
      case TokenType::ENTRADA: tipe = "ENTRADA"; break;
      case TokenType::ENTERO: tipe = "ENTERO"; break;
      case TokenType::DECIMAL: tipe = "DECIMAL"; break;
      case TokenType::CADENA: tipe = "CADENA"; break;
      case TokenType::BOOLEANO: tipe = "BOOLEANO"; break;
      case TokenType::VERDADERO: tipe = "VERDADERO"; break;
      case TokenType::FALSO: tipe = "FALSO"; break;
      case TokenType::FIN: tipe = "FIN"; break;
      case TokenType::FIN_MIENTRAS: tipe = "FIN_MIENTRAS";  break;
      case TokenType::FIN_POR: tipe = "FIN_POR"; break;
      case TokenType::MIENTRAS: tipe = "MIENTRAS"; break;
      case TokenType::SI: tipe = "SI"; break;
      case TokenType::SINO: tipe = "SINO"; break;
      case TokenType::HECHO: tipe = "HECHO"; break;
      case TokenType::POR: tipe = "POR"; break;
      case TokenType::DURANTE: tipe = "DURANTE"; break;
      case TokenType::ESPERAR: tipe = "ESPERAR"; break;
      case TokenType::LEER: tipe = "LEER"; break;
      case TokenType::PRENDER: tipe = "PRENDER"; break;
      case TokenType::APAGAR: tipe = "APAGAR"; break;
      case TokenType::IDENT: tipe = "IDENT"; break;
      case TokenType::NUM_ENTERO: tipe = "NUM_ENTERO"; break;
      case TokenType::NUM_DECIMAL: tipe = "NUM_DECIMAL"; break;
      case TokenType::LITERAL_CADENA: tipe = "LITERAL_CADENA"; break;
      case TokenType::IGUAL: tipe = "IGUAL"; break;
      case TokenType::MAS: tipe = "MAS"; break;
      case TokenType::MENOS: tipe = "MENOS"; break;
      case TokenType::MULT: tipe = "MULT"; break;
      case TokenType::DIV: tipe = "DIV"; break;
      case TokenType::MENOR_QUE: tipe = "MENOR_QUE"; break;
      case TokenType::MAYOR_QUE: tipe = "MAYOR_QUE"; break;
      case TokenType::IGUAL_IGUAL: tipe = "IGUAL_IGUAL"; break;
      case TokenType::PUNTO_Y_COMA: tipe = "PUNTO_Y_COMA"; break;
      case TokenType::COMA: tipe = "COMA"; break;
      case TokenType::PARENTESIS_IZQ: tipe = "PARENTESIS_IZQ"; break;
      case TokenType::PARENTESIS_DER: tipe = "PARENTESIS_DER"; break;
      case TokenType::EOF_TOKEN: tipe = "EOF_TOKEN"; break;
      case TokenType::ERROR: tipe = "ERROR"; break;
      default: tipe = "DESCONOCIDO";
    }
    throw std::runtime_error("Error de sintaxis " + tipe);
  }
}

void Parser::analizar(){
  parseDefinePin();
  parseListFunction();
}

void Parser::parseDefinePin(){
  while(peek().tipo != TokenType::FUNCION && peek().tipo != TokenType::EOF_TOKEN){
    expect(TokenType::DEFINIR_PIN);
    expect(TokenType::PARENTESIS_IZQ);
    expect(TokenType::IDENT);
    expect(TokenType::COMA);
    if(!match(TokenType::NUM_ENTERO) && !match(TokenType::IDENT)){
      throw std::runtime_error("Error de sintaxis: Definicion del pin incorrecta");
    }
    expect(TokenType::PARENTESIS_DER);
    expect(TokenType::PUNTO_Y_COMA);
  }
}

void Parser::parseListFunction(){
  while (peek().tipo != TokenType::EOF_TOKEN){
    expect(TokenType::FUNCION);
    if(!match(TokenType::CONFIGURACION) && !match(TokenType::PRINCIPAL) && !match(TokenType::IDENT)){
      throw std::runtime_error("Error de sintaxis: Declaración de funcion incorrecta");
    }
    expect(TokenType::PARENTESIS_IZQ);
    expect(TokenType::PARENTESIS_DER);
    parseBlock();
    expect(TokenType::FIN);
  }
}

void Parser::parseBlock(){
  if(match(TokenType::ENTRADA) || match(TokenType::SALIDA) || match(TokenType::BOCINA)){
    do {
      parseTipePin();
    } while (match(TokenType::ENTRADA) || match(TokenType::SALIDA) || match(TokenType::BOCINA));
  } else {
    parseInstruction(TokenType::FIN);
  }
}

void Parser::parseTipePin(){
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::IDENT);
  expect(TokenType::PARENTESIS_DER);
  expect(TokenType::PUNTO_Y_COMA);
}

void Parser::parseInstruction(TokenType stopToken){
  while(peek().tipo != stopToken){
    if(match(TokenType::SI)){
      parseConditional();
    } else if (match(TokenType::MIENTRAS)) {
      parseWhile();
    } else if (match(TokenType::POR)) {
      parseFor();
    } else {
      parseDeclaration(stopToken);
    }
  }
}

void Parser::parseConditional(){
  expect(TokenType::PARENTESIS_IZQ);
  parseBoolOperation();
  expect(TokenType::PARENTESIS_DER);
  parseInstruction(TokenType::HECHO);
  expect(TokenType::HECHO);
  if(match(TokenType::SINO)){
    parseSino();
  }
}

void Parser::parseSino(){
  parseInstruction(TokenType::HECHO);
  expect(TokenType::HECHO);
}

void Parser::parseWhile(){
  expect(TokenType::PARENTESIS_IZQ);
  parseBoolOperation();
  expect(TokenType::PARENTESIS_DER);
  parseInstruction(TokenType::FIN_MIENTRAS);
  expect(TokenType::FIN_MIENTRAS);
}

void Parser::parseFor(){
  expect(TokenType::IDENT);
  expect(TokenType::DURANTE);
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::NUM_ENTERO);
  expect(TokenType::COMA);
  expect(TokenType::NUM_ENTERO);
  expect(TokenType::COMA);
  expect(TokenType::NUM_ENTERO);
  expect(TokenType::PARENTESIS_DER);
  parseInstruction(TokenType::FIN_POR);
  expect(TokenType::FIN_POR);
}

void Parser::parseBoolOperation() {
  /*
  Gramatica: <IDENT> <OPERADOR_COMPARACION> (<IDENT> | <NUM> | VERDADERO | FALSO)
  */
  //expect(TokenType::IDENT);
  if(match(TokenType::LEER)){
    expect(TokenType::PARENTESIS_IZQ);
    expect(TokenType::IDENT);
    expect(TokenType::PARENTESIS_DER);
  } else {
    expect(TokenType::IDENT);
  }

  if (!match(TokenType::IGUAL_IGUAL) && !match(TokenType::MENOR_QUE) && !match(TokenType::MAYOR_QUE)) {
    throw std::runtime_error("Error de sintaxis: Se esperaba operador de comparación (==, <, >)");
  }

  TokenType tipo = peek().tipo;
  if (tipo == TokenType::IDENT ||
      tipo == TokenType::NUM_ENTERO ||
      tipo == TokenType::NUM_DECIMAL ||
      tipo == TokenType::VERDADERO ||
      tipo == TokenType::FALSO) {
    advance(); // consumir valor derecho
  } else {
    throw std::runtime_error("Error de sintaxis: Se esperaba una expresión válida en la condición");
  }
}


void Parser::parseDeclaration(TokenType stopToken){
  if(match(TokenType::PRENDER)){
    parsePrender(stopToken);
  } else if(match(TokenType::APAGAR)){
    parseApagar(stopToken);
  } else if(match(TokenType::ESPERAR)){
    parseEsperar(stopToken);
  } else if(match(TokenType::ENTERO) || match(TokenType::DECIMAL) || match(TokenType::BOOLEANO) || match(TokenType::CADENA)) {
    parseVariableDeclaration(stopToken);
  } else if(match(TokenType::IDENT)){
    parseVariableAsign(stopToken);
  } else {
    throw std::runtime_error("Error de sintaxis: Declaración no definida por el lenguaje");
  }
}

void Parser::parseVariableAsign(TokenType stopToken){
  expect(TokenType::IGUAL);
  if(match(TokenType::LEER)){
    parseReadPin();
  } else if(!match(TokenType::NUM_DECIMAL) && !match(TokenType::NUM_ENTERO) && !match(TokenType::LITERAL_CADENA) && !match(TokenType::VERDADERO) && !match(TokenType::FALSO)){
    throw std::runtime_error("Error de sintaxis: Declaración de variable incorrecta");
  }
  expect(TokenType::PUNTO_Y_COMA);
  parseInstruction(stopToken);
}

void Parser::parsePrender(TokenType stopToken){
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::IDENT);
  if(match(TokenType::COMA)){
    parsePrenderBocina();
  }
  expect(TokenType::PARENTESIS_DER);
  expect(TokenType::PUNTO_Y_COMA);
  parseInstruction(stopToken);
}

void Parser::parsePrenderBocina(){
  expect(TokenType::NUM_ENTERO);
}

void Parser::parseEsperar(TokenType stopToken){
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::NUM_ENTERO);
  expect(TokenType::PARENTESIS_DER);
  expect(TokenType::PUNTO_Y_COMA);
  parseInstruction(stopToken);
}

void Parser::parseApagar(TokenType stopToken){
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::IDENT);
  expect(TokenType::PARENTESIS_DER);
  expect(TokenType::PUNTO_Y_COMA);
  parseInstruction(stopToken);
}

void Parser::parseVariableDeclaration(TokenType stopToken){
  expect(TokenType::IDENT);
  parseDefinition();
  parseInstruction(stopToken);
}

void Parser::parseDefinition(){
  if(match(TokenType::PUNTO_Y_COMA)){
    return;
  } else if (match(TokenType::IGUAL)){
    if(match(TokenType::NUM_DECIMAL) || match(TokenType::NUM_ENTERO) || match(TokenType::IDENT) || match(TokenType::VERDADERO) || match(TokenType::FALSO)){
      expect(TokenType::PUNTO_Y_COMA);
    } else if(match(TokenType::LEER)) {
      parseReadPin();
      expect(TokenType::PUNTO_Y_COMA);
    }else {
      throw std::runtime_error("Error de sintaxis: Declaración incorrecta de la variable");
    }
  } else {
    throw std::runtime_error("Error de sintaxis: Declaración incorrecta de la variable");
  }
}

void Parser::parseConfigurationBlock(){
  while(peek().tipo != TokenType::FIN){
    parseTipePin();
  }
}

void Parser::parseReadPin(){
  expect(TokenType::PARENTESIS_IZQ);
  expect(TokenType::IDENT);
  expect(TokenType::PARENTESIS_DER);
}
