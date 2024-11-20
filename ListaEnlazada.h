#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include "Nodo.h"

// Clase que representa una lista doblemente enlazada de rutas y puntos
template <typename T>
class ListaEnlazada {
public:
    ListaEnlazada();
    ~ListaEnlazada();

    void push_backRuta(const T& ruta);
    void push_backPunto(const T& punto, NodoRuta<T>* ruta);
    void remove_if(std::function<bool(const T&)> condicionRuta, std::function<bool(const T&)> condicionPunto = nullptr);
    size_t size() const;
    bool empty() const;

    NodoRuta<T>* begin() const;
    NodoRuta<T>* end() const;

    // Métodos de acceso
    NodoRuta<T>* getCabezaRuta() const { return cabezaRuta; }
    NodoRuta<T>* getColaRuta() const { return colaRuta; }

private:
    NodoRuta<T>* cabezaRuta;
    NodoRuta<T>* colaRuta;
    size_t tamanoRuta;
};

template <typename T>
ListaEnlazada<T>::ListaEnlazada() : cabezaRuta(nullptr), colaRuta(nullptr), tamanoRuta(0) {}

template <typename T>
ListaEnlazada<T>::~ListaEnlazada() {
    NodoRuta<T>* actualRuta = cabezaRuta;
    while (actualRuta != nullptr) {
        NodoRuta<T>* tempRuta = actualRuta;
        actualRuta = actualRuta->siguiente;
        // Eliminar los puntos de la ruta
        Nodo<T>* actualPunto = tempRuta->puntos;
        while (actualPunto != nullptr) {
            Nodo<T>* tempPunto = actualPunto;
            actualPunto = actualPunto->siguiente;
            delete tempPunto;
        }
        delete tempRuta;
    }
}

template <typename T>
void ListaEnlazada<T>::push_backRuta(const T& ruta) {
    NodoRuta<T>* nuevoNodoRuta = new NodoRuta<T>(ruta);
    if (cabezaRuta == nullptr) {
        cabezaRuta = colaRuta = nuevoNodoRuta;
    }
    else {
        colaRuta->siguiente = nuevoNodoRuta;
        nuevoNodoRuta->anterior = colaRuta;
        colaRuta = nuevoNodoRuta;
    }
    ++tamanoRuta;
}

template <typename T>
void ListaEnlazada<T>::push_backPunto(const T& punto, NodoRuta<T>* ruta) {
    Nodo<T>* nuevoNodoPunto = new Nodo<T>(punto);
    if (ruta->puntos == nullptr) {
        ruta->puntos = nuevoNodoPunto;
    }
    else {
        Nodo<T>* colaPuntos = ruta->puntos;
        while (colaPuntos->siguiente != nullptr) {
            colaPuntos = colaPuntos->siguiente;
        }
        colaPuntos->siguiente = nuevoNodoPunto;
        nuevoNodoPunto->anterior = colaPuntos;
    }
}

template <typename T>
void ListaEnlazada<T>::remove_if(std::function<bool(const T&)> condicionRuta, std::function<bool(const T&)> condicionPunto) {
    NodoRuta<T>* actualRuta = cabezaRuta;
    while (actualRuta != nullptr) {
        if (condicionRuta(actualRuta->ruta)) {
            // Eliminar los puntos de la ruta si la condición se cumple
            Nodo<T>* actualPunto = actualRuta->puntos;
            while (actualPunto != nullptr) {
                if (condicionPunto != nullptr && condicionPunto(actualPunto->dato)) {
                    Nodo<T>* tempPunto = actualPunto;
                    actualPunto = actualPunto->siguiente;
                    delete tempPunto;
                }
                else {
                    actualPunto = actualPunto->siguiente;
                }
            }
            // Eliminar la ruta
            if (actualRuta == cabezaRuta) {
                cabezaRuta = actualRuta->siguiente;
                if (cabezaRuta != nullptr) cabezaRuta->anterior = nullptr;
            }
            if (actualRuta == colaRuta) {
                colaRuta = actualRuta->anterior;
                if (colaRuta != nullptr) colaRuta->siguiente = nullptr;
            }
            NodoRuta<T>* tempRuta = actualRuta;
            actualRuta = actualRuta->siguiente;
            delete tempRuta;
            --tamanoRuta;
        }
        else {
            actualRuta = actualRuta->siguiente;
        }
    }
}

template <typename T>
size_t ListaEnlazada<T>::size() const {
    return tamanoRuta;
}

template <typename T>
bool ListaEnlazada<T>::empty() const {
    return tamanoRuta == 0;
}

template <typename T>
NodoRuta<T>* ListaEnlazada<T>::begin() const {
    return cabezaRuta;
}

template <typename T>
NodoRuta<T>* ListaEnlazada<T>::end() const {
    return nullptr;
}
