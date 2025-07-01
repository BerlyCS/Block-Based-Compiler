#pragma once
#include <vector>
#include <string>
#include "token.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    void parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t current;

    Token& peek();
    Token& previous();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(TokenType type);
    Token& advance();
    Token& consume(TokenType type, const std::string& message);

    void parseListaBloques();
    void parseFuncion();
    void parseBloque();
    void parseInstruccion();
    void parseDeclaracion();
    void parseAsignacion();
    void parseExpresion();
    void parseFactor();
    void parseControl();
    void parseMientras();
    void parseSi();
    void parseSinoLista();
    void parsePor();

    bool inicioDeBloque();
    bool checkTipoDeclaracion();
};
