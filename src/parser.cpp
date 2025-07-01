#include <iostream>
#include <vector>
#include <stdexcept>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

// Constructor implementation
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

void Parser::parseProgram() {
    std::cout << "[Parser] Entrando a parseProgram()" << std::endl;
    parseListaBloques();
    //consume(TokenType::END_OF_FILE, "se esperaba fin de archivo");
    std::cout << "\n[Parser] Analisis sintactico completado exitosamente.\n";
}

Token& Parser::peek() { return const_cast<Token&>(tokens[current]); }
Token& Parser::previous() { return const_cast<Token&>(tokens[current - 1]); }
bool Parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }
bool Parser::check(TokenType type) { return !isAtEnd() && peek().type == type; }

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token& Parser::advance() {
    if (!isAtEnd()) current++;
    std::cout << "[Parser] Avanzando a token: " << previous().lexeme << " (" << tokenTypeToString(peek().type)  << ") en linea " << previous().line << std::endl;
    return previous();
}

Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        std::cout << "[Parser] Consumiendo token: \"" << tokenTypeToString(peek().type) << ") en linea " << peek().line << std::endl;
        return advance();
    }
    throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Error de sintaxis: " + message);
}

void Parser::parseListaBloques() {
    std::cout << "[Parser] Entrando a parseListaBloques()" << std::endl;
    while (!isAtEnd()) {
        if (check(TokenType::FUNCION)) {
            parseFuncion();
        } else if (inicioDeBloque()) {
            parseBloque();
        } else {
            break;
        }
    }
}

void Parser::parseFuncion() {
    std::cout << "[Parser] Entrando a parseFuncion()" << std::endl;
    consume(TokenType::FUNCION, "se esperaba 'funcion'");
    Token& name = consume(TokenType::IDENTIFIER, "se esperaba nombre de funcion");
    consume(TokenType::LEFT_PAREN, "se esperaba '('");
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    if (name.lexeme == "configuracion") {
        std::cout << "void setup() {\n";
    } else if (name.lexeme == "principal") {
        std::cout << "void loop() {\n";
    } else {
        std::cout << "void " << name.lexeme << "() {\n";
    }
    parseBloque();
    consume(TokenType::FIN, "se esperaba 'fin' al cerrar funcion");
    std::cout << "}\n";
}

void Parser::parseBloque() {
    std::cout << "[Parser] Entrando a parseBloque()" << std::endl;
    while (inicioDeBloque()) {
        if (check(TokenType::MIENTRAS) || check(TokenType::SI) || check(TokenType::POR)) {
            parseControl();
        } else {
            parseInstruccion();
            consume(TokenType::SEMICOLON, "se esperaba ';'");
        }
    }
}

