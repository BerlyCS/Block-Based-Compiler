#include <iostream>
#include <unordered_map>
#include "symbolTable.h"

void TablaSimbolos::declarar(const std::string& nombre, TipoDato tipo) {
  if (tabla.count(nombre)) {
    throw std::runtime_error("Error semántico: variable '" + nombre + "' ya declarada.");
  }
  tabla[nombre] = {nombre, tipo, false};
}

void TablaSimbolos::asignar(const std::string& nombre, TipoDato tipoExpr) {
  if (!tabla.count(nombre)) {
    throw std::runtime_error("Error semántico: variable '" + nombre + "' no declarada.");
  }
  TipoDato tipoVar = tabla[nombre].tipo;
  if (tipoVar != tipoExpr) {
    throw std::runtime_error("Error semántico: tipo incompatible en asignación a '" + nombre + "'.");
  }
  tabla[nombre].inicializado = true;
}

TipoDato TablaSimbolos::obtenerTipo(const std::string& nombre) {
  if (!tabla.count(nombre)) {
    throw std::runtime_error("Error semántico: variable '" + nombre + "' no declarada.");
  }
  return tabla[nombre].tipo;
}

bool TablaSimbolos::estaInicializada(const std::string& nombre) {
    return tabla.count(nombre) && tabla[nombre].inicializado;
}
