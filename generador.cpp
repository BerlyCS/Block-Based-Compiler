#include <iostream>
#include "generador.h"

using namespace std;

Generador::Generador(vector<Token> &t, string name) : tokens(t), nameFile(name), indentLevel(0) {
  fs::create_directories(nameFile);
  file.open(nameFile + "/" + nameFile + ".ino");
  it = tokens.begin();
}

void Generador::escribirSangria() {
  for (size_t i = 0; i < indentLevel; ++i)
    file << "\t";
}

void Generador::traducir(){
  while(it != tokens.end()){
    switch (it->tipo){
    case TokenType::DEFINIR_PIN:
      traducePin();
      break;
    case TokenType::CONFIGURACION:
      traduceConfigFun();
      break;
    case TokenType::PRINCIPAL:
      traducePrincipalFun();
      break;
    default:
      break;
    }
    it++;
  }
}

void Generador::traducePin(){
  file << "const int ";
  it += 2;
  file << it->valor << " = ";
  it += 2;
  file << it->valor <<";\n";
}

void Generador::traduceConfigFun(){
  file << "\nvoid setup(){\n";
  indentLevel++;
  escribirSangria();
  file << "Serial.begin(9600);\n";
  it += 3;
  while(it->tipo != TokenType::FIN){
    escribirSangria();
    if(it->tipo == TokenType::SALIDA){
      it += 2;
      file << "pinMode(" << it->valor << ", OUTPUT);\n";
      it += 3;
    } else if (it->tipo == TokenType::BOCINA){ 
      it += 2;
      file << "pinMode(" << it->valor << ", OUTPUT);\n";
      it += 3;
    } else {
      it += 2;
      file << "pinMode(" << it->valor << ", INPUT_PULLUP);\n";
      it += 3;
    }
  }

  indentLevel--;
  file << "}\n\n";
}

void Generador::traducePrincipalFun(){
  file << "void loop(){\n";
  indentLevel++;
  it += 3;
  traduceInstruction(TokenType::FIN);
  indentLevel--;
  file << "}\n";
}

void Generador::traduceInstruction(TokenType tokenFin){
  while(it->tipo != tokenFin){
    switch(it->tipo){
      case TokenType::POR:
        traduceFor();
        break;
      case TokenType::MIENTRAS:
        traduceWhile();
        break;
      case TokenType::SI:
        traduceIf();
        break;
      case TokenType::BOOLEANO:
        traduceDefVar("bool");
        break;
      case TokenType::ENTERO:
        traduceDefVar("int");
        break;
      case TokenType::CADENA:
        traduceDefVar("string");
        break;
      case TokenType::DECIMAL:
        traduceDefVar("float");
        break;
      case TokenType::APAGAR:
        tradiceDigitalWrite("LOW");
        break;
      case TokenType::PRENDER:      
        tradiceDigitalWrite("HIGH");
        break;
      case TokenType::ESPERAR:{
        it += 2;
        escribirSangria();
        file << "delay(" << it->valor << ");\n";
        break;
      }
      case TokenType::IDENT:
        traduceInitVar();
        break;
      default:
        break;
    }
    it++;
  }
}

void Generador::traduceFor(){
  escribirSangria();

  it++;  // Ir al nombre de la variable
  string nomVar = it->valor;

  it += 3; // Ir al valor inicial
  string inicio = it->valor;

  it += 2; // Ir al límite
  string limite = it->valor;

  it += 2; // Ir al incremento
  string incremento = it->valor;

  file << "for(int " << nomVar << " = " << inicio 
       << "; " << nomVar << " < " << limite 
       << "; " << nomVar << " += " << incremento << ") {\n";

  indentLevel++;
  it += 2;
  traduceInstruction(TokenType::FIN_POR);

  indentLevel--;
  escribirSangria();
  file << "}\n";
}

void Generador::traduceWhile(){
  escribirSangria();

  it += 2;
  string nomVar = it->valor;

  it ++;
  string op = it->valor;

  it ++;
  string value;
  if(it->tipo == TokenType::VERDADERO){
    value = "true";
  } else if (it->tipo == TokenType::FALSO){
    value = "false";
  } else {
    value = it->valor;
  }  

  file << "while(" << nomVar << " " << op << " " << value << "){\n";
  indentLevel++;
  it += 2;
  traduceInstruction(TokenType::FIN_MIENTRAS);
  indentLevel--;

  escribirSangria();
  file << "}\n";
}

void Generador::traduceIf(){
  escribirSangria();

  it += 2;
  string nomVar;
  if(it->tipo == TokenType::LEER){
    it += 2;
    nomVar = "digitalRead(" + it->valor + ")";
    it++;
  } else {
    nomVar = it->valor;
  }

  it ++;
  string op = it->valor;

  it ++;
  string value;
  if(it->tipo == TokenType::VERDADERO){
    value = "true";
  } else if (it->tipo == TokenType::FALSO){
    value = "false";
  } else {
    value = it->valor;
  }  
  
  file << "if (" << nomVar << " " << op << " " << value << "){\n";
  indentLevel++;
  it += 2;
  traduceInstruction(TokenType::HECHO);
  indentLevel--;

  escribirSangria();
  file << "}\n";

  it++;
  if(it->tipo == TokenType::SINO){
    escribirSangria();
    file << "else {\n";
    it++;
    indentLevel++;
    traduceInstruction(TokenType::HECHO);
    indentLevel--;

    escribirSangria();
    file << "}\n";
  } else {
    it--;
  }
}

void Generador::traduceDefVar(string tipe){
  escribirSangria();
  it++;
  string name = it->valor;

  it++;
  if(it->tipo == TokenType::PUNTO_Y_COMA){
    file << tipe << " " << name << ";\n";
    return;
  }

  it++;
  string value;
  if(it->tipo == TokenType::VERDADERO){
    value = "true";
  } else if (it->tipo == TokenType::FALSO){
    value = "false";
  } else if (it->tipo == TokenType::LEER) {
    it += 2;
    value = "digitalRead(" + it->valor + ")";
  } else {
    value = it->valor;
  }  
  file << tipe << " " << name << " = " << value << ";\n";

  it++;
}

void Generador::traduceInitVar(){
  escribirSangria();
  string nameVar = it->valor;
  
  it +=2;
  string value;
  if(it->tipo == TokenType::VERDADERO){
    value = "true";
  } else if (it->tipo == TokenType::FALSO){
    value = "false";
  } else if (it->tipo == TokenType::LEER) {
    it += 2;
    value = "digitalRead(" + it->valor + ")";
  } else {
    value = it->valor;
  }  
  file << nameVar << " = " << value << ";\n";

  it++;
}

void Generador::tradiceDigitalWrite(string value){
  escribirSangria();
  it += 2;
  std::string varName = it->valor;

  it++;
  if(it->tipo == TokenType::COMA){
    it++;
    bocinas.insert(varName);
    file << "tone(" << varName << ", " << it->valor << ");\n";
    it+= 2;
    return;
  }

  if(bocinas.find(varName) != bocinas.end() && value == "LOW"){
    file << "noTone(" << varName << ");\n";
    return;
  }
  file << "digitalWrite(" << varName << ", " << value << ");\n";
  it += 1;
}