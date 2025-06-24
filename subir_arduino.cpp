#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
  if(argc != 2){
    std::cerr << "Uso: " << argv[0] << " <carpeta_archivo.ino>\n";
  }
  const std::string fqbn = "arduino:avr:uno";
  const std::string port = "/dev/ttyACM0"; // Cambia si tu puerto es otro
  std::string fileName = argv[1];
  const std::string sketchPath = "./" + fileName;

  std::string compileCmd = "arduino-cli compile --fqbn " + fqbn + " " + sketchPath;
  std::string uploadCmd = "arduino-cli upload -p " + port + " --fqbn " + fqbn + " " + sketchPath;

  std::cout << "Compilando...\n";
  int comp = system(compileCmd.c_str());
  if (comp != 0) {
    std::cerr << "Error al compilar.\n";
    return 1;
  }

  std::cout << "Subiendo...\n";
  int upl = system(uploadCmd.c_str());
  if (upl != 0) {
    std::cerr << "Error al subir.\n";
    return 1;
  }

  std::cout << "¡Código subido con éxito!\n";
  return 0;
}

