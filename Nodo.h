#pragma once

#include <string>

// Nodo para los puntos
template <typename T>
class Nodo {
public:
    T dato;               // Dato (punto)
    Nodo* siguiente;      // Puntero al siguiente nodo
    Nodo* anterior;       // Puntero al nodo anterior

    Nodo(const T& dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}
};

// Nodo para las rutas
template <typename T>
class NodoRuta {
public:
    T ruta;               // Ruta
    NodoRuta* siguiente;  // Puntero al siguiente nodo de la lista externa
    NodoRuta* anterior;   // Puntero al nodo anterior de la lista externa
    Nodo<T>* puntos;      // Lista de puntos (nodos internos)

    NodoRuta(const T& ruta) : ruta(ruta), siguiente(nullptr), anterior(nullptr), puntos(nullptr) {}
};
