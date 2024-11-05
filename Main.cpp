#include "Mapa.h"
#include <iostream>

int main() {
    try {
        MapaTuristico mapa("imagen/mapa2.jpg", "fuentes/Open_Sans-italic.static");
        mapa.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
