#pragma once

#include <iostream>
#include <functional>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo* siguiente;

    Nodo(const T& dato) : dato(dato), siguiente(nullptr) {}
};

template <typename T>
class ListaEnlazada {
public:
    ListaEnlazada();
    ~ListaEnlazada();

    void push_back(const T& valor);
    void remove_if(std::function<bool(const T&)> condicion);
    size_t size() const;
    bool empty() const;

    Nodo<T>* begin() const;
    Nodo<T>* end() const;

    // Métodos de acceso a cabeza y cola
    Nodo<T>* getCabeza() const { return cabeza; }
    void setCabeza(Nodo<T>* nuevoCabeza) { cabeza = nuevoCabeza; }
    Nodo<T>* getCola() const { return cola; }
    void setCola(Nodo<T>* nuevoCola) { cola = nuevoCola; }

private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    size_t tamano;
};


template <typename T>
ListaEnlazada<T>::ListaEnlazada() : cabeza(nullptr), cola(nullptr), tamano(0) {}

template <typename T>
ListaEnlazada<T>::~ListaEnlazada() {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

template <typename T>
void ListaEnlazada<T>::push_back(const T& valor) {
    Nodo<T>* nuevoNodo = new Nodo<T>(valor);
    if (cabeza == nullptr) {
        cabeza = cola = nuevoNodo;
    }
    else {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
    ++tamano;
}

template <typename T>
void ListaEnlazada<T>::remove_if(std::function<bool(const T&)> condicion) {
    Nodo<T>* actual = cabeza;
    Nodo<T>* anterior = nullptr;

    while (actual != nullptr) {
        if (condicion(actual->dato)) {
            Nodo<T>* temp = actual;
            if (anterior == nullptr) {
                cabeza = actual->siguiente;
            }
            else {
                anterior->siguiente = actual->siguiente;
            }
            if (actual == cola) {
                cola = anterior;
            }
            actual = actual->siguiente;
            delete temp;
            --tamano;
        }
        else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}

template <typename T>
size_t ListaEnlazada<T>::size() const {
    return tamano;
}

template <typename T>
bool ListaEnlazada<T>::empty() const {
    return tamano == 0;
}

template <typename T>
Nodo<T>* ListaEnlazada<T>::begin() const {
    return cabeza;
}

template <typename T>
Nodo<T>* ListaEnlazada<T>::end() const {
    return nullptr;
}