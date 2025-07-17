// semantic.cpp
#include "semantic.h"

TipoDato SemanticAnalyzer::getTipe(TokenType t){
  switch(t){
    case TokenType::ENTERO:
      return TipoDato::ENTERO;
    case TokenType::DECIMAL:
      return TipoDato::DECIMAL;
    case TokenType::BOOLEANO:
      return TipoDato::BOOLEANO;
    case TokenType::CADENA:
      return TipoDato::CADENA;
    case TokenType::NUM_ENTERO:
      return TipoDato::ENTERO;
    case TokenType::NUM_DECIMAL:
      return TipoDato::DECIMAL;
    case TokenType::LITERAL_CADENA:
      return TipoDato::CADENA;
    case TokenType::VERDADERO:
      return TipoDato::BOOLEANO;
    case TokenType::FALSO:
      return TipoDato::BOOLEANO;
    default:
      throw std::runtime_error("Error Semántico: Tipo de Dato incorrecto");
  }
}

void SemanticAnalyzer::verificar(std::vector<Token> &tokens){
  std::vector<Token>::iterator it = tokens.begin();
  while(it != tokens.end()){
    switch (it->tipo){
    case TokenType::DEFINIR_PIN:{
      it += 2;
      std::string value = it->valor;
      simbols.declarar(value, TipoDato::ENTERO);
      it += 2;
      simbols.asignar(value, getTipe(it->tipo));
      break;
    }
    case TokenType::FUNCION:
      it += 1;
      if(it->tipo == TokenType::CONFIGURACION || it->tipo == TokenType::PRINCIPAL) break;
      simbols.declarar(it->valor, TipoDato::FUNCION);
      simbols.asignar(it->valor, TipoDato::FUNCION);
      break;
    case TokenType::ENTRADA:
      it += 2;
      if(!simbols.estaInicializada(it->valor))
        throw std::runtime_error("Error semántico: variable '" + it->valor + "' no declarada.");
      break;
    case TokenType::SALIDA:
      it += 2;
      if(!simbols.estaInicializada(it->valor))
        throw std::runtime_error("Error semántico: variable '" + it->valor + "' no declarada.");
      break;
    case TokenType::ENTERO:{
      it += 1;
      std::string value = it->valor;
      simbols.declarar(value, TipoDato::ENTERO);
      it += 1;
      if(it->tipo != TokenType::PUNTO_Y_COMA){
        it += 1;
        if(it->tipo == TokenType::LEER){
          simbols.asignar(value, TipoDato::ENTERO);
        } else {
          simbols.asignar(value, getTipe(it->tipo));
        }
      }
      break;
    }
    case TokenType::DECIMAL:{
      it += 1;
      std::string value = it->valor;
      simbols.declarar(value, TipoDato::DECIMAL);
      it += 1;
      if(it->tipo != TokenType::PUNTO_Y_COMA){
        it += 1;
        simbols.asignar(value, getTipe(it->tipo));
      }
      break;
    }
    case TokenType::CADENA:{
      it += 1;
      std::string value = it->valor;
      simbols.declarar(value, TipoDato::CADENA);
      it += 1;
      if(it->tipo != TokenType::PUNTO_Y_COMA){
        it += 1;
        simbols.asignar(value, getTipe(it->tipo));
      }
      break;
    }
    case TokenType::BOOLEANO:{
      it += 1;
      std::string value = it->valor;
      simbols.declarar(value, TipoDato::BOOLEANO);
      it += 1;
      if(it->tipo != TokenType::PUNTO_Y_COMA){
        it += 1;
        simbols.asignar(value, getTipe(it->tipo));
      }
      break;
    }
    case TokenType::PRENDER:
      it += 2;
      if(!simbols.estaInicializada(it->valor))
        throw std::runtime_error("Error semántico: variable '" + it->valor + "' no declarada.");
      break;

    case TokenType::APAGAR:
      it += 2;
      if(!simbols.estaInicializada(it->valor))
        throw std::runtime_error("Error semántico: variable '" + it->valor + "' no declarada.");
      break;

    case TokenType::ESPERAR:
      it += 2;
      if(it->tipo != TokenType::IDENT) break;
      if(!simbols.estaInicializada(it->valor))
        throw std::runtime_error("Error semántico: variable '" + it->valor + "' no declarada.");
      break;

    case TokenType::POR:
      it++;
      simbols.declarar(it->valor, TipoDato::ENTERO);
      simbols.asignar(it->valor, TipoDato::ENTERO);
      break;

    case TokenType::IDENT:{
      std::string value = it->valor;
      it += 1;
      if(it->tipo == TokenType::IGUAL){
        it += 1;
        if(it->tipo == TokenType::LEER){
          simbols.asignar(value, TipoDato::ENTERO);
        } else {
          simbols.asignar(value, getTipe(it->tipo));
        }
      } else {
        if(!simbols.estaInicializada(value))
          throw std::runtime_error("Error semántico: variable '" + value + "' no declarada.");
      }
      break;
    }
    default:
      break;
    }
  it += 1;
  }
}


