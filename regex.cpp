#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string translateLine(const std::string& line) {
    std::smatch match;
    std::string trimmed = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");

    if (trimmed.empty() || trimmed[0] == '#') return "";

    // Funciones
    if (trimmed == "FUNCION CONFIGURACION()") return "void setup() {";
    if (trimmed == "FUNCION PRINCIPAL()") return "void loop() {";

    // Fin de bloques
    if (trimmed == "FIN" || trimmed == "FIN_MIENTRAS" || trimmed == "FIN_POR" || trimmed == "HECHO")
        return "}";

    // Tipos
    if (trimmed.find("ENTERO ") == 0)
        return "  int " + trimmed.substr(7);
    if (trimmed.find("DECIMAL ") == 0)
        return "  float " + trimmed.substr(8);
    if (trimmed.find("CADENA ") == 0)
        return "  String " + trimmed.substr(7);
    if (trimmed.find("BOOLEANO ") == 0)
        return "  bool " + trimmed.substr(9);

    // Constantes booleanas
    trimmed = std::regex_replace(trimmed, std::regex("VERDADERO"), "true");
    trimmed = std::regex_replace(trimmed, std::regex("FALSO"), "false");

    // Definición de pines
    if (std::regex_match(trimmed, match, std::regex(R"(DEFINIR_PIN\((\w+),\s*(\d+)\);)")))
        return "  const int " + match[1].str() + " = " + match[2].str() + ";";

    // Modos de pin
    if (std::regex_match(trimmed, match, std::regex(R"(SALIDA\((\w+)\);)")))
        return "  pinMode(" + match[1].str() + ", OUTPUT);";
    if (std::regex_match(trimmed, match, std::regex(R"(ENTRADA\((\w+)\);)")))
        return "  pinMode(" + match[1].str() + ", INPUT);";

    // Lectura y escritura digital
    if (std::regex_match(trimmed, match, std::regex(R"(LEER\((\w+)\))")))
        return "digitalRead(" + match[1].str() + ")";
    if (std::regex_match(trimmed, match, std::regex(R"(PRENDER\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", HIGH);";
    if (std::regex_match(trimmed, match, std::regex(R"(APAGAR\((\w+)\);)")))
        return "  digitalWrite(" + match[1].str() + ", LOW);";
    if (std::regex_match(trimmed, match, std::regex(R"(ESPERAR\((\d+)\);)")))
        return "  delay(" + match[1].str() + ");";

    // Control de flujo
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

    // Bucle POR (declaración implícita de ENTERO)
    if (std::regex_match(trimmed, match, std::regex(R"(POR\s*\(\s*(\w+)\s*=\s*[^;]+;[^;]+;[^)]+\))"))) {
        std::string forBody = trimmed.substr(4, trimmed.size() - 5); // sin "POR(" y ")"
        std::vector<std::string> parts;
        std::stringstream ss(forBody);
        std::string item;
        while (std::getline(ss, item, ';')) {
            parts.push_back(std::regex_replace(item, std::regex("^\\s+|\\s+$"), ""));
        }
        if (parts.size() == 3)
            return "  for (int " + parts[0] + "; " + parts[1] + "; " + parts[2] + ") {";
        else
            return "  // Error: bucle POR mal formado → " + trimmed;
    }

    // Asignaciones (terminadas en ;)
    if (trimmed.find('=') != std::string::npos && trimmed.back() == ';')
        return "  " + trimmed;

    // Si no se reconoció la instrucción
    return "  // Error: instrucción no reconocida o uso incorrecto → " + trimmed;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada.ar> <archivo_salida.ino>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    std::string nameOutput = argv[2];
    fs::create_directory(nameOutput);
    std::ofstream output(nameOutput+"/"+nameOutput+".ino");

    if (!input) {
        std::cerr << "Error: no se pudo abrir el archivo de entrada.\n";
        return 1;
    }
    if (!output) {
        std::cerr << "Error: no se pudo crear el archivo de salida.\n";
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::string translated = translateLine(line);
        if (!translated.empty())
            output << translated << '\n';
    }

    std::cout << "Compilación completada. Código generado en: " << argv[2] << '\n';
    return 0;
}
