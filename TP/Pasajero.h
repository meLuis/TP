#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Pasajero {
private:
    string nombre;
    string apellido;
    int dni;
    string numeroAsiento;
    float precioCabina;
    float precioBodega;
    float precioAsiento;
    float precioRuta;

    bool pedirOpcion(string mensaje) {
        char opcion;
        cout << mensaje;cin >> opcion;
        return (opcion == 's' || opcion == 'S');
    }

public:

    Pasajero(float precioRuta) : nombre(""), apellido(""), dni(0), numeroAsiento(""),
        precioCabina(0.0f), precioBodega(0.0f), precioAsiento(0.0f), precioRuta(precioRuta) {
    }

    Pasajero(float precioRuta, float precioCabina, float precioBodega, float precioAsiento)
        : nombre(""), apellido(""), dni(0), numeroAsiento(""),
        precioCabina(precioCabina), precioBodega(precioBodega),
        precioAsiento(precioAsiento), precioRuta(precioRuta) {
    }

    void setNombre(string nombre) { this->nombre = nombre; }
    void setApellido(string apellido) { this->apellido = apellido; }
    void setDni(int dni) { this->dni = dni; }
    void setNumeroAsiento(string numeroAsiento) { this->numeroAsiento = numeroAsiento; }
    void setPrecioCabina(float precioCabina) { this->precioCabina = precioCabina; }
    void setPrecioBodega(float precioBodega) { this->precioBodega = precioBodega; }
    void setPrecioAsiento(float precioAsiento) { this->precioAsiento = precioAsiento; }

    string getNombre() { return this->nombre; }
    string getApellido() { return this->apellido; }
    int getDni() { return this->dni; }
    string getNumeroAsiento() { return this->numeroAsiento; }
    float getPrecioCabina() { return this->precioCabina; }
    float getPrecioBodega() { return this->precioBodega; }
    float getPrecioAsiento() { return this->precioAsiento; }
    float getPrecioRuta() { return this->precioRuta; }

    void seleccionarEquipaje() {
        if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de cabina? (s/n): ")) this->precioCabina = 97.04;
        if (pedirOpcion("\t\t\t\t\tDesea agregar equipaje de bodega? (s/n): ")) this->precioBodega = 102.54;
    }

    void seleccionarAsiento() {
        if (pedirOpcion("\t\t\t\t\tDesea elegir un asiento? (s/n): ")) {
            cout << "\t\t\t|\t\tIngrese el numero de asiento: ";
            cin >> this->numeroAsiento;
            this->precioAsiento = 50.0f;
        }
    }

    float calcularPrecioAdicional() {
        float precioAdicional = 0.0f;
        if (this->precioCabina) precioAdicional += this->precioCabina;
        if (this->precioBodega) precioAdicional += this->precioBodega;
        if (!this->numeroAsiento.empty()) precioAdicional += this->precioAsiento;
        return precioAdicional;
    }

    float calcularPrecioPasajero() {
        float precioAdicional = this->calcularPrecioAdicional();
        return this->precioRuta + precioAdicional + (0.18f * (this->precioRuta + precioAdicional)) + 45.0f + 30.0f;
    }

    void mostrarResumen() {
        cout << "\t\t\t|\t\tResumen de la reserva:" << endl;
        cout << "\t\t\t|\t\tNombre: " << this->nombre << endl;
        cout << "\t\t\t|\t\tApellido: " << this->apellido << endl;
        cout << "\t\t\t|\t\tDNI: " << this->dni << endl;
        cout << "\t\t\t|\t\tEquipaje de cabina: " << (this->precioCabina ? "Sí" : "No") << endl;
        cout << "\t\t\t|\t\tEquipaje de bodega: " << (this->precioBodega ? "Sí" : "No") << endl;
        cout << "\t\t\t|\t\tAsiento elegido: " << (this->numeroAsiento.empty() ? "No" : this->numeroAsiento) << endl;
        cout << "\t\t\t|\t\tPrecio total: S/." << this->calcularPrecioPasajero() << endl;
    }
};
