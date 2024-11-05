#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Rutas.h"

using namespace std;
class MapaTuristico {
public:
    MapaTuristico(const std::string& mapaFile, const std::string& fontFile);
    void run();

private:
    void manejarEventos();
    void dibujar();
    void agregarPunto(const sf::Vector2f& pos);

    sf::RenderWindow window;
    sf::Texture mapaTexture;
    sf::Sprite mapaSprite;
    sf::Font font;
    sf::Color colorSeleccionado;
    std::vector<PuntoConNombre> puntos;
    std::vector<sf::RectangleShape> paleta;
    std::vector<sf::Color> colores;
};


MapaTuristico::MapaTuristico(const std::string& mapaFile, const std::string& fontFile)
    : window(sf::VideoMode(800, 600), "Mapa Turístico"), colorSeleccionado(sf::Color::Red) {

    if (!mapaTexture.loadFromFile(mapaFile)) {
        throw std::runtime_error("No se pudo cargar la textura del mapa");
    }
    mapaSprite.setTexture(mapaTexture);

   

    colores = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };

    for (size_t i = 0; i < colores.size(); ++i) {
        sf::RectangleShape colorBox(sf::Vector2f(30, 30));
        colorBox.setFillColor(colores[i]);
        colorBox.setPosition(10 + i * 40, 550);
        paleta.push_back(colorBox);
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

            for (size_t i = 0; i < paleta.size(); ++i) {
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

    for (const auto& punto : puntos) {
        punto.dibujar(window);
    }

    for (const auto& colorBox : paleta) {
        window.draw(colorBox);
    }

    window.display();
}

void MapaTuristico::agregarPunto(const sf::Vector2f& pos) {
    std::string nombre;
    std::cout << "Ingresa el nombre del punto: ";
    std::cin >> nombre;

    puntos.emplace_back(pos, colorSeleccionado, nombre, font);
}
