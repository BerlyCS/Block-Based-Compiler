#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "No se puede abrir el archivo " << argv[1] << "\n";
        return 1;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    std::cout<<"Lexico\n";
    auto tokens = lexer.scanTokens();
for (const auto &token : tokens) {
  std::cout << "[Linea " << token.line << "] "
            << tokenTypeToString(token.type)
            << "-->" << token.lexeme << '\n';
}
    if (!lexer.getErrors().empty()) {
        std::cout<<"se encontraron errores\n";
        for (const auto& err : lexer.getErrors()) {
            std::cerr << err << '\n';
        }
        return 1;
    }
    ///*
    std::cout<<"parseando...\n";
    Parser parser(tokens);
    try {
        parser.parseProgram();
        std::cout << "Compilacion exitosa!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
    //*/
}