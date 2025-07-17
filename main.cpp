#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "colors.h"
#include "generador.h"

using namespace std;

string quitarExtensionAR(const std::string& nombre) {
  size_t pos = nombre.rfind(".ar");
  if (pos != std::string::npos && pos == nombre.length() - 3) {
    return nombre.substr(0, pos);  // Elimina los últimos 3 caracteres
  }
  return nombre;  // No tenía .ar al final
}

int main(int argn, char *argv[]){
  if(argn < 2){
    cerr<<"Uso: "<<argv[0]<<" <archivo.ar>\n";
    return -1;
  }
  cout<<endl;
  ifstream file(argv[1]);
  Lexer l;  
  try {
    vector<Token> v = l.analizar(file);
    for(Token t:v){
      cout<<"["<<l.tokenTypeToString(t.tipo)<<"]\t"<<"->\t"<<t.valor<<endl;
    }
    Parser p(v);
    SemanticAnalyzer s;
    Generador g(v, quitarExtensionAR(argv[1]));

    p.analizar();
    s.verificar(v);
    g.traducir();

    cout<<BOLD<<GREEN<<"Compilación Completada"<<RESET<<endl<<endl;
  }
  catch(const std::runtime_error& e) {
    cerr << BOLD << RED << "Se encontró un ERROR: \n\t" << RESET << e.what() << endl;
  }
  
  return 0;
}
