// lexer.cpp
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "lexer.h"

std::vector<Token> Lexer::analizar(std::ifstream& codigo) {
  std::vector<Token> tokens;
  
  char c;
  while(codigo.get(c)){
    if(c == '#'){
      while(c != '\n')
        codigo.get(c);
      continue; 
    }

    if (isspace(c))
      continue;

    if (isalpha(c)) {
      std::string palabra(1, c);
      while (isalnum(codigo.peek()) || codigo.peek() == '_') {
        palabra += codigo.get();
      }
      auto it = palabrasClave.find(palabra);
      if (it != palabrasClave.end()) {
        tokens.push_back({it->second, palabra});
      } else {
        tokens.push_back({TokenType::IDENT, palabra});
      }
      continue;
    }

    if (std::isdigit(c) || (c == '-' && std::isdigit(codigo.peek()))) {
      if(isalpha(codigo.peek())) throw std::runtime_error("Error léxico: No puede haber un numero antes de un texto");
      std::string numero;
      bool esDecimal = false;

      if(c == '-'){
        numero += c;
        c = codigo.get();
      }

      numero += c;


      while (isdigit(codigo.peek()) || codigo.peek() == '.') {
        if (codigo.peek() == '.') {
          if (esDecimal) break; // solo un punto
          esDecimal = true;
        }
        numero += codigo.get();
        }
      tokens.push_back({esDecimal ? TokenType::NUM_DECIMAL : TokenType::NUM_ENTERO, numero});
      continue;
    }

    // Cadenas entre comillas
    if (c == '"') {
      std::string cadena;
      while (codigo.peek() != '"' && codigo.peek() != EOF) {
        cadena += codigo.get();
      }
      codigo.get(); // consumir comilla de cierre
      tokens.push_back({TokenType::LITERAL_CADENA, cadena});
      continue;
    }
    switch (c) {
      case '=':
        if (codigo.peek() == '=') {
          codigo.get();
          tokens.push_back({TokenType::IGUAL_IGUAL, "=="});
          continue;
        }
        tokens.push_back({TokenType::IGUAL, "="});
        break;
      case '+': tokens.push_back({TokenType::MAS, "+"}); break;
      case '-': tokens.push_back({TokenType::MENOS, "-"}); break;
      case '*': tokens.push_back({TokenType::MULT, "*"}); break;
      case '/': tokens.push_back({TokenType::DIV, "/"}); break;
      case '>': tokens.push_back({TokenType::MAYOR_QUE, ">"}); break;
      case '<': tokens.push_back({TokenType::MENOR_QUE, "<"}); break;
      case ';': tokens.push_back({TokenType::PUNTO_Y_COMA, ";"}); break;
      case ',': tokens.push_back({TokenType::COMA, ","}); break;
      case '(': tokens.push_back({TokenType::PARENTESIS_IZQ, "("}); break;
      case ')': tokens.push_back({TokenType::PARENTESIS_DER, ")"}); break;
      default:
        throw std::runtime_error("Error léxico: carácter no reconocido '" + std::string(1, c) + "'");
    }
  }
  
  tokens.push_back({TokenType::EOF_TOKEN, ""});
  return tokens;
}

std::string Lexer::tokenTypeToString(TokenType tipo) {
  switch (tipo) {
    case TokenType::FUNCION: return "FUNCION";
    case TokenType::CONFIGURACION: return "CONFIGURACION";
    case TokenType::PRINCIPAL: return "PRINCIPAL";
    case TokenType::DEFINIR_PIN: return "DEFINIR_PIN";
    case TokenType::SALIDA: return "SALIDA";
    case TokenType::ENTRADA: return "ENTRADA";
    case TokenType::ENTERO: return "ENTERO";
    case TokenType::DECIMAL: return "DECIMAL";
    case TokenType::CADENA: return "CADENA";
    case TokenType::BOOLEANO: return "BOOLEANO";
    case TokenType::VERDADERO: return "VERDADERO";
    case TokenType::FALSO: return "FALSO";
    case TokenType::FIN: return "FIN";
    case TokenType::FIN_MIENTRAS: return "FIN_MIENTRAS";
    case TokenType::FIN_POR: return "FIN_POR";
    case TokenType::MIENTRAS: return "MIENTRAS";
    case TokenType::SI: return "SI";
    case TokenType::SINO: return "SINO";
    case TokenType::HECHO: return "HECHO";
    case TokenType::POR: return "POR";
    case TokenType::DURANTE: return "DUTANTE";
    case TokenType::ESPERAR: return "ESPERAR";
    case TokenType::LEER: return "LEER";
    case TokenType::PRENDER: return "PRENDER";
    case TokenType::APAGAR: return "APAGAR";
    case TokenType::IDENT: return "IDENT";
    case TokenType::NUM_ENTERO: return "NUM_ENTERO";
    case TokenType::NUM_DECIMAL: return "NUM_DECIMAL";
    case TokenType::LITERAL_CADENA: return "LITERAL_CADENA";
    case TokenType::IGUAL: return "IGUAL";
    case TokenType::MAS: return "MAS";
    case TokenType::MENOS: return "MENOS";
    case TokenType::MULT: return "MULT";
    case TokenType::DIV: return "DIV";
    case TokenType::MENOR_QUE: return "MENOR_QUE";
    case TokenType::MAYOR_QUE: return "MAYOR_QUE";
    case TokenType::IGUAL_IGUAL: return "IGUAL_IGUAL";
    case TokenType::PUNTO_Y_COMA: return "PUNTO_Y_COMA";
    case TokenType::COMA: return "COMA";
    case TokenType::PARENTESIS_IZQ: return "PARENTESIS_IZQ";
    case TokenType::PARENTESIS_DER: return "PARENTESIS_DER";
    case TokenType::EOF_TOKEN: return "EOF_TOKEN";
    case TokenType::BOCINA: return "BOCINA";
    case TokenType::ERROR: return "ERROR";
    default: return "DESCONOCIDO";
  }
}
