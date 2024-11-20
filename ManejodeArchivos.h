#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "Nodo.h"

struct Punto {
    std::string nombre;
    std::string color;
    int x, y;

    Punto(const std::string& nombre = "", const std::string& color = "", int x = 0, int y = 0)
        : nombre(nombre), color(color), x(x), y(y) {
    }
};

// Cargar una ruta desde el archivo
void cargarRutaDesdeArchivo(const std::string& nombreRuta, NodoRuta<std::string>* ruta) {
    std::ifstream archivo(nombreRuta + ".txt");
    if (archivo.is_open()) {
        std::string nombrePunto, color;
        int x, y;
        while (archivo >> nombrePunto >> color >> x >> y) {
            Punto punto(nombrePunto, color, x, y);
            // Aquí debes agregar el punto a la ruta usando la lista
            // Suponiendo que tienes un método como push_backPunto
        }
        archivo.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo para cargar la ruta.\n";
    }
}

// Guardar una ruta en el archivo
void guardarRutaEnArchivo(const std::string& nombreRuta, NodoRuta<std::string>* ruta) {
    std::ofstream archivo(nombreRuta + ".txt");
    if (archivo.is_open()) {
        Nodo<Punto>* puntoActual = ruta->puntos;
        while (puntoActual != nullptr) {
            archivo << puntoActual->dato.nombre << " "
                << puntoActual->dato.color << " "
                << puntoActual->dato.x << " "
                << puntoActual->dato.y << "\n";
            puntoActual = puntoActual->siguiente;
        }
        archivo.close();
    }
    else {
        std::cerr << "No se pudo guardar la ruta en el archivo.\n";
    }
}
