#pragma once
#include <string>
#include <vector>
#include "token.h"

std::string tokenTypeToString(TokenType type);
TokenType identifierType(const std::string &text);

class Lexer
{
public:
    Lexer(const std::string &source);
    std::vector<Token> scanTokens();
    const std::vector<std::string> &getErrors() const;

private:
    const std::string source;
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    bool isAtEnd() const { return current >= source.size(); }
    char advance() { return source[current++]; }
    char peek() const { return isAtEnd() ? '\0' : source[current]; }
    char peekNext() const {
        return (current + 1 >= source.size()) ? '\0' : source[current + 1];
    }

    void addToken(TokenType type);
    void error(const std::string &message);
    void scanToken();
    bool matchChar(char expected);
    void identifier();
    void number();
    void stringLiteral();
};
