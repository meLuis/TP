#pragma once
#include <iostream>
#include "Reserva.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace  std;
struct NodoReserva {
    Reserva reserva;
    NodoReserva* siguiente;

    NodoReserva(const Reserva& r) : reserva(r), siguiente(nullptr) {}
};
class ListaReservas {
private:
    NodoReserva* cabeza;
    set<int> idsExistentes;

public:
    ListaReservas() : cabeza(nullptr) {
        srand(time(0));
        cargarIdsExistentes();
    }

    ~ListaReservas() {
        NodoReserva* actual = cabeza;
        while (actual != nullptr) {
            NodoReserva* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar;
        }
    }
    void cargarIdsExistentes() {
        ifstream archivo("reserva.txt");
        if (!archivo) return;

        string linea;
        while (getline(archivo, linea)) {
            size_t pos = linea.find('|');
            if (pos != string::npos) {
                int id = stoi(linea.substr(0, pos));
                idsExistentes.insert(id);
            }
        }
        archivo.close();
    }

    int generarIdUnico() {
        int id;
        do {
            id = 1000 + rand() % 9000;
        } while (idsExistentes.find(id) != idsExistentes.end());
        idsExistentes.insert(id);
        return id;
    }

    void agregarReserva(Reserva& reserva) {
        reserva.setIdReserva(generarIdUnico());
        NodoReserva* nuevoNodo = new NodoReserva(reserva);
        if (!cabeza) {
            cabeza = nuevoNodo;
        }
        else {
            NodoReserva* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    void guardarListaEnArchivo(string& nombreArchivo) {
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo para guardar reservas." << endl;
            return;
        }
        NodoReserva* actual = cabeza;
        while (actual != nullptr) {
            actual->reserva.guardarEnArchivo(archivo);
            actual = actual->siguiente;
        }
        archivo.close();
    }
};