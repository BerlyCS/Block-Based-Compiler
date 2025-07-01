#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string translateline(const std::string& line) {
    std::smatch match;
    std::string trimmed = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");

    // ignorar líneas vacías o comentarios
    if (trimmed.empty() || trimmed[0] == '#') return "";

    // funciones
    if (trimmed == "funcion configuracion()") return "void setup() {";
    if (trimmed == "funcion principal()") return "void loop() {";

    // fin de bloques
    if (trimmed == "fin" || trimmed == "fin_mientras" || trimmed == "fin_por" || trimmed == "hecho")
        return "}";

    // tipos primitivos
    if (trimmed.find("entero ") == 0)
        return "  int " + trimmed.substr(7);
    if (trimmed.find("decimal ") == 0)
        return "  float " + trimmed.substr(8);
    if (trimmed.find("cadena ") == 0)
        return "  String " + trimmed.substr(7);
    if (trimmed.find("booleano ") == 0)
        return "  bool " + trimmed.substr(9);

    // constantes booleanas
    trimmed = std::regex_replace(trimmed, std::regex("verdadero", std::regex_constants::icase), "true");
    trimmed = std::regex_replace(trimmed, std::regex("falso", std::regex_constants::icase), "false");

    // definición de pines
    if (std::regex_match(trimmed, match, std::regex(R"(definir_pin\((\w+),\s*(\d+)\);)", std::regex_constants::icase)))
        return "  const int " + match[1].str() + " = " + match[2].str() + ";";

    // configuración de pines
    if (std::regex_match(trimmed, match, std::regex(R"(salida\((\w+)\);)", std::regex_constants::icase)))
        return "  pinMode(" + match[1].str() + ", OUTPUT);";
    if (std::regex_match(trimmed, match, std::regex(R"(entrada\((\w+)\);)", std::regex_constants::icase)))
        return "  pinMode(" + match[1].str() + ", INPUT);";

    // lectura y escritura
    if (std::regex_match(trimmed, match, std::regex(R"(leer\((\w+)\))", std::regex_constants::icase)))
        return "digitalRead(" + match[1].str() + ")";
    if (std::regex_match(trimmed, match, std::regex(R"(prender\((\w+)\);)", std::regex_constants::icase)))
        return "  digitalWrite(" + match[1].str() + ", HIGH);";
    if (std::regex_match(trimmed, match, std::regex(R"(apagar\((\w+)\);)", std::regex_constants::icase)))
        return "  digitalWrite(" + match[1].str() + ", LOW);";
    if (std::regex_match(trimmed, match, std::regex(R"(esperar\((\d+)\);)", std::regex_constants::icase)))
        return "  delay(" + match[1].str() + ");";

    // condicionales
    if (std::regex_match(trimmed, match, std::regex(R"(si\s*\((.+)\))", std::regex_constants::icase)))
        return "  if (" + match[1].str() + ") {";
    if (trimmed.find("sino si") == 0) {
        std::string cond = trimmed.substr(8);
        return "  else if (" + cond + ") {";
    }
    if (trimmed == "sino")
        return "  else {";

    // bucle mientras
    if (std::regex_match(trimmed, match, std::regex(R"(mientras\s*\((.+)\))", std::regex_constants::icase)))
        return "  while (" + match[1].str() + ") {";

    // bucle por
    if (std::regex_match(trimmed, match, std::regex(R"(por\s*\(\s*(\w+)\s*=\s*[^;]+;[^;]+;[^)]+\))", std::regex_constants::icase))) {
        std::string forbody = trimmed.substr(4, trimmed.size() - 5); // eliminar POR( y )
        std::vector<std::string> parts;
        std::stringstream ss(forbody);
        std::string item;
        while (std::getline(ss, item, ';')) {
            parts.push_back(std::regex_replace(item, std::regex("^\\s+|\\s+$"), ""));
        }
        if (parts.size() == 3)
            return "  for (int " + parts[0] + "; " + parts[1] + "; " + parts[2] + ") {";
        else
            return "  // error: bucle por mal formado → " + trimmed;
    }

    // asignaciones simples
    if (trimmed.find('=') != std::string::npos && trimmed.back() == ';')
        return "  " + trimmed;

    // instrucción no reconocida
    return "  // error: instrucción no reconocida → " + trimmed;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "uso: " << argv[0] << " <archivo_entrada.ar> <carpeta_salida>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    std::string outputdir = argv[2];
    fs::create_directory(outputdir);
    std::ofstream output(outputdir + "/" + outputdir + ".ino");

    if (!input) {
        std::cerr << "error: no se pudo abrir el archivo de entrada.\n";
        return 1;
    }
    if (!output) {
        std::cerr << "error: no se pudo crear el archivo de salida.\n";
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::string translated = translateline(line);
        if (!translated.empty())
            output << translated << '\n';
    }

    std::cout << "compilación completada. código generado en: " << outputdir << '\n';
    return 0;
}
