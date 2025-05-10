#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include "ListaReservas.h"
#include "Administrador.h"
#include "GestorRutas.h"
#include "GestorPasajero.h"
#include "Pasajero.h"
#include "Cliente.h"

using namespace std;

class Menu
{
private:
    int diaActual, mesActual, anioActual;
    void obtenerFechaActual() {
        time_t tiempoActual = time(nullptr);
        tm tiempoLocal;
        localtime_s(&tiempoLocal, &tiempoActual);
        diaActual = tiempoLocal.tm_mday;
        mesActual = tiempoLocal.tm_mon + 1;
        anioActual = tiempoLocal.tm_year + 1900;
    }

public:
    Menu() {
        obtenerFechaActual();
    }

    ~Menu() {}
    void guardarUsuarioArchivo(Usuario& user) {
        ofstream archivo("usuarios.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }
        archivo << user.getNombre() << "|" << user.getApellido() << "|"
            << user.getEmail() << "|"
            << user.getContraseña() << "|"
            << user.getTipo() << endl;
        archivo.close();
        cout << "Usuario registrado con exito." << endl;
    }


    void registro() {
        string nombre, apellido, email, dni, contraseña;

        cout << "\t\t\t----REGISTRO DE USUARIO----" << endl;

        // Función lambda para validar campos
        auto leerCampo = [](const string& mensaje) -> string {
            string valor;
            do {
                cout << mensaje;
                getline(cin, valor);
                if (valor.empty()) {
                    cout << "\t\t\tEs obligatorio llenar este campo.\n";
                }
            } while (valor.empty());
            return valor;
            };

        nombre = leerCampo("\t\t\tNombre: ");
        apellido = leerCampo("\t\t\tApellido: ");
        dni = leerCampo("\t\t\tDNI: ");
        email = leerCampo("\t\t\tEmail: ");
        contraseña = leerCampo("\t\t\tContrasena: ");
        Cliente cliente(nombre, apellido, email, contraseña);
        guardarUsuarioArchivo(cliente);

    }
    Usuario* iniciarSesion() {
        string emailIngresado, contrasenaIngresada;
        cout << "\t\t\t-----INICIO DE SESION----" << endl;
        cout << " " << endl;

        do {
            cout << "\t\t Email: ";
            getline(cin, emailIngresado);
            if (emailIngresado.empty()) {
                cout << "Es obligatorio llenar este campo.\n";
            }
        } while (emailIngresado.empty());

        do {
            cout << "\t\t Contrasena: ";
            getline(cin, contrasenaIngresada);
            if (contrasenaIngresada.empty()) {
                cout << "Es obligatorio llenar este campo.\n";
            }
        } while (contrasenaIngresada.empty());

        ifstream archivo("usuarios.txt");
        if (!archivo) {
            cout << "No hay usuarios registrados." << endl;
            return nullptr;
        }

        string linea;
        while (getline(archivo, linea)) {
            size_t pos1 = linea.find('|');
            size_t pos2 = linea.find('|', pos1 + 1);
            size_t pos3 = linea.find('|', pos2 + 1);
            size_t pos4 = linea.find('|', pos3 + 1);

            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos && pos4 != string::npos) {
                string nombre = linea.substr(0, pos1);
                string apellido = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                string emailArchivo = linea.substr(pos2 + 1, pos3 - pos2 - 1);
                string contrasenaArchivo = linea.substr(pos3 + 1, pos4 - pos3 - 1);
                string tipo = linea.substr(pos4 + 1);

                if (emailIngresado == emailArchivo && contrasenaIngresada == contrasenaArchivo) {
                    if (tipo == "Cliente") {
                        Cliente* cliente = new Cliente(nombre, apellido, emailArchivo, contrasenaArchivo);
                        return cliente;
                    }
                    else if (tipo == "Administrador") {
                        Administrador* admin = new Administrador(nombre, apellido, emailArchivo, contrasenaArchivo);
                        return admin;
                    }
                }
            }
        }

        cout << "Usuario no encontrado, intente nuevamente.\n";
        return nullptr;
    }

