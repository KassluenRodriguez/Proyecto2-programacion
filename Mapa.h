
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Rutas.h"

#include "RutasEnlazadas.h"


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
    void seleccionarPunto(const Vector2f& pos);
    void eliminarRuta();


    // Métodos de manejo de archivos
    void guardarRutasEnArchivo(const string& nombreArchivo);
    void cargarRutasDesdeArchivo(const string& nombreArchivo);


    RenderWindow window;
    Texture mapaTexture;
    Sprite mapaSprite;
    Font font;
    Color colorSeleccionado;
    View view;

    ListaEnlazada<Ruta*> rutas; // Uso de ListaEnlazada 

    RectangleShape paleta[6];
    Color colores[6];
    Text nombresRutas[100];

    int contadorPuntos = 1;
    int nombresRutasCount = 0;
    int contadorRutas = 1;

    RectangleShape botonAgregarRuta;
    RectangleShape botonModoEdicion;
    RectangleShape botonEliminarRuta;
    bool mostrarBotonEliminar = false;
    bool modoEdicion = false;
    bool sePuedeAgregarPunto = false;
    Ruta* rutaSeleccionada = nullptr;

    float zoomFactor = 1.0f;
    float minZoomFactor = 1.0f;
    float maxZoomFactor = 1.0f;
    Vector2f desplazamiento;
    bool isDragging = false;
    Vector2f dragStartPos;
    float moveSpeed = 5.0f;
};



MapaTuristico::MapaTuristico(const string& mapaFile, const string& fontFile)
    : window(VideoMode(800, 800), "Mapa Turístico"), colorSeleccionado(Color::Red), desplazamiento(0, 0) {

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

    minZoomFactor = min(
        static_cast<float>(window.getSize().x) / mapaTexture.getSize().x,
        static_cast<float>(window.getSize().y) / mapaTexture.getSize().y
    ) * 0.5f;

    maxZoomFactor = 3.0f;

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

    botonEliminarRuta.setSize(Vector2f(150, 40));
    botonEliminarRuta.setFillColor(Color::Red);
    botonEliminarRuta.setOutlineColor(Color::Black);
    botonEliminarRuta.setOutlineThickness(2);
    botonEliminarRuta.setPosition(Vector2f(700, 20));  // Posición inicial arbitraria, se ajustará dinámicamente


    cargarRutasDesdeArchivo("rutas.txt");


void MapaTuristico::run() {
    while (window.isOpen()) {
        manejarEventos();
        window.clear();
        dibujar();
        window.display();
    }
}


void MapaTuristico::guardarRutasEnArchivo(const string& nombreArchivo) {
    ManejoDeArchivos::guardarRutas(rutas, nombreArchivo);
}

void MapaTuristico::cargarRutasDesdeArchivo(const string& nombreArchivo) {
    ManejoDeArchivos::cargarRutas(rutas, nombreArchivo);
}


void MapaTuristico::ajustarEscala() {
    Vector2u windowSize = window.getSize();
    Vector2u textureSize = mapaTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

    mapaSprite.setScale(scaleX, scaleY);

    maxZoomFactor = max(
        static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
        static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
    );
}

void MapaTuristico::manejarEventos() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseButtonPressed) {
            Vector2f clickPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y), view);

            if (event.mouseButton.button == Mouse::Left) {
                if (rutaSeleccionada != nullptr) {

                    
                    Nodo<Ruta*>* nodoRuta = rutas.begin();
                    Nodo<Ruta*>* nodoPrevio = nullptr;

                   
                    while (nodoRuta != nullptr) {
                        if (nodoRuta->dato == rutaSeleccionada) {
                           
                            if (nodoPrevio == nullptr) {
                                rutas.removeFirst(); 
                            }
                            else {
                               
                                nodoPrevio->siguiente = nodoRuta->siguiente;
                                if (nodoRuta->siguiente != nullptr) {
                                    nodoRuta->siguiente->anterior = nodoPrevio;
                                }
                            }

                            
                            delete rutaSeleccionada;
                            rutaSeleccionada = nullptr;
                            mostrarBotonEliminar = false;  
                            break;
                        }
                        nodoPrevio = nodoRuta;
                        nodoRuta = nodoRuta->siguiente;
                    }

                }
                else {
                    for (int i = 0; i < 6; ++i) {
                        if (paleta[i].getGlobalBounds().contains(clickPos)) {
                            colorSeleccionado = colores[i];
                            return;
                        }
                    }

                    if (botonAgregarRuta.getGlobalBounds().contains(clickPos)) {
                        agregarRuta();
                    }

                    else if (botonEliminarRuta.getGlobalBounds().contains(clickPos)) {
                        eliminarRuta();
                        guardarRutasEnArchivo("rutas.txt"); // Guardar cambios después de eliminar
                    }

                    else if (botonModoEdicion.getGlobalBounds().contains(clickPos)) {
                        activarModoEdicion();
                    }
                    else if (modoEdicion) {
                        seleccionarPunto(clickPos);
                    }
                    else if (sePuedeAgregarPunto && clickPos.y < 550) {
                        agregarPunto(clickPos);
                    }
                }
            }

            if (event.mouseButton.button == Mouse::Right) {
                if (rutaSeleccionada != nullptr) {
                    // Eliminar punto de la ruta seleccionada
                    rutaSeleccionada->eliminarPunto(clickPos);
                }
                else {
                    // Seleccionar ruta al hacer clic derecho
                    Nodo<Ruta*>* nodoRuta = rutas.begin();
                    while (nodoRuta != nullptr) {
                        Ruta* ruta = nodoRuta->dato;
                        Nodo<PuntoConNombre>* nodoPunto = ruta->getPuntos().begin();
                        while (nodoPunto != nullptr) {
                            const PuntoConNombre& punto = nodoPunto->dato;
                            if (punto.distanciaAlPunto(clickPos) < 5.0f) {
                                rutaSeleccionada = ruta;
                                mostrarBotonEliminar = true;
                                return;
                            }
                            nodoPunto = nodoPunto->siguiente;
                        }
                        nodoRuta = nodoRuta->siguiente;
                    }
                }
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
            Vector2f mouseWorldPos = window.mapPixelToCoords(Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), view);
            float zoomAmount = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
            float newZoomFactor = zoomFactor * zoomAmount;

            if (newZoomFactor >= minZoomFactor && newZoomFactor <= maxZoomFactor) {
                zoomFactor = newZoomFactor;
                view.zoom(zoomAmount);

                Vector2f mouseWorldPosAfterZoom = window.mapPixelToCoords(Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), view);
                Vector2f offset = mouseWorldPos - mouseWorldPosAfterZoom;
                view.move(offset);

                limitarVista();
                window.setView(view);
            }
        }

        if (event.type == Event::Resized) {
            Vector2f newSize(event.size.width, event.size.height);
            view.setSize(newSize);
            view.setCenter(newSize * 0.5f);
            window.setView(view);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::W)) view.move(0, -moveSpeed * zoomFactor);
    if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-moveSpeed * zoomFactor, 0);
    if (Keyboard::isKeyPressed(Keyboard::S)) view.move(0, moveSpeed * zoomFactor);
    if (Keyboard::isKeyPressed(Keyboard::D)) view.move(moveSpeed * zoomFactor, 0);

    limitarVista();
    window.setView(view);
}




