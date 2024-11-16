#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <list>  
using namespace std;
using namespace sf;

class PuntoConNombre {
public:
    PuntoConNombre();
    PuntoConNombre(const Vector2f& pos, const Color& col, const string& nom, const Font& font);

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

PuntoConNombre::PuntoConNombre(const Vector2f& pos, const Color& col, const string& nom, const Font& font)
    : posicion(pos), color(col), nombre(nom), fuente(&font) {
    punto.setRadius(6);
    punto.setFillColor(color);
    punto.setPosition(pos - Vector2f(5, 5));

    texto.setFont(font);
    texto.setString(nombre);
    texto.setCharacterSize(14);
    texto.setFillColor(Color::Black);
    texto.setPosition(pos + Vector2f(10, -5));
}


class Ruta {
public:
    Ruta(const string& nombreRuta);
    ~Ruta();

    void agregarPunto(const PuntoConNombre& punto);
    void eliminarPunto(const sf::Vector2f& pos);
    void dibujar(sf::RenderWindow& window);
    void dibujarLineaCurva(sf::RenderWindow& window, const sf::Color& color = sf::Color(128, 0, 128));

    const std::list<PuntoConNombre>& getPuntos() const;

private:
    string nombreRuta;
    std::list<PuntoConNombre> puntos;
};

const std::list<PuntoConNombre>& Ruta::getPuntos() const {
    return puntos;
}

class ListaRutas {
public:
    ListaRutas() = default;

    ~ListaRutas() {
        for (Ruta* ruta : rutas) {
            delete ruta;
        }
    }

    bool empty() const {
        return rutas.empty();
    }

    void push_back(Ruta* ruta) {
        rutas.push_back(ruta);
    }

    void agregarRuta(Ruta* ruta) {
        rutas.push_back(ruta);
    }

    void eliminarRuta(Ruta* ruta) {
        rutas.remove(ruta);
    }

    void eliminarPunto(const sf::Vector2f& pos) {
        for (Ruta* ruta : rutas) {
            ruta->eliminarPunto(pos);
        }
    }

    void dibujar(sf::RenderWindow& window) {
        for (Ruta* ruta : rutas) {
            ruta->dibujar(window);  
        }
    }

    void dibujarLineaCurva(sf::RenderWindow& window) {
        for (Ruta* ruta : rutas) {
            ruta->dibujarLineaCurva(window);  
        }
    }

    Ruta* back() {
        if (!rutas.empty()) {
            return rutas.back();
        }
        return nullptr;
    }

 
    auto begin() { return rutas.begin(); }
    auto end() { return rutas.end(); }
    auto begin() const { return rutas.begin(); }
    auto end() const { return rutas.end(); }

private:
    std::list<Ruta*> rutas;  
};


void PuntoConNombre::dibujar(RenderWindow& window) const {
    window.draw(punto);
    window.draw(texto);
}

Vector2f PuntoConNombre::getPosition() const {
    return posicion;
}

float PuntoConNombre::distanciaAlPunto(const Vector2f& pos) const {
    return sqrt(pow(pos.x - posicion.x, 2) + pow(pos.y - posicion.y, 2));
}


Ruta::Ruta(const string& nombreRuta) : nombreRuta(nombreRuta) {}

Ruta::~Ruta() {
    
}

void Ruta::agregarPunto(const PuntoConNombre& punto) {
    puntos.push_back(punto);
}

void Ruta::eliminarPunto(const sf::Vector2f& pos) {
    const float tolerancia = 5.0f;  
    puntos.remove_if([&](const PuntoConNombre& p) {
        return p.distanciaAlPunto(pos) < tolerancia;
        });
}





void Ruta::dibujar(sf::RenderWindow& window) {
    for (const auto& punto : puntos) {
        punto.dibujar(window);  
    }
}


void Ruta::dibujarLineaCurva(sf::RenderWindow& window, const sf::Color& color) {
    if (puntos.size() < 2) return;  

    float grosor = 5.0f;

    std::vector<sf::Vertex> vertices;
    std::vector<sf::Vector2f> posiciones;

    
    for (const auto& punto : puntos) {
        posiciones.push_back(punto.getPosition());
    }

    
    for (size_t i = 0; i < posiciones.size(); ++i) {
        sf::Vector2f p0 = (i == 0) ? posiciones[i] : posiciones[i - 1];
        sf::Vector2f p1 = posiciones[i];
        sf::Vector2f p2 = (i + 1 < posiciones.size()) ? posiciones[i + 1] : posiciones[i];
        sf::Vector2f p3 = (i + 2 < posiciones.size()) ? posiciones[i + 2] : p2;

        for (float t = 0; t <= 1; t += 0.1f) {  
            float t2 = t * t;
            float t3 = t2 * t;

            
            float b0 = -0.5f * t3 + t2 - 0.5f * t;
            float b1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
            float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
            float b3 = 0.5f * t3 - 0.5f * t2;

            sf::Vector2f puntoCurvado;
            puntoCurvado.x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
            puntoCurvado.y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;

            
            vertices.push_back(sf::Vertex(puntoCurvado, color)); 
        }
    }

    
    sf::VertexArray linea(sf::Quads, vertices.size() * 4);  

    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        sf::Vector2f direction = vertices[i + 1].position - vertices[i].position;
        sf::Vector2f unitDir = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f normal(-unitDir.y, unitDir.x);
        sf::Vector2f offset = normal * grosor / 2.0f;

        linea[i * 4 + 0].position = vertices[i].position + offset;
        linea[i * 4 + 1].position = vertices[i].position - offset;
        linea[i * 4 + 2].position = vertices[i + 1].position - offset;
        linea[i * 4 + 3].position = vertices[i + 1].position + offset;

        for (int j = 0; j < 4; ++j) {
            linea[i * 4 + j].color = color;  
        }
    }

    window.draw(linea);
}
