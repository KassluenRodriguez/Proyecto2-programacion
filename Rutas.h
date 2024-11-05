#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

class PuntoConNombre {
public:
    PuntoConNombre(const sf::Vector2f& pos, const sf::Color& color, const std::string& nombre, const sf::Font& font);

    void dibujar(sf::RenderWindow& window) const;

private:
    sf::CircleShape punto;
    sf::Text texto;
};



PuntoConNombre::PuntoConNombre(const sf::Vector2f& pos, const sf::Color& color, const std::string& nombre, const sf::Font& font) {
    punto.setRadius(5);
    punto.setFillColor(color);
    punto.setPosition(pos - sf::Vector2f(5, 5));

    texto.setFont(font);
    texto.setString(nombre);
    texto.setCharacterSize(14);
    texto.setFillColor(sf::Color::Black);
    texto.setPosition(pos + sf::Vector2f(10, -5));
}

void PuntoConNombre::dibujar(sf::RenderWindow& window) const {
    window.draw(punto);
    window.draw(texto);
}