void MapaTuristico::seleccionarPunto(const Vector2f& pos) {
    rutaSeleccionada = nullptr;

    Nodo<Ruta*>* nodoRuta = rutas.begin();  // Iterar desde el inicio de la lista de rutas
    while (nodoRuta != nullptr) {
        Nodo<PuntoConNombre>* nodoPunto = nodoRuta->dato->getPuntos().begin();
        while (nodoPunto != nullptr) {
            const PuntoConNombre& punto = nodoPunto->dato;
            if (punto.distanciaAlPunto(pos) < 5.0f) {
                rutaSeleccionada = nodoRuta->dato;
                return;
            }
            nodoPunto = nodoPunto->siguiente;
        }
        nodoRuta = nodoRuta->siguiente;  // Avanzar al siguiente nodo de la lista
    }
}

void MapaTuristico::limitarVista() {
    Vector2f viewSize = view.getSize();
    Vector2f viewCenter = view.getCenter();

    float left = viewCenter.x - viewSize.x / 2;
    float top = viewCenter.y - viewSize.y / 2;

    float right = viewCenter.x + viewSize.x / 2;
    float bottom = viewCenter.y + viewSize.y / 2;

    float textureWidth = static_cast<float>(mapaTexture.getSize().x);
    float textureHeight = static_cast<float>(mapaTexture.getSize().y);


    if (viewSize.x > textureWidth) {
        left = 0;
        right = textureWidth;
        viewCenter.x = textureWidth / 2;
    }
    if (viewSize.y > textureHeight) {
        top = 0;
        bottom = textureHeight;
        viewCenter.y = textureHeight / 2;
    }


    if (left < 0) viewCenter.x += -left;
    if (top < 0) viewCenter.y += -top;
    if (right > textureWidth) viewCenter.x -= (right - textureWidth);
    if (bottom > textureHeight) viewCenter.y -= (bottom - textureHeight);

    view.setCenter(viewCenter);
}

