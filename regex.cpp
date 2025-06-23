#include <iostream>
#include <fstream>
#include <string>
#include <regex>

std::string translateLine(const std::string& line) {
    std::smatch match;

    std::string trimmed = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");

    if (trimmed.empty()) return "";

    if (trimmed == "FUNCION CONFIGURACION()") return "void setup() {";
    if (trimmed == "FUNCION PRINCIPAL()") return "void loop() {";
    if (trimmed == "FIN") return "}";

    if (trimmed.find("ENTERO ") == 0)
        return std::regex_replace(trimmed, std::regex("ENTERO"), "int");

    if (std::regex_match(trimmed, match, std::regex(R"(SALIDA\((\w+)\);)")))
        return "  pinMode(" + match[1].str() + ", OUTPUT);";

    if (std::regex_match(trimmed, match, std::regex(R"(PRENDER\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", HIGH);";

    if (std::regex_match(trimmed, match, std::regex(R"(APAGAR\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", LOW);";

    if (std::regex_match(trimmed, match, std::regex(R"(ESPERAR\((\d+)\);)")))
        return "  delay(" + match[1].str() + ");";

    return "  // Unknown: " + trimmed;
}

int main() {
    std::ifstream input("example.ar");
    std::ofstream output("generated.ino");

    if (!input || !output) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        output << translateLine(line) << '\n';
    }

    std::cout << "Compiled to generated.ino\n";
    return 0;
}
