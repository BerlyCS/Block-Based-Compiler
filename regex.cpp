#include <iostream>
#include <fstream>
#include <string>
#include <regex>

std::string translateLine(const std::string& line) {
    std::smatch match;
    std::string trimmed = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");
    if (trimmed.empty()) return "";

    // Function Headers
    if (trimmed == "FUNCION CONFIGURACION()") return "void setup() {";
    if (trimmed == "FUNCION PRINCIPAL()") return "void loop() {";

    // Block endings
    if (trimmed == "FIN") return "}";
    if (trimmed == "FIN_MIENTRAS") return "}"; // loop end
    if (trimmed == "FIN_POR") return "}";
    if (trimmed == "HECHO") return "}";

    // Types
    if (trimmed.find("ENTERO ") == 0)
        return "  int " + trimmed.substr(7);
    if (trimmed.find("DECIMAL ") == 0)
        return "  float " + trimmed.substr(8);
    if (trimmed.find("CADENA ") == 0)
        return "  String " + trimmed.substr(7);
    if (trimmed.find("BOOLEANO ") == 0)
        return "  bool " + trimmed.substr(9);

    // Constants
    trimmed = std::regex_replace(trimmed, std::regex("VERDADERO"), "true");
    trimmed = std::regex_replace(trimmed, std::regex("FALSO"), "false");

    // Pin definitions
    if (std::regex_match(trimmed, match, std::regex(R"(DEFINIR_PIN\((\w+),\s*(\d+)\);)")))
        return "  const int " + match[1].str() + " = " + match[2].str() + ";";

    // Pin mode
    if (std::regex_match(trimmed, match, std::regex(R"(SALIDA\((\w+)\);)")))
        return "  pinMode(" + match[1].str() + ", OUTPUT);";
    if (std::regex_match(trimmed, match, std::regex(R"(ENTRADA\((\w+)\);)")))
        return "  pinMode(" + match[1].str() + ", INPUT);";

    // Read/Write
    if (std::regex_match(trimmed, match, std::regex(R"(LEER\((\w+)\))")))
        return "digitalRead(" + match[1].str() + ")";
    if (std::regex_match(trimmed, match, std::regex(R"(PRENDER\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", HIGH);";
    if (std::regex_match(trimmed, match, std::regex(R"(APAGAR\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", LOW);";
    if (std::regex_match(trimmed, match, std::regex(R"(ESPERAR\((\d+)\);)")))
        return "  delay(" + match[1].str() + ");";

    // Control flow
    if (std::regex_match(trimmed, match, std::regex(R"(MIENTRAS\s*\((.+)\))")))
        return "  while (" + match[1].str() + ") {";
    if (std::regex_match(trimmed, match, std::regex(R"(SI\s*\((.+)\))")))
        return "  if (" + match[1].str() + ") {";
    if (trimmed.find("SINO SI") == 0) {
        std::string cond = trimmed.substr(8);
        return "  else if (" + cond + ") {";
    }
    if (trimmed == "SINO")
        return "  else {";

    // FOR loop
    if (std::regex_match(trimmed, match, std::regex(R"(POR\s*\(([^;]+);([^;]+);(.+)\))")))
        return "  for (" + match[1].str() + ";" + match[2].str() + ";" + match[3].str() + ") {";

    // Assignment and arithmetic
    if (trimmed.find("=") != std::string::npos && trimmed.back() == ';')
        return "  " + trimmed;

    // Fallback
    return "  // Unknown: " + trimmed;
}

int main() {
    std::ifstream input("semaforo.ar");
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
