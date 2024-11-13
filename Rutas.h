#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;

class PuntoConNombre {
public:
    PuntoConNombre() : posicion(0, 0), color(Color::White), nombre(""), fuente(nullptr) {}
    PuntoConNombre(const Vector2f& pos, const Color& col, const string& nom, const Font& font)
        : posicion(pos), color(col), nombre(nom), fuente(&font) {
        punto.setRadius(5);
        punto.setFillColor(color);
        punto.setPosition(pos - Vector2f(5, 5));

        texto.setFont(font);
        texto.setString(nombre);
        texto.setCharacterSize(14);
        texto.setFillColor(Color::Black);
        texto.setPosition(pos + Vector2f(10, -5));
    }

    Color getFillColor() const {
        return color;
    }

    void dibujar(RenderWindow& window) const {
        window.draw(punto);
        window.draw(texto);
    }

    Vector2f getPosition() const {
        return posicion;
    }

    float distanciaAlPunto(const Vector2f& pos) const {
        return sqrt(pow(pos.x - posicion.x, 2) + pow(pos.y - posicion.y, 2));
    }

private:
    Vector2f posicion;
    Color color;
    string nombre;
    const Font* fuente;
    CircleShape punto;
    Text texto;
};

class NodoRuta {
public:
    PuntoConNombre punto;
    NodoRuta* siguiente;
    NodoRuta* anterior;

    NodoRuta(const PuntoConNombre& punto)
        : punto(punto), siguiente(nullptr), anterior(nullptr) {}
};


class Ruta {
public:
    NodoRuta* cabeza;
    NodoRuta* cola;

    Ruta() : cabeza(nullptr), cola(nullptr) {}

    void agregarPunto(const PuntoConNombre& punto) {
        NodoRuta* nuevoNodo = new NodoRuta(punto);

        if (!cabeza) {
            cabeza = cola = nuevoNodo;
        }
        else {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            cola = nuevoNodo;
        }
    }

    void eliminarPunto(const Vector2f& pos) {
        NodoRuta* actual = cabeza;
        while (actual) {
            if (actual->punto.distanciaAlPunto(pos) < 10.0f) { // Si el clic está cerca del punto
                if (actual->anterior) {
                    actual->anterior->siguiente = actual->siguiente;
                }
                if (actual->siguiente) {
                    actual->siguiente->anterior = actual->anterior;
                }
                if (actual == cabeza) {
                    cabeza = actual->siguiente;
                }
                if (actual == cola) {
                    cola = actual->anterior;
                }
                delete actual;
                return; 
            }
            actual = actual->siguiente;
        }
    }

    void dibujar(RenderWindow& ventana) const {
        NodoRuta* actual = cabeza;
        while (actual) {
            actual->punto.dibujar(ventana);
            actual = actual->siguiente;
        }
    }

    // Dibujar una curva B-Spline entre los puntos usando la lista de nodos
    void dibujarLineaCurva(sf::RenderWindow& window) const {
        if (!cabeza || !cabeza->siguiente) return;

        const int MAX_PUNTOS = 100;
        sf::Vector2f puntos[MAX_PUNTOS];
        int numPuntos = 0;

        NodoRuta* actual = cabeza;
        while (actual && numPuntos < MAX_PUNTOS) {
            puntos[numPuntos++] = actual->punto.getPosition();
            actual = actual->siguiente;
        }

        if (numPuntos < 2) return; 

        sf::VertexArray curva(sf::LinesStrip);

        for (int i = 0; i < numPuntos - 1; ++i) {
            sf::Vector2f p0 = (i == 0) ? puntos[i] : puntos[i - 1];
            sf::Vector2f p1 = puntos[i];
            sf::Vector2f p2 = puntos[i + 1];
            sf::Vector2f p3 = (i + 2 < numPuntos) ? puntos[i + 2] : puntos[i + 1];

            for (float t = 0; t <= 1; t += 0.01f) {
                float t2 = t * t;
                float t3 = t2 * t;

                // Fórmulas de Catmull-Rom
                float b0 = -0.5f * t3 + t2 - 0.5f * t;
                float b1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
                float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
                float b3 = 0.5f * t3 - 0.5f * t2;

                sf::Vector2f pos = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
                curva.append(sf::Vertex(pos, sf::Color::Black));
            }
        }

        window.draw(curva);
    }

    ~Ruta() {
        NodoRuta* actual = cabeza;
        while (actual) {
            NodoRuta* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};
