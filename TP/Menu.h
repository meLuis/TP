#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include "ListaReservas.h"
#include "Administrador.h"
#include "GestorRutas.h"
#include "GestorPasajero.h"
#include "Cliente.h"
#include "ColaPagos.h"

#include <limits>
using namespace std;

class Menu
{
private:
    ColaPagos <Pago>colaPagos;
    ListaReservas<Reserva> listaReservas;
    string fecha;
    string opcionAdicionales;
    int opcionRuta;
    int origen, destino;
    int cantPasajeros;
    string archivoReservas = "Archivos//reserva.txt";
    string archivoPasajeros = "Archivos//pasajeros.txt";
    MapaRutas rutas;
    GestorRutas gestorRutas;
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
        ofstream archivo("Archivos//usuarios.txt", ios::app);
        if (!archivo) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }
        archivo << user.getNombre() << "|" << user.getApellido() << "|"
            << user.getEmail() << "|"
            << user.getContrase�a() << "|"
            << user.getTipo() << endl;
        archivo.close();
        cout << "Usuario registrado con exito." << endl;
    }


    void registro() {
        string nombre, apellido, email, dni, contrase�a;

        cout << "\t\t\t----REGISTRO DE USUARIO----" << endl;

        // Funci�n lambda para validar campos
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
        contrase�a = leerCampo("\t\t\tContrasena: ");
        Cliente cliente(nombre, apellido, email, contrase�a);
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

        ifstream archivo("Archivos//usuarios.txt");
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

    void anularReserva(const vector<Reserva>& reservas, const string& archivoReservas) {
        vector<string> todasLasLineas;
        ifstream archivoLectura(archivoReservas);
        if (archivoLectura.is_open()) {
            string linea;
            while (getline(archivoLectura, linea)) {
                todasLasLineas.push_back(linea);
            }
            archivoLectura.close();
        }
        else
        {
            cout << "\t\t\t Error al abrir el archivo." << endl;
            return;
        }

        //IA
        // Ahora abrir el archivo para escribir
        ofstream archivoEscritura(archivoReservas, ios::trunc);

        if (!archivoEscritura.is_open()) {
            cout << "\t\t\t Error al abrir el archivo para escritura." << endl;
            return;
        }

        // Mantener un registro de los IDs de reserva que han sido anulados
        vector<int> idsAnulados;
        for (const Reserva& r : reservas) {
            if (r.getEstado() == "Anulada") {
                idsAnulados.push_back(r.getIdReserva());
            }
        }

        // Procesar cada l�nea del archivo
        for (const string& linea : todasLasLineas) {
            // Verificar si esta l�nea es una reserva (contiene suficientes '|')
            int contadorDelimitadores = 0;
            for (char c : linea) {
                if (c == '|') contadorDelimitadores++;
            }

            // Si tiene suficientes delimitadores para ser una reserva
            if (contadorDelimitadores >= 5) {
                // Extraer el ID de la reserva
                size_t pos = linea.find('|');
                if (pos != string::npos) {
                    string idStr = linea.substr(0, pos);
                    try {
                        int id = stoi(idStr);

                        // Si esta reserva debe ser anulada
                        bool debeAnular = false;
                        for (int idAnulado : idsAnulados) {
                            if (id == idAnulado) {
                                debeAnular = true;
                                break;
                            }
                        }

                        if (debeAnular) {
                            // Reconstruir la l�nea con el estado "Anulada"
                            vector<string> campos;
                            string temp = linea;
                            size_t posTemp;

                            // Dividir la l�nea en campos
                            while ((posTemp = temp.find('|')) != string::npos) {
                                campos.push_back(temp.substr(0, posTemp));
                                temp = temp.substr(posTemp + 1);
                            }
                            campos.push_back(temp); // �ltimo campo

                            // Cambiar el campo de estado (6� elemento, �ndice 5)
                            if (campos.size() > 5) {
                                campos[5] = "Anulada";
                            }

                            // Reconstruir la l�nea
                            string nuevaLinea = "";
                            for (size_t i = 0; i < campos.size(); i++) {
                                nuevaLinea += campos[i];
                                if (i < campos.size() - 1) nuevaLinea += "|";
                            }

                            archivoEscritura << nuevaLinea << endl;
                            continue; // Pasar a la siguiente l�nea
                        }
                    }
                    catch (const std::invalid_argument& e) {
                        // No es un ID v�lido, mantener la l�nea original
                    }
                }
            }

            // Si no es una reserva o no necesita ser modificada, escribir la l�nea original
            archivoEscritura << linea << endl;
        }

        archivoEscritura.close();
    }
    void buscarReservaPorID() {
        vector<Reserva> reservas = Reserva::leerReservasDesdeArchivo("reserva.txt");

        if (reservas.empty()) {
            cout << "\t\t\tNo hay reservas registradas." << endl;
            return;
        }

        int id;
        cout << "\t\t\tIngrese el ID de la reserva a buscar: ";
        cin >> id;
        cin.ignore();

        Reserva* encontrada = Reserva::buscarPorID(reservas, id);
        if (encontrada != nullptr) {
            cout << "\n\t\t\tReserva encontrada:\n";
            encontrada->mostrar();

            int opcion;
            cout << "\n\t\t\t1. Anular reserva" << endl;
            cout << "\t\t\t2. Retroceder" << endl;
            cout << "\t\t\tSeleccione una opcion: ";
            cin >> opcion;

            if (opcion == 1) {
                if (encontrada->getEstado() != "Anulada") {
                    encontrada->setEstado("Anulada");
                    anularReserva(reservas, "reserva.txt");
                    cout << "\t\t\t Reserva anulada correctamente." << endl;
                }
                else {
                    cout << "\t\t\t La reserva ya est� anulada." << endl;
                }
            }
            else if (opcion == 2) {
                cout << "\t\t\t?? Retrocediendo..." << endl;
            }
            else {
                cout << "\t\t\t Opci�n invalida." << endl;
            }

        }
        else {
            cout << "\t\t\tReserva no encontrada." << endl;
        }

        system("pause>0");
    }

	void menuAdministrador(Usuario* user) {
	    int opcion;
	    do {
	        system("cls");
	        user->mostrarBienvenida();

	        cout << "\t\t\t------------ MENU ADMINISTRADOR ------------" << endl;
	        cout << "  " << endl;
	        cout << "\t\t\t1. Buscar reserva" << endl;
	        cout << "\t\t\t2. Salir" << endl;
	        cout << "\t\t\t-------------------------------------------" << endl;
	        cout << "\t\t\tSeleccione una opci�n: ";

	        string input;
	        getline(cin, input);
	        try {
	            opcion = stoi(input);
	        } catch (...) {
	            opcion = -1; // Valor inv�lido
	        }

	        switch (opcion) {
	        case 1:
	            buscarReservaPorID();
	            system("pause>0");
	            break;
	        case 2:
	            break;
	        default:
	            cout << "\t\t\tOpcion invalida." << endl;
	            system("pause>0");
	        }
	    } while (opcion != 2);
	}


    void realizarReservas() {
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

        cout << "\t\t\t Buscar.... " << endl;

        system("pause>0");
        system("cls");

        string claveRuta = rutas.getSiglasLugar(origen) + "-" + rutas.getSiglasLugar(destino);
        gestorRutas.cargarDesdeArchivo("Archivos//rutas.txt", claveRuta);
        gestorRutas.imprimirRutas();

        cin >> opcionRuta;
        Ruta rutaSeleccionada = gestorRutas.getRuta(opcionRuta - 1);

        Reserva reserva(fecha, rutas.getNombreLugar(destino), rutas.getNombreLugar(origen), cantPasajeros);
        listaReservas.agregarReserva(reserva);

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
                Pasajero pasajero(rutaSeleccionada.getPrecio(),reserva.getIdReserva());
                pasajero.seleccionarEquipaje();
                pasajero.seleccionarAsiento();
                pasajeros.push_back(pasajero);
            }
        }
        else if (opcionAdicionales == "NO")
        {
            Pasajero pasajero(rutaSeleccionada.getPrecio(),reserva.getIdReserva());
            pasajero.seleccionarEquipaje();
            pasajero.seleccionarAsiento();
            pasajeros.push_back(pasajero);
            for (int i = 0;i < cantPasajeros - 1;i++) {
                Pasajero clon(rutaSeleccionada.getPrecio(), pasajero.getPrecioCabina(), pasajero.getPrecioBodega(), pasajero.getPrecioAsiento(),pasajero.getIdReserva());
                pasajeros.push_back(clon);
            }
        }

        system("pause>0");
        system("cls");

        cout << "\t\t\t----------------------------- Registro Pasajeros -------------------------------" << endl;
		for (int i = 0;i < pasajeros.size();i++) {
			cout << "\n\t\t\tPasajero " << i + 1 << ": " << endl;
			pasajeros[i].pedirDatosPersonales();
		}

        system("pause>0");
        system("cls");

        float precioTotal = 0.0f;
        for (auto& pasajero : pasajeros) {
            precioTotal += pasajero.calcularPrecioPasajero();
        }

        Pago pago(precioTotal, pasajeros, reserva, rutaSeleccionada, reserva.getIdReserva());

        cout << "\t\t\t--------------------------------- PAGO -----------------------------------" << endl;
        pago.ingresarDatosPagador();
        colaPagos.enqueue(pago);
        cout << "\t\t\tPago agregado a la cola." << endl;

        cout << "\t\t\t--------------------------------- PROCESANDO PAGOS -----------------------------------" << endl;
        colaPagos.procesarPagos();

        listaReservas.guardarListaEnArchivo(archivoReservas);
        gestorRutas.guardarRutaEnArchivo(opcionRuta - 1, archivoReservas);
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
        cout << "\t\t\t| lo ultimo en vuelos, novedades, promociones y mucho m�s.       |" << endl;
        cout << "\t\t\t| Si no deseas registrarte, puedes continuar sin cuenta.         |" << endl;
        cout << "\t\t\t-----------------------------------------------------------------" << endl;
        cout << "\t\t\t      SI/NO: ";
    }

};
