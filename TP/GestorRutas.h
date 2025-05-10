#pragma once
#include "Ruta.h"
#include <fstream>

class GestorRutas {
private:
    vector<Ruta> rutas;

public:
    Ruta getRuta(int indice) {
        if (indice < 0 || indice >= rutas.size()) {
            cout << "Índice fuera de rango." << endl;
            return Ruta();
        }
        return rutas[indice];
    }
    void cargarDesdeArchivo(string archivoNombre, string origenDestinoID) {
        rutas.clear();

        fstream archivo(archivoNombre, ios::in);
        if (!archivo.is_open()) {
            cout << "Error abriendo el archivo." << endl;
            return;
        }

        string linea;
        bool leyendo = false;

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            if (linea[0] == '#') {
                leyendo = (linea == "#" + origenDestinoID);
                continue;
            }

            if (leyendo) {
                vector<string> datos;
                string actual = "";

                for (int i = 0; i < linea.length(); i++) {
                    if (linea[i] == ',') {
                        datos.push_back(actual);
                        actual = "";
                    }
                    else {
                        actual += linea[i];
                    }
                }
                datos.push_back(actual);

                if (datos.size() >= 5) {
                    string horaInicio = datos[0];
                    string horaFin = datos[1];
                    string duracion = datos[2];
                    float precio = stof(datos.back());

                    vector<string> paradas;
                    for (int j = 3; j < datos.size() - 1; j++) {
                        paradas.push_back(datos[j]);
                    }

                    Ruta r(horaInicio, horaFin, duracion, paradas, precio);
                    rutas.push_back(r);
                }
            }
        }

        archivo.close();
    }

    void mostrarRutas() {
        cout << "\t\t\t----------------------------- Vuelos Disponibles ------------------------------- ";
        for (int i = 0; i < rutas.size(); i++) {
            cout << "\n\n\t\t\t\t[" << i + 1 << "] ";
            rutas[i].mostrar();
        }
        cout << "\t\t\tElige tu vuelo: ";
    }

    void ordenarPorPrecio() {
        for (int i = 0; i < rutas.size() - 1; i++) {
            for (int j = 0; j < rutas.size() - i - 1; j++) {
                if (rutas[j].getPrecio() > rutas[j + 1].getPrecio()) {
                    Ruta temp = rutas[j];
                    rutas[j] = rutas[j + 1];
                    rutas[j + 1] = temp;
                }
            }
        }
    }

    void ordenarPorDuracion() {
        for (int i = 0; i < rutas.size() - 1; i++) {
            for (int j = 0; j < rutas.size() - i - 1; j++) {
                if (rutas[j].getDuracionEnMinutos() > rutas[j + 1].getDuracionEnMinutos()) {
                    Ruta temp = rutas[j];
                    rutas[j] = rutas[j + 1];
                    rutas[j + 1] = temp;
                }
            }
        }
    }
    void guardarRutaEnArchivo(int indice, string nombreArchivo) {
        if (indice < 0 || indice >= rutas.size()) {
            cout << "Índice fuera de rango." << endl;
            return;
        }
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo.is_open()) {
            cout << "Error abriendo el archivo para guardar la ruta." << endl;
            return;
        }

        archivo << rutas[indice].getHoraInicio() << "|";
        archivo << rutas[indice].getHoraFin() << "|";
        archivo << rutas[indice].getDuracion() << "|";
        archivo << rutas[indice].getPrecio() << "\n";
        archivo.close();
    }

};