#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
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

private:
    Vector2f posicion;
    Color color;
    string nombre;
    const Font* fuente;
    CircleShape punto;
    Text texto;
};

// Nodo para almacenar cada punto turístico
class NodoRuta {
public:
    PuntoConNombre punto;
    NodoRuta* siguiente;
    NodoRuta* anterior;

    NodoRuta(const PuntoConNombre& punto)
        : punto(punto), siguiente(nullptr), anterior(nullptr) {}
};

// Lista doblemente enlazada para una ruta (con múltiples puntos)
class Ruta {
public:
    NodoRuta* cabeza;
    NodoRuta* cola;

    Ruta() : cabeza(nullptr), cola(nullptr) {}

    void agregarPunto(const PuntoConNombre& punto) {
        NodoRuta* nuevoNodo = new NodoRuta(punto);

        if (!cabeza) {
            cabeza = cola = nuevoNodo;  // Si la lista está vacía, el primer nodo es cabeza y cola
        }
        else {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            cola = nuevoNodo;
        }
    }

    void dibujar(RenderWindow& ventana) const {
        NodoRuta* actual = cabeza;
        while (actual) {
            actual->punto.dibujar(ventana);
            actual = actual->siguiente;
        }
    }

    void unirPuntos(RenderWindow& ventana) const {
        NodoRuta* actual = cabeza;
        while (actual && actual->siguiente) {
            Vector2f pos1 = actual->punto.getPosition();
            Vector2f pos2 = actual->siguiente->punto.getPosition();

            // Calcular distancia correctamente (usando la raíz cuadrada)
            float distancia = distanciaManualmente(pos1, pos2);

            // Calcular ángulo correctamente usando atan2
            float angulo = anguloManualmente(pos1, pos2);

            RectangleShape linea(Vector2f(distancia, 3));
            linea.setPosition(pos1);
            linea.setRotation(angulo);
            linea.setFillColor(Color::Black);

            ventana.draw(linea);
            actual = actual->siguiente;
        }
    }

    // Método para calcular distancia manualmente (sin sqrt)
    float distanciaManualmente(const Vector2f& pos1, const Vector2f& pos2) const {
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;
        return sqrt(dx * dx + dy * dy);  
    }

    // Método para calcular ángulo manualmente (sin atan2)
    float anguloManualmente(const Vector2f& pos1, const Vector2f& pos2) const {
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;
        return atan2(dy, dx) * (180.0f / 3.14159f);  
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