void MapaTuristico::dibujar() {
    window.setView(view);
    window.draw(mapaSprite);


    Nodo<Ruta*>* nodoRuta = rutas.begin();
    while (nodoRuta != nullptr) {
        Ruta* ruta = nodoRuta->dato;
        ruta->dibujar(window);

        if (modoEdicion && ruta == rutaSeleccionada) {
            ruta->dibujarLineaCurva(window, Color::Green);
        }
        else {
            ruta->dibujarLineaCurva(window);
        }
        nodoRuta = nodoRuta->siguiente;

    }

    if (mostrarBotonEliminar && rutaSeleccionada != nullptr) {
        window.draw(botonEliminarRuta);

        Text textoBoton("Eliminar Ruta", font, 18);
        textoBoton.setFillColor(Color::Black);
        textoBoton.setPosition(botonEliminarRuta.getPosition().x + 10, botonEliminarRuta.getPosition().y + 10);
        window.draw(textoBoton);
    }

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
}

void MapaTuristico::agregarPunto(const Vector2f& pos) {
    string nombrePunto = mostrarCuadroDeDialogo();

    if (nombrePunto.empty()) {
        nombrePunto = "Punto " + to_string(contadorPuntos);
    }



    if (rutas.begin() != nullptr) {
        PuntoConNombre nuevoPunto(pos, colorSeleccionado, nombrePunto, font);


        Nodo<Ruta*>* ultimoNodo = rutas.begin();
        while (ultimoNodo->siguiente != nullptr) {
            ultimoNodo = ultimoNodo->siguiente;
        }

        ultimoNodo->dato->agregarPunto(nuevoPunto);
    }


    contadorPuntos++;
}

void MapaTuristico::agregarRuta() {

    string nombreRuta = "Ruta " + to_string(contadorRutas);


    Ruta* nuevaRuta = new Ruta(nombreRuta);
    rutas.push_back(nuevaRuta);

    if (nombresRutasCount < 100) {
        Text rutaTexto(nombreRuta, font, 14);
        rutaTexto.setFillColor(Color::Black);

        float offsetX = 10.0f;
        float offsetY = window.getSize().y - 60.0f - (nombresRutasCount * 20);

        rutaTexto.setPosition(offsetX, offsetY);
        nombresRutas[nombresRutasCount++] = rutaTexto;
    }



    sePuedeAgregarPunto = true;



    contadorRutas++;
}

string MapaTuristico::mostrarCuadroDeDialogo() {
    RenderWindow inputWindow(VideoMode(300, 100), "Nombre del Punto");
    Text textoInput("", font, 20);
    textoInput.setPosition(10, 10);
    textoInput.setFillColor(Color::Black);



    string input = "Punto " + to_string(contadorPuntos);
    string tituloRuta = "Ruta " + to_string(contadorRutas);


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

        textoInput.setString(tituloRuta + " - " + input);

        inputWindow.clear(Color::White);
        inputWindow.draw(textoInput);
        inputWindow.display();
    }
    return input;  
}

void MapaTuristico::eliminarRuta() {
    if (rutaSeleccionada != nullptr) {

        Nodo<Ruta*>* nodoRuta = rutas.getCabeza();
        Nodo<Ruta*>* nodoPrevio = nullptr;


        while (nodoRuta != nullptr) {
            if (nodoRuta->dato == rutaSeleccionada) {

                if (nodoPrevio != nullptr) {
                    nodoPrevio->siguiente = nodoRuta->siguiente;
                }
                else {

                    rutas.setCabeza(nodoRuta->siguiente);
                }

                if (nodoRuta == rutas.getCola()) {

                    rutas.setCola(nodoPrevio);
                }


                delete nodoRuta->dato;
                delete nodoRuta;
                rutaSeleccionada = nullptr;
                mostrarBotonEliminar = false;
                return;
            }


            nodoPrevio = nodoRuta;
            nodoRuta = nodoRuta->siguiente;
        }
    }
}


void MapaTuristico::eliminarPunto(const Vector2f& pos) {
    Nodo<Ruta*>* nodoRuta = rutas.begin();


    while (nodoRuta != nullptr) {
        nodoRuta->dato->eliminarPunto(pos);
        nodoRuta = nodoRuta->siguiente;
    }
}

void MapaTuristico::activarModoEdicion() {
    modoEdicion = !modoEdicion;
}