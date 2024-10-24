#include <SFML/Graphics.hpp>
using namespace sf;
int main() {
    // Crear la ventana
    RenderWindow window(VideoMode(800, 600), "Mapa Turístico");

    // Cargar la textura del mapa desde un archivo
    Texture mapaTexture;
    if (!mapaTexture.loadFromFile("imagen/mapa2.png")) {
        return -1; // Verifica si la imagen se cargó correctamente
    }

    // Crear un sprite para dibujar la textura
    Sprite mapaSprite;
    mapaSprite.setTexture(mapaTexture);

    // Bucle principal de la ventana
    while (window.isOpen()) {
        // Procesar eventos
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Limpiar la ventana
        window.clear();

        // Dibujar el sprite del mapa
        window.draw(mapaSprite);

        // Mostrar el contenido dibujado en la ventana
        window.display();
    }

    return 0;
}