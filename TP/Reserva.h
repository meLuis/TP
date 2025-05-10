#pragma once
#include <iostream>
#include <fstream>
#include "MapaRutas.h"

using namespace std;

class Reserva
{
private:
	int idReserva;
	string fecha;
	string destino;
	string origen;
	int cantPasajeros;
public:
	Reserva(string f, string d, string o, int c) : fecha(f), destino(d), origen(o), cantPasajeros(c) {
		idReserva = 0;
	}

	~Reserva() {}
	void setIdReserva(int id) { idReserva = id; }
	int getIdReserva() { return idReserva; }
	string getFecha() { return fecha; }
	string getDestino() { return destino; }
	string getOrigen() { return origen; }
	int getCantPasajeros() { return cantPasajeros; }

	void guardarEnArchivo(ofstream& archivo) const {
		archivo << idReserva << "|" << fecha << "|" << destino << "|" << origen << "|" << cantPasajeros << "|";
	}



};
