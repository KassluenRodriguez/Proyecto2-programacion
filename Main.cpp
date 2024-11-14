#include "Mapa.h"
#include <iostream>

int main() {
    try {
        MapaTuristico mapa("imagen/gta.jpg", "fuentes/NostalgicWhispers-Regular.ttf");
        mapa.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;

}