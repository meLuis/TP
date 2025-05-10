#pragma once
#include "Pasajero.h"
#include <fstream>

class GestorPasajero {
private:
	vector<Pasajero> pasajeros;
public:
    Pasajero getPasajero(int indice) {
        if (indice < 0 || indice >= pasajeros.size()) {
            cout << "Índice fuera de rango." << endl;
            return Pasajero(0);
        }
        return pasajeros[indice];
    }

};