    void menuAdministrador(Usuario* user) {
        int opcion;
        do {
            system("cls");
            user->mostrarBienvenida();

            cout << "\t\t\t------------ MENU ADMINISTRADOR ------------" << endl;
            cout << "  " << endl;
            cout << "\t\t\t1. Buscar reserva" << endl;
            cout << "\t\t\t2. Modificar reserva" << endl;
            cout << "\t\t\t3. Eliminar reserva" << endl;
            cout << "\t\t\t4. Ordenar reservas" << endl;
            cout << "\t\t\t5. Salir" << endl;
            cout << "\t\t\t-------------------------------------------" << endl;
            cout << "\t\t\tSeleccione una opción: ";
            cin >> opcion;
            cin.ignore();


            switch (opcion) {
            case 1: cout << "\t\t\tBuscar :)\n"; break;
            case 2: cout << "\t\t\tModificar :)\n"; break;
            case 3: cout << "\t\t\tEliminar :)\n"; break;
            case 4: cout << "\t\t\tOrdenar :)\n"; break;
            case 5: cout << "\t\t\tSaliendo al menú principal...\n"; break;
            default: cout << "\t\t\tOpción inválida.\n";
            }

            system("pause");
        } while (opcion != 5);
    }


    void realizarReservas() {
        string fecha;
        string opcionAdicionales;
        int opcionRuta;
        int origen, destino;
        int cantPasajeros;
        string archivoReservas = "reservas.txt";
        ListaReservas listaReservas;
        MapaRutas rutas;
        GestorRutas gestorRutas;
        /*
        PrecioTotal precio;*/

        auto validarFecha = [this](string& fecha) {
            if (fecha.length() != 10 || fecha[2] != '/' || fecha[5] != '/') {
                cout << "\t\t\tFecha invalida. Formato correcto: DD/MM/AAAA" << endl;
                return false;
            }
            int dia = stoi(fecha.substr(0, 2));
            int mes = stoi(fecha.substr(3, 2));
            int anio = stoi(fecha.substr(6, 4));

            if (dia < 1 || dia>31 || mes < 1 || mes>12 || anio < 2023 || anio>2026) {
                cout << "\t\t\t\tFecha invalida. " << endl;
                return false;
            }
            if (anio < anioActual ||
                (anio == anioActual && mes < mesActual) ||
                (anio == anioActual && mes == mesActual && dia <= diaActual)) {
                cout << "\t\t\t\tLa fecha debe ser posterior a la fecha actual." << endl;
                return false;
            }
            return true;
            };

        cout << "\t\t\t----------------------------- RESERVA -----------------------------  " << endl;
        cout << "   " << endl;
        while (true) {
            cout << "\t\t\t   Ingrese fecha del vuelo (DD/MM/AAAA): ";
            cin >> fecha;
            if (validarFecha(fecha)) {
                break;
            }
        }
        cout << "\t\t\t   Seleccione ciudad de origen: " << endl;
        origen = rutas.seleccionarOrigen();
        if (origen == -1) {
            cout << "\t\t\t   Reserva cancelada. " << endl;
            return;
        }
        cout << "\t\t\t   Seleccione ciudad de destino:    " << endl;
        destino = rutas.seleccionarDestino(origen);
        if (destino == -1) {
            cout << "\t\t\t| Reserva cancelada." << endl;
            return;
        }
        cout << "\t\t\t   Ingrese la cantidad de pasajeros: ";
        cin >> cantPasajeros;

        //Reserva reserva(fecha, rutas.getNombreLugar(destino), rutas.getNombreLugar(origen), cantPasajeros);
        //Pasajero* pasajeros = new Pasajero[cantPasajeros];
        //listaReservas.agregarReserva(reserva);
        //listaReservas.guardarListaEnArchivo(nombreArchivo);

        cout << "\t\t\t Buscar.... " << endl;

        system("pause>0");
        system("cls");

        string claveRuta = rutas.getSiglasLugar(origen) + "-" + rutas.getSiglasLugar(destino);
        gestorRutas.cargarDesdeArchivo("rutas.txt", claveRuta);
        gestorRutas.mostrarRutas();

        cin >> opcionRuta;
        Ruta rutaSeleccionada = gestorRutas.getRuta(opcionRuta - 1);
        //gestor.guardarRutaEnArchivo(opcionRuta-1, nombreArchivo);

        system("pause>0");
        system("cls");

        cin.ignore();
        cout << "\t\t\t----------------------------- Equipajes y Asientos -------------------------------\n";
        cout << "\t\t\t\t-----------------------------------------------------------------" << endl;
        cout << "\t\t\t\t| Deseas agregar equipaje personalizado por pasajero ?  |" << endl;
        cout << "\t\t\t\t-----------------------------------------------------------------" << endl;
        cout << "\t\t\t      SI/NO: ";
        getline(cin, opcionAdicionales);

        for (int i = 0; i < opcionAdicionales.length(); i++) {
            opcionAdicionales[i] = toupper(opcionAdicionales[i]);
        }
        vector<Pasajero> pasajeros;
        if (opcionAdicionales == "SI")
        {
            for (int i = 0; i < cantPasajeros; i++)
            {
                cout << "\n\t\t\tPasajero " << i + 1 << ": " << endl;
                Pasajero pasajero(rutaSeleccionada.getPrecio());
                pasajero.seleccionarEquipaje();
                pasajero.seleccionarAsiento();
                pasajeros.push_back(pasajero);
            }
        }
        else if (opcionAdicionales == "NO")
        {
            Pasajero pasajero(rutaSeleccionada.getPrecio());
            pasajero.seleccionarEquipaje();
            pasajero.seleccionarAsiento();
            pasajeros.push_back(pasajero);
            for (int i = 0;i < cantPasajeros - 1;i++) {
                Pasajero clon(rutaSeleccionada.getPrecio(), pasajero.getPrecioCabina(), pasajero.getPrecioBodega(), pasajero.getPrecioAsiento());
                pasajeros.push_back(clon);
            }
        }

        Reserva reserva(fecha, rutas.getNombreLugar(destino), rutas.getNombreLugar(origen), cantPasajeros);
        listaReservas.agregarReserva(reserva);
        listaReservas.guardarListaEnArchivo(archivoReservas);
        gestorRutas.guardarRutaEnArchivo(opcionRuta - 1, archivoReservas);

		for (int i = 0;i < pasajeros.size();i++) {
			pasajeros[i].pedirDatosPersonales();
			pasajeros[i].setIdReserva(reserva.getIdReserva());
			pasajeros[i].mostrarResumen();
		}
		string archivoPasajeros = "pasajeros.txt";
		GestorPasajero gestorPasajero(pasajeros);
		gestorPasajero.guardarPasajerosEnArchivo(archivoPasajeros);

    }
    void menuPricipal() {
        system("cls");
        cout << "\t\t\t ----------------- MENU ----------------- " << endl;
        cout << "\t\t\t|                                       |" << endl;
        cout << "\t\t\t|1. Iniciar sesion                      |" << endl;
        cout << "\t\t\t|2. Iniciar Reserva sin cuenta          |" << endl;
        cout << "\t\t\t|3. Salir                               |" << endl;
        cout << "\t\t\t|                                       |" << endl;
        cout << "\t\t\t   Seleccione una opcion: ";
    }
    void deseasRegistrarte() {
        system("cls");
        cout << "\t\t\t-----------------------------------------------------------------" << endl;
        cout << "\t\t\t| Deseas Registrarte?                                            |" << endl;
        cout << "\t\t\t| Recuerda que registrandote obtienes acceso a ofertas ademas de |" << endl;
        cout << "\t\t\t| lo ultimo en vuelos, novedades, promociones y mucho más.       |" << endl;
        cout << "\t\t\t| Si no deseas registrarte, puedes continuar sin cuenta.         |" << endl;
        cout << "\t\t\t-----------------------------------------------------------------" << endl;
        cout << "\t\t\t      SI/NO: ";
    }

};
