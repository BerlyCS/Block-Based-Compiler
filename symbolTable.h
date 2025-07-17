#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <unordered_map>

enum class TipoDato {
  ENTERO,
  DECIMAL,
  CADENA,
  BOOLEANO,
  FUNCION
};

struct Simbolo {
  std::string nombre;
  TipoDato tipo;
  bool inicializado;
};

class TablaSimbolos {
private:
  std::unordered_map<std::string, Simbolo> tabla;

public:
  void declarar(const std::string& nombre, TipoDato tipo);
  void asignar(const std::string& nombre, TipoDato tipoExpr);
  TipoDato obtenerTipo(const std::string& nombre);
  bool estaInicializada(const std::string& nombre);
};

#endif
