// Mapa.h
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Rutas.h"

using namespace std;
using namespace sf;

class MapaTuristico {
public:
    MapaTuristico(const string& mapaFile, const string& fontFile);
    void run();

private:
    void manejarEventos();
    void dibujar();
    void ajustarEscala();
    void limitarVista();
    void agregarPunto(const Vector2f& pos);
    string mostrarCuadroDeDialogo();
    void agregarRuta();
    void eliminarPunto(const Vector2f& pos);
    void activarModoEdicion();

    RenderWindow window;
    Texture mapaTexture;
    Sprite mapaSprite;
    Font font;
    Color colorSeleccionado;
    View view;

    Ruta rutas;

    RectangleShape paleta[6];
    Color colores[6];
    Text nombresRutas[100];

    int contadorPuntos = 1;
    int nombresRutasCount = 0;
    int contadorRutas = 1;
    RectangleShape botonAgregarRuta;
    RectangleShape botonModoEdicion;
    bool modoEdicion = false;

    float zoomFactor = 1.0f;
    float minZoomFactor;
    Vector2f desplazamiento;
    bool isDragging = false;
    Vector2f dragStartPos;
    float moveSpeed = 20.0f;
};

MapaTuristico::MapaTuristico(const string& mapaFile, const string& fontFile)
    : window(VideoMode(1000, 700), "Mapa Turístico"), colorSeleccionado(Color::Red), desplazamiento(0, 0) {

    if (!mapaTexture.loadFromFile(mapaFile)) {
        throw runtime_error("No se pudo cargar la textura del mapa");
    }
    mapaSprite.setTexture(mapaTexture);
    ajustarEscala();

    if (!font.loadFromFile(fontFile)) {
        throw runtime_error("No se pudo cargar la fuente");
    }

    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    window.setView(view);

    minZoomFactor = std::max(
        static_cast<float>(window.getSize().x) / mapaTexture.getSize().x,
        static_cast<float>(window.getSize().y) / mapaTexture.getSize().y
    );

    colores[0] = Color::Red;
    colores[1] = Color::Green;
    colores[2] = Color::Blue;
    colores[3] = Color::Yellow;
    colores[4] = Color::Magenta;
    colores[5] = Color::Cyan;

    for (int i = 0; i < 6; ++i) {
        paleta[i].setSize(Vector2f(30, 30));
        paleta[i].setFillColor(colores[i]);
        paleta[i].setPosition(10 + i * 40, 660);
    }

    botonAgregarRuta.setSize(Vector2f(150, 40));
    botonAgregarRuta.setFillColor(Color::White);
    botonAgregarRuta.setPosition(250, window.getSize().y - 50);

    botonModoEdicion.setSize(Vector2f(235, 40));
    botonModoEdicion.setFillColor(Color::White);
    botonModoEdicion.setPosition(410, window.getSize().y - 50);
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
            Vector2f clickPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y), view);

            for (int i = 0; i < 6; ++i) {
                if (paleta[i].getGlobalBounds().contains(clickPos)) {
                    colorSeleccionado = colores[i];
                    return;
                }
            }
            if (botonAgregarRuta.getGlobalBounds().contains(clickPos)) {
                agregarRuta();
            }
            if (botonModoEdicion.getGlobalBounds().contains(clickPos)) {
                activarModoEdicion();
            }
            if (modoEdicion) {
                eliminarPunto(clickPos);
            }
            else if (clickPos.y < 550) {
                agregarPunto(clickPos);
            }
            dragStartPos = clickPos;
            isDragging = true;
        }

        if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
            isDragging = false;
        }

        if (event.type == Event::MouseMoved && isDragging) {
            Vector2f currentPos = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y), view);
            Vector2f offset = dragStartPos - currentPos;
            view.move(offset);
            dragStartPos = currentPos;
            limitarVista();
            window.setView(view);
        }

        if (event.type == Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                zoomFactor *= 1.1f;
            }
            else {
                zoomFactor /= 1.1f;
                if (zoomFactor < minZoomFactor) {
                    zoomFactor = minZoomFactor;
                }
            }
            view.setSize(window.getSize().x * zoomFactor, window.getSize().y * zoomFactor);
            limitarVista();
            window.setView(view);
        }

        if (event.type == Event::Resized) {
            Vector2f newSize(event.size.width, event.size.height);
            view.setSize(newSize);
            view.setCenter(newSize * 0.5f);
            window.setView(view);
            minZoomFactor = max(
                static_cast<float>(newSize.x) / mapaTexture.getSize().x,
                static_cast<float>(newSize.y) / mapaTexture.getSize().y
            );
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::W)) {
        view.move(0, -moveSpeed * zoomFactor);
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        view.move(-moveSpeed * zoomFactor, 0);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        view.move(0, moveSpeed * zoomFactor);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        view.move(moveSpeed * zoomFactor, 0);
    }
    limitarVista();
    window.setView(view);
}

