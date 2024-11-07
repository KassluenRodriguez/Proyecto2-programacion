#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class PuntoConNombre {
public:
    PuntoConNombre() : posicion(0, 0), color(sf::Color::White), nombre(""), fuente(nullptr) {} 
    PuntoConNombre(const sf::Vector2f& pos, const sf::Color& col, const std::string& nom, const sf::Font& font)
        : posicion(pos), color(col), nombre(nom), fuente(&font) {
        punto.setRadius(5);
        punto.setFillColor(color);
        punto.setPosition(pos - sf::Vector2f(5, 5));

        texto.setFont(font);
        texto.setString(nombre);
        texto.setCharacterSize(14);
        texto.setFillColor(sf::Color::Black);
        texto.setPosition(pos + sf::Vector2f(10, -5));
    }

    void dibujar(sf::RenderWindow& window) const {
        window.draw(punto);
        window.draw(texto);
    }

private:
    sf::Vector2f posicion;
    sf::Color color;
    std::string nombre;
    const sf::Font* fuente;
    sf::CircleShape punto;
    sf::Text texto;
};

class Node {
public:
    PuntoConNombre puntos;
    Node* next;

    Node(PuntoConNombre punto) : puntos(punto), next(nullptr) {}
};

class Rutas {
private:
    Node* head;

public:
    Rutas() : head(nullptr) {}

    // Método para añadir una nueva ruta (punto) al final de la lista enlazada
    void nuevaRuta(const PuntoConNombre& punto) {
        Node* nuevoNodo = new Node(punto);

        if (!head) {
            head = nuevoNodo;
        }
        else {
            Node* actual = head;
            while (actual->next) {
                actual = actual->next;
            }
            actual->next = nuevoNodo;
        }
    }

    // Método para dibujar todos los puntos de la lista enlazada
    void dibujar(sf::RenderWindow& window) const {
        Node* actual = head;
        while (actual) {
            actual->puntos.dibujar(window);
            actual = actual->next;
        }
    }

    // Destructor para liberar la memoria
    ~Rutas() {
        Node* actual = head;
        while (actual) {
            Node* temp = actual;
            actual = actual->next;
            delete temp;
        }
    }
};
