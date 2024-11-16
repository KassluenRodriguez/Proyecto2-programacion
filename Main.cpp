
#include <SFML/Graphics.hpp>
#include "Mapa.h"

int main() {
    // Ruta a los archivos del mapa y la fuente
    std::string mapaFile = "imagen/gta.jpg";
    std::string fontFile = "fuentes/NostalgicWhispers-Regular.ttf";

    try {
        // Crear una instancia de MapaTuristico
        MapaTuristico mapa(mapaFile, fontFile);

        // Ejecutar el programa
        mapa.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