void Parser::parseInstruccion() {
    std::cout << "[Parser] Entrando a parseInstruccion()" << std::endl;
    if (match(TokenType::DEFINIR_PIN)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '(' tras definir_pin");
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::COMMA, "se esperaba ','");
        Token& num = consume(TokenType::NUMBER, "se esperaba numero");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  const int " << id.lexeme << " = " << num.lexeme << ";\n";
    } else if (match(TokenType::SALIDA_KW)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('" );
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  pinMode(" << id.lexeme << ", OUTPUT);\n";
    } else if (match(TokenType::ENTRADA_KW)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('" );
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  pinMode(" << id.lexeme << ", INPUT);\n";
    } else if (match(TokenType::PRENDER)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('" );
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  digitalWrite(" << id.lexeme << ", HIGH);\n";
    } else if (match(TokenType::APAGAR)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('" );
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  digitalWrite(" << id.lexeme << ", LOW);\n";
    } else if (match(TokenType::ESPERAR)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('" );
        Token& num = consume(TokenType::NUMBER, "se esperaba número");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "  delay(" << num.lexeme << ");\n";
    } else if (checkTipoDeclaracion()) {
        parseDeclaracion();
    } else if (check(TokenType::IDENTIFIER)) {
        parseAsignacion();
    } else {
        throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Instruccion no valida");
    }
}

void Parser::parseDeclaracion() {
    std::cout << "[Parser] Entrando a parseDeclaracion()" << std::endl;
    Token& tipo = previous(); // tipo token (ya avanzado)
    Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
    consume(TokenType::ASSIGN, "se esperaba '='");
    // Imprimir tipo de variable
    std::string ctype;
    if (tipo.type == TokenType::ENTERO) ctype = "int";
    else if (tipo.type == TokenType::DECIMAL) ctype = "float";
    else if (tipo.type == TokenType::CADENA) ctype = "String";
    else if (tipo.type == TokenType::BOOLEANO) ctype = "bool";
    else ctype = "auto";
    std::cout << "  " << ctype << " " << id.lexeme << " = ";
    parseExpresion();
    std::cout << ";\n";
}

void Parser::parseAsignacion() {
    std::cout << "[Parser] Entrando a parseAsignacion()" << std::endl;
    Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
    consume(TokenType::ASSIGN, "se esperaba '='");
    std::cout << "  " << id.lexeme << " = ";
    parseExpresion();
    std::cout << ";\n";
}

void Parser::parseExpresion() {
    std::cout << "[Parser] Entrando a parseExpresion()" << std::endl;
    parseFactor();
    while (check(TokenType::OPERATOR)) {
        std::cout << peek().lexeme;
        advance();
        parseFactor();
    }
}

void Parser::parseFactor() {
    std::cout << "[Parser] Entrando a parseFactor()" << std::endl;
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFIER) || match(TokenType::STRING_LITERAL) ||
        match(TokenType::VERDADERO) || match(TokenType::FALSO)) {
        std::cout << previous().lexeme;
        return;
    }
    if (match(TokenType::LEER)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('");
        Token& id = consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << "digitalRead(" << id.lexeme << ")";
        return;
    }
    throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Factor no reconocido en expresion");
}

void Parser::parseControl() {
    std::cout << "[Parser] Entrando a parseControl()" << std::endl;
    if (match(TokenType::MIENTRAS)) parseMientras();
    else if (match(TokenType::SI)) parseSi();
    else if (match(TokenType::POR)) parsePor();
}

void Parser::parseMientras() {
    std::cout << "[Parser] Entrando a parseMientras()" << std::endl;
    consume(TokenType::LEFT_PAREN, "se esperaba '(' tras mientras");
    std::cout << "  while (";
    parseExpresion();
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    std::cout << ") {\n";
    parseBloque();
    consume(TokenType::FIN_MIENTRAS, "se esperaba 'fin_mientras'");
    std::cout << "  }\n";
}

void Parser::parseSi() {
    std::cout << "[Parser] Entrando a parseSi()" << std::endl;
    consume(TokenType::LEFT_PAREN, "se esperaba '(' tras si");
    std::cout << "  if (";
    parseExpresion();
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    std::cout << ") {\n";
    parseBloque();
    parseSinoLista();
    std::cout << "  }\n";
}

void Parser::parseSinoLista() {
    std::cout << "[Parser] Entrando a parseSinoLista()" << std::endl;
    while (match(TokenType::SINO_SI)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '(' tras sino si");
        std::cout << "  else if (";
        parseExpresion();
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        std::cout << ") {\n";
        parseBloque();
    }
    if (match(TokenType::SINO)) {
        std::cout << "  else {\n";
        parseBloque();
    }
}

void Parser::parsePor() {
    std::cout << "[Parser] Entrando a parsePor()" << std::endl;
    consume(TokenType::LEFT_PAREN, "se esperaba '(' tras por");
    consume(TokenType::IDENTIFIER, "se esperaba identificador");
    consume(TokenType::ASSIGN, "se esperaba '='");
    parseExpresion();
    consume(TokenType::SEMICOLON, "se esperaba ';'");
    parseExpresion();
    consume(TokenType::SEMICOLON, "se esperaba ';'");
    parseAsignacion();
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    parseBloque();
    consume(TokenType::FIN_POR, "se esperaba 'fin_por'");
}

bool Parser::inicioDeBloque() {
    return check(TokenType::DEFINIR_PIN) || check(TokenType::SALIDA_KW) ||
           check(TokenType::ENTRADA_KW) || check(TokenType::PRENDER) ||
           check(TokenType::APAGAR) || check(TokenType::ESPERAR) ||
           check(TokenType::IDENTIFIER) || checkTipoDeclaracion() ||
           check(TokenType::MIENTRAS) || check(TokenType::SI) || check(TokenType::POR);
}

bool Parser::checkTipoDeclaracion() {
    return check(TokenType::ENTERO) || check(TokenType::DECIMAL) ||
           check(TokenType::CADENA) || check(TokenType::BOOLEANO);
}