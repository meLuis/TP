#pragma once
#include "Nodo.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Pago.h"

using namespace std;
template <typename T>
class ColaPagos {
private:
    Nodo<T>* inicio;
    Nodo<T>* fin;

public:
    ColaPagos()
    {
        inicio = nullptr;
        fin = nullptr;
    }

    ~ColaPagos()
    {
        while (!esVacia()) {
            dequeue();
        }
    }

    void enqueue(const T& dato)
    {
        Nodo<T>* nodo = new Nodo<T>(dato);
        if (esVacia())
        {
            inicio = nodo;
            fin = inicio;
        }
        else
        {
            fin->siguiente = nodo;
            fin = nodo;
        }
    }

    T dequeue()
    {
        if (esVacia())
        {
            throw runtime_error("La cola de pagos est� vac�a");
        }

        T dato = inicio->dato;
        Nodo<T>* temp = inicio;

        if (inicio == fin)
        {
            inicio = nullptr;
            fin = nullptr;
        }
        else
        {
            inicio = inicio->siguiente;
        }

        delete temp; // Liberar memoria del nodo
        return dato;
    }

    bool esVacia() const
    {
        return (inicio == nullptr);
    }

    size_t obtenerTamanoCola() const
    {
        size_t tamano = 0;
        Nodo<T>* actual = inicio;
        while (actual != nullptr)
        {
            tamano++;
            actual = actual->siguiente;
        }
        return tamano;
    }

    void procesarPagos()
    {
        while (!esVacia())
        {
            T pago = dequeue();
            cout << "\t\t\tProcesando pago..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            pago.generarComprobante();
            cout << "\t\t\tPago procesado exitosamente." << endl;
            system("pause>0");
            system("cls");
            pago.mostrarComprobante();

        }
    }
};
