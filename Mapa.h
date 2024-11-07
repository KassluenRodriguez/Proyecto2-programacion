#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Rutas.h"

using namespace std;
using namespace sf;

class MapaTuristico {
public:
    MapaTuristico(const std::string& mapaFile, const std::string& fontFile);
    void run();

private:
    void manejarEventos();
    void dibujar();
    void agregarPunto(const sf::Vector2f& pos);
    std::string mostrarCuadroDeDialogo();

    sf::RenderWindow window;
    sf::Texture mapaTexture;
    sf::Sprite mapaSprite;
    sf::Font font;
    sf::Color colorSeleccionado;

    // Limitamos la cantidad de puntos, colores y textos de rutas que se pueden añadir
    PuntoConNombre puntos[100];
    sf::RectangleShape paleta[6];
    sf::Color colores[6];
    sf::Text nombresRutas[100];

    int contadorPuntos = 1;
    int puntosCount = 0;
    int nombresRutasCount = 0;
};

MapaTuristico::MapaTuristico(const std::string& mapaFile, const std::string& fontFile)
    : window(sf::VideoMode(1000, 700), "Mapa Turístico"), colorSeleccionado(sf::Color::Red) {

    if (!mapaTexture.loadFromFile(mapaFile)) {
        throw std::runtime_error("No se pudo cargar la textura del mapa");
    }
    mapaSprite.setTexture(mapaTexture);

    if (!font.loadFromFile(fontFile)) {
        throw std::runtime_error("No se pudo cargar la fuente");
    }

    // Definimos los colores de la paleta
    colores[0] = sf::Color::Red;
    colores[1] = sf::Color::Green;
    colores[2] = sf::Color::Blue;
    colores[3] = sf::Color::Yellow;
    colores[4] = sf::Color::Magenta;
    colores[5] = sf::Color::Cyan;

    for (int i = 0; i < 6; ++i) {
        paleta[i].setSize(sf::Vector2f(30, 30));
        paleta[i].setFillColor(colores[i]);
        paleta[i].setPosition(10 + i * 40, 660);
    }
}

void MapaTuristico::run() {
    while (window.isOpen()) {
        manejarEventos();
        dibujar();
    }
}

void MapaTuristico::manejarEventos() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);

            for (int i = 0; i < 6; ++i) {
                if (paleta[i].getGlobalBounds().contains(clickPos)) {
                    colorSeleccionado = colores[i];
                    return;
                }
            }

            if (event.mouseButton.y < 550) {
                agregarPunto(clickPos);
            }
        }
    }
}

void MapaTuristico::dibujar() {
    window.clear();
    window.draw(mapaSprite);

    for (int i = 0; i < puntosCount; ++i) {
        puntos[i].dibujar(window);
    }

    for (int i = 0; i < 6; ++i) {
        window.draw(paleta[i]);
    }

    for (int i = 0; i < nombresRutasCount; ++i) {
        window.draw(nombresRutas[i]);
    }

    window.display();
}

std::string MapaTuristico::mostrarCuadroDeDialogo() {
    sf::RenderWindow inputWindow(sf::VideoMode(300, 100), "Nombre del Punto");
    sf::Text textoInput("", font, 20);
    textoInput.setPosition(10, 10);
    textoInput.setFillColor(sf::Color::Black);

    string input = "Punto " + std::to_string(contadorPuntos) + " : ";
    while (inputWindow.isOpen()) {
        sf::Event event;
        while (inputWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                inputWindow.close();
            }
            else if (event.type == sf::Event::TextEntered) {
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
        textoInput.setString("Ruta: " + input);
        inputWindow.clear(sf::Color::White);
        inputWindow.draw(textoInput);
        inputWindow.display();
    }
    return input;
}

void MapaTuristico::agregarPunto(const sf::Vector2f& pos) {
    std::string nombre = mostrarCuadroDeDialogo();

    if (nombre.empty()) {
        nombre = "Punto " + std::to_string(contadorPuntos);
    }
    contadorPuntos++;

    if (puntosCount < 100 && nombresRutasCount < 100) {
        puntos[puntosCount++] = PuntoConNombre(pos, colorSeleccionado, nombre, font);

        sf::Text puntoTexto(nombre, font, 14);
        puntoTexto.setFillColor(sf::Color::Black);

        float offsetX = 10.0f;
        float offsetY = window.getSize().y - 60.0f - (nombresRutasCount * 20);

        puntoTexto.setPosition(offsetX, offsetY);

        nombresRutas[nombresRutasCount++] = puntoTexto;
    }
}
