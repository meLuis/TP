#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Usuario
{
protected:
	string nombre;
	string apellido;
	string eMail;
	string contrase�a;
public:
	Usuario() {}
	Usuario(string nom, string a, string e, string c) : nombre(nom), apellido(a), eMail(e), contrase�a(c) {}

	~Usuario() {}

	string getEmail() { return eMail; }
	string getNombre() { return nombre; }
	string getApellido() { return apellido; }
	string getContrase�a() { return contrase�a; }

	virtual string getTipo() { return "sin definir"; };
	virtual void mostrarBienvenida() {
		cout << "Bienvenido " << nombre << " " << apellido << endl;
	};
};
