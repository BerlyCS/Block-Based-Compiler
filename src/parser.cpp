#include <iostream>
#include <vector>
#include <stdexcept>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

// Constructor implementation
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

void Parser::parseProgram() {
    parseListaBloques();
    consume(TokenType::END_OF_FILE, "se esperaba fin de archivo");
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
    return previous();
}

Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Error de sintaxis: " + message);
}

void Parser::parseListaBloques() {
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
    consume(TokenType::FUNCION, "se esperaba 'funcion'");
    consume(TokenType::IDENTIFIER, "se esperaba nombre de funcion");
    consume(TokenType::LEFT_PAREN, "se esperaba '('");
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    parseBloque();
    consume(TokenType::FIN, "se esperaba 'fin' al cerrar funcion");
}

void Parser::parseBloque() {
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
    if (match(TokenType::DEFINIR_PIN)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '(' tras definir_pin");
        consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::COMMA, "se esperaba ','");
        consume(TokenType::NUMBER, "se esperaba numero");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    } else if (match(TokenType::SALIDA_KW) || match(TokenType::ENTRADA_KW) ||
               match(TokenType::PRENDER) || match(TokenType::APAGAR)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('");
        consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    } else if (match(TokenType::ESPERAR)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('");
        consume(TokenType::NUMBER, "se esperaba número");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    } else if (checkTipoDeclaracion()) {
        parseDeclaracion();
    } else if (check(TokenType::IDENTIFIER)) {
        parseAsignacion();
    } else {
        throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Instruccion no valida");
    }
}

void Parser::parseDeclaracion() {
    advance(); // tipo
    consume(TokenType::IDENTIFIER, "se esperaba identificador");
    consume(TokenType::ASSIGN, "se esperaba '='");
    parseExpresion();
}

void Parser::parseAsignacion() {
    consume(TokenType::IDENTIFIER, "se esperaba identificador");
    consume(TokenType::ASSIGN, "se esperaba '='");
    parseExpresion();
}

void Parser::parseExpresion() {
    parseFactor();
    while (check(TokenType::OPERATOR)) {
        advance();
        parseFactor();
    }
}

void Parser::parseFactor() {
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFIER) || match(TokenType::STRING_LITERAL) ||
        match(TokenType::VERDADERO) || match(TokenType::FALSO)) {
        return;
    }
    if (match(TokenType::LEER)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '('");
        consume(TokenType::IDENTIFIER, "se esperaba identificador");
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        return;
    }
    throw std::runtime_error("[Linea " + std::to_string(peek().line) + "] Factor no reconocido en expresion");
}

void Parser::parseControl() {
    if (match(TokenType::MIENTRAS)) parseMientras();
    else if (match(TokenType::SI)) parseSi();
    else if (match(TokenType::POR)) parsePor();
}

void Parser::parseMientras() {
    consume(TokenType::LEFT_PAREN, "se esperaba '(' tras mientras");
    parseExpresion();
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    parseBloque();
    consume(TokenType::FIN_MIENTRAS, "se esperaba 'fin_mientras'");
}

void Parser::parseSi() {
    consume(TokenType::LEFT_PAREN, "se esperaba '(' tras si");
    parseExpresion();
    consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
    parseBloque();
    parseSinoLista();
}

void Parser::parseSinoLista() {
    while (match(TokenType::SINO_SI)) {
        consume(TokenType::LEFT_PAREN, "se esperaba '(' tras sino si");
        parseExpresion();
        consume(TokenType::RIGHT_PAREN, "se esperaba ')'");
        parseBloque();
    }
    if (match(TokenType::SINO)) {
        parseBloque();
    }
}

void Parser::parsePor() {
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