
#include <SFML/Graphics.hpp>
#include "Mapa.h"

int main() {
   
    std::string mapaFile = "imagen/gta.jpg";
    std::string fontFile = "fuentes/NostalgicWhispers-Regular.ttf";

    try {
       
        MapaTuristico mapa(mapaFile, fontFile);

       
        mapa.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
