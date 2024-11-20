#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

using namespace sf;
using namespace std;

class PuntoConNombre {
public:
    PuntoConNombre();
    PuntoConNombre(const Vector2f& pos, const Color& col, const string& nom, const Font& font);

    Color getFillColor() const;
    void dibujar(RenderWindow& window) const;
    Vector2f getPosition() const;
    float distanciaAlPunto(const Vector2f& pos) const;

private:
    Vector2f posicion;
    Color color;
    string nombre;
    const Font* fuente;
    CircleShape punto;
    Text texto;
};

PuntoConNombre::PuntoConNombre()
    : fuente(nullptr) {
    // Constructor por defecto (puedes agregar inicialización aquí si es necesario)
}

PuntoConNombre::PuntoConNombre(const Vector2f& pos, const Color& col, const string& nom, const Font& font)
    : posicion(pos), color(col), nombre(nom), fuente(&font) {
    // Inicializa el punto y el texto con los valores proporcionados
    punto.setRadius(10);  // Establece un radio de 10 para el círculo
    punto.setFillColor(color);
    punto.setPosition(posicion);

    texto.setFont(*fuente);
    texto.setString(nombre);
    texto.setCharacterSize(24);
    texto.setFillColor(Color::Black);
    texto.setPosition(posicion.x + 15, posicion.y);  // Ajuste de la posición del texto
}

Color PuntoConNombre::getFillColor() const {
    return color;
}

void PuntoConNombre::dibujar(RenderWindow& window) const {
    window.draw(punto);
    window.draw(texto);
}

Vector2f PuntoConNombre::getPosition() const {
    return posicion;  // Devuelve la posición almacenada en el miembro `posicion`
}

float PuntoConNombre::distanciaAlPunto(const Vector2f& pos) const {
    return sqrt(pow(pos.x - posicion.x, 2) + pow(pos.y - posicion.y, 2));
}