void MapaTuristico::limitarVista() {
    Vector2f mapaSize(mapaTexture.getSize().x * mapaSprite.getScale().x,
        mapaTexture.getSize().y * mapaSprite.getScale().y);
    FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());

    if (viewBounds.left < 0) view.setCenter(view.getSize().x / 2.f, view.getCenter().y);
    if (viewBounds.top < 0) view.setCenter(view.getCenter().x, view.getSize().y / 2.f);
    if (viewBounds.left + viewBounds.width > mapaSize.x)
        view.setCenter(mapaSize.x - view.getSize().x / 2.f, view.getCenter().y);
    if (viewBounds.top + viewBounds.height > mapaSize.y)
        view.setCenter(view.getCenter().x, mapaSize.y - view.getSize().y / 2.f);
}

void MapaTuristico::dibujar() {
    window.clear();
    window.setView(view);  // Ajustar la vista antes de dibujar el mapa
    window.draw(mapaSprite);
    rutas.dibujar(window);
    rutas.dibujarLineaCurva(window);

    for (int i = 0; i < 6; ++i) {
        window.draw(paleta[i]);
    }

    for (int i = 0; i < nombresRutasCount; ++i) {
        window.draw(nombresRutas[i]);
    }

    window.draw(botonAgregarRuta);
    window.draw(botonModoEdicion);

    Text textoBoton("Agregar Ruta", font, 18);
    textoBoton.setFillColor(Color::Black);
    textoBoton.setPosition(botonAgregarRuta.getPosition().x + 10, botonAgregarRuta.getPosition().y + 10);
    window.draw(textoBoton);

    Text textoModoEdicion(modoEdicion ? "Modo Edición: Activado" : "Modo Edición: Desactivado", font, 18);
    textoModoEdicion.setFillColor(Color::Black);
    textoModoEdicion.setPosition(botonModoEdicion.getPosition().x + 10, botonModoEdicion.getPosition().y + 10);
    window.draw(textoModoEdicion);

    window.display();
}


void MapaTuristico::ajustarEscala() {
    Vector2u windowSize = window.getSize();
    Vector2u textureSize = mapaTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    mapaSprite.setScale(scaleX, scaleY);
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
        textoInput.setString("Ruta " + to_string(contadorRutas) + " - " + input);
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

    if (nombresRutasCount < 100) {
        Text rutaTexto(nombreRuta, font, 14);
        rutaTexto.setFillColor(Color::Black);

        float offsetX = 10.0f;
        float offsetY = window.getSize().y - 60.0f - (nombresRutasCount * 20);

        rutaTexto.setPosition(offsetX, offsetY);
        nombresRutas[nombresRutasCount++] = rutaTexto;
    }

    contadorRutas++;
}

void MapaTuristico::eliminarPunto(const Vector2f& pos) {
    rutas.eliminarPunto(pos);
}

void MapaTuristico::activarModoEdicion() {
    modoEdicion = !modoEdicion;
}