#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Pasajero.h"
#include "GestorRutas.h"

using namespace std;

class Pago
{
private:
    vector<Pasajero> pasajeros;
    Reserva reserva;
    Ruta rutaSeleccionada;

    float total;
    int idReserva;

public:
    Pago(float total, const vector<Pasajero>& pasajeros, const Reserva& reserva, const Ruta& rutaSeleccionada, int idReserva)
        : total(total), pasajeros(pasajeros), reserva(reserva), rutaSeleccionada(rutaSeleccionada), idReserva(idReserva) {
    }

    void generarComprobante() {
        ofstream archivo("comprobante_pago.txt");
        if (!archivo) {
            cout << "Error al crear el comprobante." << endl;
            return;
        }

        archivo << "--------------------- COMPROBANTE DE PAGO ---------------------\n";
        archivo << "ID Reserva: " << idReserva << "\n";
        archivo << "Fecha: " << reserva.getFecha() << "\n";
        archivo << "Origen: " << reserva.getOrigen() << "\t Destino: " << reserva.getDestino() << "\n";
        archivo << "Hora Salida: " << rutaSeleccionada.getHoraInicio() << " - " << rutaSeleccionada.getHoraFin() << "\n";
        archivo << "Detalles de Pasajeros:\n";

        for (auto& pasajero : pasajeros) {
            archivo << "--------------------------------------------------\n";
            archivo << "Nombre: " << pasajero.getNombre() << "\n";
            archivo << "Apellido: " << pasajero.getApellido() << "\n";
            archivo << "DNI: " << pasajero.getDni() << "\n";
            archivo << "Asiento: " << pasajero.getNumeroAsiento() << "\n";
            archivo << "Precio Total: S/. " << pasajero.calcularPrecioPasajero() << "\n";
        }

        archivo << "--------------------------------------------------\n";
        archivo << "Total a Pagar: S/. " << total << "\n";
        archivo << "Gracias por su compra!\n";
        archivo.close();

        cout << "Comprobante generado: comprobante_pago.txt" << endl;
    }
};
