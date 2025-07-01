#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "lexer.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::COMMENT:
        return "COMENTARIO";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::FUNCION:
        return "FUNCION";
    case TokenType::CONFIGURACION:
        return "CONFIGURACION";
    case TokenType::PRINCIPAL:
        return "PRINCIPAL";
    case TokenType::FIN:
        return "FIN";
    case TokenType::FIN_MIENTRAS:
        return "FIN_MIENTRAS";
    case TokenType::FIN_POR:
        return "FIN_POR";
    case TokenType::HECHO:
        return "HECHO";
    case TokenType::ENTERO:
        return "ENTERO";
    case TokenType::DECIMAL:
        return "DECIMAL";
    case TokenType::CADENA:
        return "CADENA";
    case TokenType::BOOLEANO:
        return "BOOLEANO";
    case TokenType::DEFINIR_PIN:
        return "DEFINIR_PIN";
    case TokenType::SALIDA_KW:
        return "SALIDA";
    case TokenType::ENTRADA_KW:
        return "ENTRADA";
    case TokenType::PRENDER:
        return "PRENDER";
    case TokenType::APAGAR:
        return "APAGAR";
    case TokenType::LEER:
        return "LEER";
    case TokenType::ESPERAR:
        return "ESPERAR";
    case TokenType::MIENTRAS:
        return "MIENTRAS";
    case TokenType::SI:
        return "SI";
    case TokenType::SINO:
        return "SINO";
    case TokenType::SINO_SI:
        return "SINO_SI";
    case TokenType::POR:
        return "POR";
    case TokenType::VERDADERO:
        return "VERDADERO";
    case TokenType::FALSO:
        return "FALSO";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::OPERATOR:
        return "OPERATOR";
    case TokenType::STRING_LITERAL:
        return "STRING_LITERAL";
    case TokenType::END_OF_FILE:
        return "EOF";
    case TokenType::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

Lexer::Lexer(const std::string &source) : source(source) {}

std::vector<Token> Lexer::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}

const std::vector<std::string> &Lexer::getErrors() const { return errors; }

void Lexer::addToken(TokenType type)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, line});
}

void Lexer::error(const std::string &message)
{
    std::ostringstream oss;
    oss << "[Linea " << line << "] Error lexico: " << message;
    errors.push_back(oss.str());
}

void Lexer::scanToken()
{
    char c = advance();
    switch (c)
    {
    case '<':
        if (matchChar('='))
            addToken(TokenType::OPERATOR);
        else
            addToken(TokenType::OPERATOR);
        break;
    case '>':
        if (matchChar('='))
            addToken(TokenType::OPERATOR);
        else
            addToken(TokenType::OPERATOR);
        break;
    case '=':
        if (matchChar('='))
            addToken(TokenType::OPERATOR);
        else
            addToken(TokenType::ASSIGN);
        break;
    case '!':
        if (matchChar('='))
            addToken(TokenType::OPERATOR);
        else
            error("Caracter inesperado '!' (te referias a '!=')");
        break;
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '+':
    case '-':
    case '*':
    case '/':
        addToken(TokenType::OPERATOR);
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;
    case '"':
        stringLiteral();
        break;
    case '#':
        while (peek() != '\n' && !isAtEnd())
            advance();
        break;
    default:
        if (std::isalpha(c) || c == '_')
            identifier();
        else if (std::isdigit(c))
            number();
        else
        {
            std::string s(1, c);
            error("Caracter inesperado '" + s + "'");
        }
    }
}

bool Lexer::matchChar(char expected)
{
    if (isAtEnd())
        return false;
    if (source[current] != expected)
        return false;
    current++;
    return true;
}

void Lexer::identifier()
{
    while (std::isalnum(peek()) || peek() == '_')
        advance();
    std::string text = source.substr(start, current - start);
    TokenType type = identifierType(text);
    addToken(type);
}

void Lexer::number()
{
    while (std::isdigit(peek()))
        advance();
    if (peek() == '.' && std::isdigit(peekNext()))
    {
        advance();
        while (std::isdigit(peek()))
            advance();
    }
    addToken(TokenType::NUMBER);
}

void Lexer::stringLiteral()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }
    if (isAtEnd())
    {
        error("Unterminated string literal");
        return;
    }
    advance(); // closing quote
    addToken(TokenType::STRING_LITERAL);
}

TokenType identifierType(const std::string &text)
{
    static std::unordered_map<std::string, TokenType> keywords = {
        {"funcion", TokenType::FUNCION},
        {"configuracion", TokenType::CONFIGURACION},
        {"principal", TokenType::PRINCIPAL},
        {"fin", TokenType::FIN},
        {"fin_mientras", TokenType::FIN_MIENTRAS},
        {"fin_por", TokenType::FIN_POR},
        {"hecho", TokenType::HECHO},
        {"entero", TokenType::ENTERO},
        {"decimal", TokenType::DECIMAL},
        {"cadena", TokenType::CADENA},
        {"booleano", TokenType::BOOLEANO},
        {"definir_pin", TokenType::DEFINIR_PIN},
        {"salida", TokenType::SALIDA_KW},
        {"entrada", TokenType::ENTRADA_KW},
        {"prender", TokenType::PRENDER},
        {"apagar", TokenType::APAGAR},
        {"leer", TokenType::LEER},
        {"esperar", TokenType::ESPERAR},
        {"mientras", TokenType::MIENTRAS},
        {"si", TokenType::SI},
        {"sino", TokenType::SINO},
        {"sino_si", TokenType::SINO_SI},
        {"por", TokenType::POR},
        {"verdadero", TokenType::VERDADERO},
        {"falso", TokenType::FALSO},
    };
    auto it = keywords.find(text);
    if (it != keywords.end())
        return it->second;
    return TokenType::IDENTIFIER;
}
/*
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "No se puede abrir el archivo "<< argv[1]<<"\n";
    return 1;
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();

  Lexer lexer(source);
  auto tokens = lexer.scanTokens();
  if (lexer.getErrors().size() == 0) {
    std::cerr<<"No se encontraron errores\n";
  }
  for (const auto &err : lexer.getErrors()) {
    std::cerr << err << '\n';
  }

for (const auto &token : tokens) {
  std::cout << "[Linea " << token.line << "] "
            << tokenTypeToString(token.type)
            << "-->" << token.lexeme << '\n';
}
  return 0;
}
*/