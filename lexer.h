#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <unordered_map>

enum class TokenType {
  // Palabras clave
  FUNCION,
  CONFIGURACION,
  PRINCIPAL,
  DEFINIR_PIN,
  SALIDA,
  ENTRADA,
  ENTERO,
  DECIMAL,
  CADENA,
  BOOLEANO,
  VERDADERO,
  FALSO,
  FIN,
  FIN_MIENTRAS,
  FIN_POR,
  MIENTRAS,
  SI,
  SINO,
  HECHO,
  POR,
  DURANTE,
  ESPERAR,
  LEER,
  PRENDER,
  APAGAR,
  BOCINA,

  // Identificadores y literales
  IDENT,          // Nombres de variables como `valor1`, `BOTON`, etc.
  NUM_ENTERO,     // Ej: 10, 20, 3
  NUM_DECIMAL,    // Ej: 23.5, 30.0
  LITERAL_CADENA, // Ej: "Sistema listo"

  // Operadores
  IGUAL,          // =
  MAS,            // +
  MENOS,          // -
  MULT,           // *
  DIV,            // /
  MENOR_QUE,      // <
  MAYOR_QUE,      // >
  IGUAL_IGUAL,    // ==

  // Símbolos
  PUNTO_Y_COMA,   // ;
  COMA,           // ,
  PARENTESIS_IZQ, // (
  PARENTESIS_DER, // )

  // Fin de archivo o error
  EOF_TOKEN,
  ERROR
};

struct Token {
  TokenType tipo;
  std::string valor;
};

class Lexer {
private:
  const std::unordered_map<std::string, TokenType> palabrasClave = {
    {"funcion", TokenType::FUNCION},
    {"configuracion", TokenType::CONFIGURACION},
    {"principal", TokenType::PRINCIPAL},
    {"definir_pin", TokenType::DEFINIR_PIN},
    {"salida", TokenType::SALIDA},
    {"entrada", TokenType::ENTRADA},
    {"num", TokenType::ENTERO},
    {"dec", TokenType::DECIMAL},
    {"cadena", TokenType::CADENA},
    {"bool", TokenType::BOOLEANO},
    {"VERDADERO", TokenType::VERDADERO},
    {"FALSO", TokenType::FALSO},
    {"fin", TokenType::FIN},
    {"fin_mientras", TokenType::FIN_MIENTRAS},
    {"fin_por", TokenType::FIN_POR},
    {"mientras", TokenType::MIENTRAS},
    {"si", TokenType::SI},
    {"sino", TokenType::SINO},
    {"hecho", TokenType::HECHO},
    {"por", TokenType::POR},
    {"durante", TokenType::DURANTE},
    {"esperar", TokenType::ESPERAR},
    {"leer", TokenType::LEER},
    {"prender", TokenType::PRENDER},
    {"apagar", TokenType::APAGAR},
    {"bocina", TokenType::BOCINA}
  };
public:
  std::vector<Token> analizar(std::ifstream& codigo);
  std::string tokenTypeToString(TokenType tipo);
};

#endif