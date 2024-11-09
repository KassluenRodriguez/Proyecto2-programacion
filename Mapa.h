#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Rutas.h"

using namespace std;
using namespace sf;

class MapaTuristico {
public:
    MapaTuristico(const string& mapaFile, const string& fontFile);

    // Método principal que ejecutará el mapa
    void run();

private:
    
    void manejarEventos();
    void dibujar();
    void agregarPunto(const Vector2f& pos);
    string mostrarCuadroDeDialogo();
    void agregarRuta();  

   
    RenderWindow window;
    Texture mapaTexture;
    Sprite mapaSprite;
    Font font;
    Color colorSeleccionado;

    Ruta rutas;  // Esta es la clase que manejará las rutas, almacenando los puntos turísticos

    // Para la paleta de colores de selección
    RectangleShape paleta[6];
    Color colores[6];
    Text nombresRutas[100];

    int contadorPuntos = 1; 
    int nombresRutasCount = 0;
    int contadorRutas = 1; 
    RectangleShape botonAgregarRuta;  // El botón para agregar una nueva ruta
};

MapaTuristico::MapaTuristico(const string& mapaFile, const string& fontFile)
    : window(VideoMode(1000, 700), "Mapa Turístico"), colorSeleccionado(Color::Red) {

    if (!mapaTexture.loadFromFile(mapaFile)) {
        throw runtime_error("No se pudo cargar la textura del mapa");
    }
    mapaSprite.setTexture(mapaTexture);

    if (!font.loadFromFile(fontFile)) {
        throw runtime_error("No se pudo cargar la fuente");
    }

    // Definimos los colores de la paleta
    colores[0] = Color::Red;
    colores[1] = Color::Green;
    colores[2] = Color::Blue;
    colores[3] = Color::Yellow;
    colores[4] = Color::Magenta;
    colores[5] = Color::Cyan;

    //crear boton de colores para cambiar los puntos
    for (int i = 0; i < 6; ++i) {
        paleta[i].setSize(Vector2f(30, 30));
        paleta[i].setFillColor(colores[i]);
        paleta[i].setPosition(10 + i * 40, 660);
    }

    // Crear el botón para agregar una nueva ruta
    botonAgregarRuta.setSize(Vector2f(150, 40));
    botonAgregarRuta.setFillColor(Color::White);
    botonAgregarRuta.setPosition(250, window.getSize().y - 50);  // Ubicación del botón 
}

void MapaTuristico::run() {
    while (window.isOpen()) {
        manejarEventos();
        dibujar();
    }
}

void MapaTuristico::manejarEventos() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);

            // Detectar si el usuario ha hecho clic en la paleta de colores
            for (int i = 0; i < 6; ++i) {
                if (paleta[i].getGlobalBounds().contains(clickPos)) {
                    colorSeleccionado = colores[i];
                    return;
                }
            }

            // Si el clic es sobre la paleta de color o los puntos, agregar punto
            if (clickPos.y < 550) {
                agregarPunto(clickPos);
            }

            // Verificar si se hizo clic en el botón de agregar ruta
            if (botonAgregarRuta.getGlobalBounds().contains(clickPos)) {
                agregarRuta();
            }
        }
    }
}

void MapaTuristico::dibujar() {
    window.clear();
    window.draw(mapaSprite);

    rutas.dibujar(window);      // Dibuja los puntos
    rutas.unirPuntos(window);   // Dibuja las líneas entre los puntos

    // Dibuja la paleta de colores
    for (int i = 0; i < 6; ++i) {
        window.draw(paleta[i]);
    }

    // Dibuja los nombres de las rutas en la esquina inferior izquierda
    for (int i = 0; i < nombresRutasCount; ++i) {
        window.draw(nombresRutas[i]);
    }

    // Dibuja el botón de agregar ruta
    window.draw(botonAgregarRuta);

    // Muestra el texto en el botón
    Text textoBoton("Agregar Ruta", font, 18);
    textoBoton.setFillColor(Color::Black);
    textoBoton.setPosition(botonAgregarRuta.getPosition().x + 10, botonAgregarRuta.getPosition().y + 10);
    window.draw(textoBoton);

    window.display();
}



string MapaTuristico::mostrarCuadroDeDialogo() {
    RenderWindow inputWindow(VideoMode(300, 100), "Nombre del Punto");
    Text textoInput("", font, 20);
    textoInput.setPosition(10, 10);
    textoInput.setFillColor(Color::Black);

    string input = "Punto " + to_string(contadorPuntos) + " : ";
    while (inputWindow.isOpen()) {
        Event event;
        while (inputWindow.pollEvent(event)) {
            if (event.type == Event::Closed) {
                inputWindow.close();
            }
            else if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                }
                else if (event.text.unicode == '\r') {
                    inputWindow.close();
                }
                else if (event.text.unicode < 128) {
                    input += static_cast<char>(event.text.unicode);
                }
            }
        }
        textoInput.setString("Ruta " +to_string(contadorRutas)+" - " + input);
        inputWindow.clear(Color::White);
        inputWindow.draw(textoInput);
        inputWindow.display();
    }
    return input;
}

void MapaTuristico::agregarPunto(const Vector2f& pos) {
    string nombrePunto = mostrarCuadroDeDialogo();

    
    if (nombrePunto.empty()) {
        nombrePunto = "Punto " + to_string(contadorPuntos);  
    }

    PuntoConNombre nuevoPunto(pos, colorSeleccionado, nombrePunto, font);
    rutas.agregarPunto(nuevoPunto);

      contadorPuntos++;
}

void MapaTuristico::agregarRuta() {
   
    string nombreRuta = "Ruta " + to_string(contadorRutas);

    // Crear un nuevo texto para mostrar el nombre de la ruta en la esquina inferior izquierda
    if (nombresRutasCount < 100) {
        Text rutaTexto(nombreRuta, font, 14);
        rutaTexto.setFillColor(Color::Black);

        // Posicionamos las rutas en la esquina inferior izquierda
        float offsetX = 10.0f;
        float offsetY = window.getSize().y - 60.0f - (nombresRutasCount * 20);

        rutaTexto.setPosition(offsetX, offsetY);
        nombresRutas[nombresRutasCount++] = rutaTexto;
    }

   
    contadorRutas++;
}
