// practica_3_blackjack_mejorado.cpp : 
//

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

const int NUMERO_CARTAS = 40;

typedef struct {
	string nick;
	int partidasGanadas;
	int partidasPerdidas;
	double saldo;
}Jugador;

typedef struct {
	Jugador jugadores[20];
	int contador;
}DatosPartidas;

typedef struct {
	int carta[NUMERO_CARTAS];
	int contador;
}cartas;

Jugador iniciarSesion(DatosPartidas d, string nick) {

	Jugador participante;
	bool existe = false;
	for (int i = 0; i < d.contador && existe == false; i++) {
		if (nick == d.jugadores[i].nick) {
			existe = true;
			participante = d.jugadores[i];
		}
	}
	if (existe == false) {
		participante.nick = nick;
		participante.partidasGanadas = 0;
		participante.partidasPerdidas = 0;
		participante.saldo = 100;
		d.jugadores[d.contador] = participante;
	}
	return participante;
}

int buscaJugador(DatosPartidas d, string nick) {
	int indice = -1;
	bool existe = false;
	for (int i = 0; i < d.contador && existe == false; i++) {
		if (nick == d.jugadores[i].nick) {
			existe = true;
			indice = i;
		}
	}

	return indice;
}

void cargaDatos(DatosPartidas& d) {
	ifstream archivo;
	archivo.open("jugadores.txt");
	if (archivo.is_open()) {
		archivo >> d.contador;
		for (int i = 0; i < d.contador; i++) {
			archivo >> d.jugadores[i].nick;
			archivo >> d.jugadores[i].partidasGanadas;
			archivo >> d.jugadores[i].partidasPerdidas;
			archivo >> d.jugadores[i].saldo;
		}
		archivo.close();
	}
	else {
		cout << "No se ha podido abrir el archivo de texto"
	}
}
void gestionSaldo(DatosPartidas d, Jugador j) {
	int decision;
	cin >> decision;
	cout << "1- Consulta saldo disponible ";
	cout << "2- Recarga ";
	cout << "3- Salir ";
	switch (decision) {
	case 1:
		cout << "El saldo del jugador es " << j.saldo << endl;
		break;
	case 2:
		int extra;
		cin >> extra;
		d.jugadores[buscaJugador(d, j.nick)].saldo = d.jugadores[buscaJugador(d, j.nick)].saldo + extra;
		break;
	case 0:
		break;
	}
}
void muestraInfo(DatosPartidas d, string nick) {
	cout << "El nombre del jugador es " << d.jugadores[buscaJugador(d, nick)].nick << endl;
	cout << "El numero de partidas ganadas del jugador es " << d.jugadores[buscaJugador(d, nick)].partidasGanadas << endl;
	cout << "El numero de partidas perdidas del jugador es " << d.jugadores[buscaJugador(d, nick)].partidasPerdidas << endl;
	cout << "El saldo del jugador es " << d.jugadores[buscaJugador(d, nick)].saldo << endl;

}


void infoJugadores(DatosPartidas d) {
	for (int i = 0; i < d.contador; i++) {
		cout << "----------------- ";
		muestraInfo(d, d.jugadores[i].nick);
	}
}

void actualizaJugador(DatosPartidas d, Jugador j) {
	int pos = buscaJugador(d, j.nick);
	if (pos == -1) {
		d.jugadores[d.contador] = j;
	}
	else {
		d.jugadores[pos] = j;
	}
}
void guardaJugador(DatosPartidas d, Jugador j) {
	if (d.contador == 20) {
		cout << "Esta llena la lista de jugadores ";
	}
	else {
		d.jugadores[d.contador] = j;
		cout << " El jugador " << j.nick << " Se ha metido en el array";
	}
}

void guardaDatos(DatosPartidas d) {
	ofstream archivo;
	archivo.open("jugadores.txt");
	archivo << d.contador << endl;
	for (int i = 0; i < d.contador; i++) {
		archivo << "-------";
		archivo << d.jugadores[i].nick << " ";
		archivo << d.jugadores[i].partidasGanadas << " ";
		archivo << d.jugadores[i].partidasPerdidas << " ";
		archivo << d.jugadores[i].saldo << endl;
	}
	archivo.close();
}

//muestra el texto de las reglas de juego por pantalla
void muestraReglas() {
	cout << "------------------------------------------------------------------------------------" << endl;
	cout << "Al principio, los dos jugadores obtienen dos cartas. " << endl;
	cout << "En tu turno, eligiras si quieres una carta mas o si quieres plantarrte. " << endl;
	cout << "El ganador de la partida sera el que mas se aproxime a 21 sin pasarse. " << endl;
	cout << "Las cartas numericas suman su valor, las figuras suman 10 y el As vale 11 o 1. " << endl;
	cout << "La mejor jugada es el Blackjack: conseguir 21 con solo dos cartas, un As mas un 10. " << endl;
	cout << "Un Blackjack gana sobre un 21 conseguido con mas de dos cartas. " << endl;
	cout << "Volviendo al menu..." << endl;
	cout << "------------------------------------------------------------------------------------" << endl;
}

//función que, dada una mano de cartas calcula los puntos de las cartas de la mano de acuerdo a las reglas del juego
int calculaPuntos(cartas mano) {
	int resultado = 0;
	for (int i = 0; i < mano.contador; i++) {
		if (mano.carta[i] >= 2 && mano.carta[i] <= 7) {
			resultado = resultado + mano.carta[i];
		}
		else if (mano.carta[i] >= 10) {
			resultado = resultado + 10;
		}
		else {
			if (resultado + 11 > 21) {
				resultado = resultado + 1;
			}
			else {
				resultado = resultado + 11;
			}
		}
	}
	return resultado;
}

//esta función calcula el ganador de la partida teniendo en cuenta la puntuación y, en caso de empate, el número de cartas de cada uno
int ganador(int puntosJugador, int puntosCrupier, int numCartasJugador, int numCartasCrupier) {
	int marcador;
	if (puntosJugador > 21) {
		marcador = 2;
	}
	else if (puntosCrupier > 21) {
		marcador = 1;
	}
	else if (puntosJugador > puntosCrupier) {
		marcador = 1;
	}
	else if (puntosJugador < puntosCrupier) {
		marcador = 2;
	}
	else {
		marcador = 0;
	}
	return marcador;
}

//muestra el mazo de cartas por pantalla
void muestraMazo(cartas cartas) {
	for (int i = 0; i < cartas.contador; i++) {
		cout << cartas.carta[i] << " ";
	}
	cout << endl;
}

//saca una carta del mazo y devuelve su valor
int sacarCartaMazo(cartas& mazo) {
	int contact = mazo.contador;
	mazo.contador = mazo.contador - 1;
	return mazo.carta[contact - 1];
}

//añade una carta a la mano de cartas
void annadeCarta(cartas& mano, int carta) {
	mano.carta[mano.contador] = carta;
	mano.contador = mano.contador + 1;
}

//en esta función se desarrolla el turno del jugador, devuelve la puntuación total del jugador
int juegaHumano(cartas& mazo, cartas& cartasJugador) {

	cout << "Cartas jugador: ";
	muestraMazo(cartasJugador);
	int puntos = 0;
	puntos = calculaPuntos(cartasJugador);
	cout << "Puntos jugador: " << puntos << endl;
	if (puntos > 21) {
		return puntos;
	}
	else {
		char decision;
		cout << "Quieres plantarte? (S/N) ";
		cin >> decision;
		if (decision == 's') {
			return puntos;
		}
		else {
			annadeCarta(cartasJugador, sacarCartaMazo(mazo));
			juegaHumano(mazo, cartasJugador);
		}
	}
}

//en esta función se desarrolla el turno del Crupier, devuelve la puntuación total del Crupier
int juegaCrupier(cartas& mazo, cartas& cartasCrupier) {
	cout << "Cartas crupier: ";
	muestraMazo(cartasCrupier);
	int puntos = 0;
	puntos = calculaPuntos(cartasCrupier);
	cout << "Puntos crupier: " << puntos << endl;
	if (puntos >= 17) {
		return puntos;
	}
	else {
		while (puntos < 17) {
			annadeCarta(cartasCrupier, sacarCartaMazo(mazo));
			cout << "Cartas crupier: ";
			muestraMazo(cartasCrupier);
			puntos = calculaPuntos(cartasCrupier);
			cout << "Puntos crupier: " << puntos << endl;
		}
		return puntos;
	}
}

//dado un mazo de cartas creado, inicializa los valores en orden con los 40 valores de la baraja española
void inicializaMazo(cartas& mazo) {
	for (int i = 0; i < NUMERO_CARTAS; i++) {
		mazo.carta[i] = (i % 10) + 1;
		if (mazo.carta[i] > 7) {
			mazo.carta[i] += 2;
		}
	}
	mazo.contador = 40;
}

//dado un mazo de cartas, las mezcla aleatoriamente
void barajar(cartas& mazo) {
	for (int i = 0; i < NUMERO_CARTAS; i++) {
		int c1 = rand() % NUMERO_CARTAS;
		int c2 = rand() % NUMERO_CARTAS;
		int aux = mazo.carta[c2];
		mazo.carta[c2] = mazo.carta[c1];
		mazo.carta[c1] = aux;
	}
}

//aquí se desarrolla la acción principal del juego
void juegoBlackjack(DatosPartidas d, Jugador j) {
	cout << "Cuanto quieres apostar?   ";
	cout << "Tienes disponible " << j.saldo;
	int apuesta;
	cin >> apuesta;
	if (apuesta > j.saldo) {
		cout << "No dispones de tanto dinero ";
		return;
	}
	cartas mazo;
	inicializaMazo(mazo);
	barajar(mazo);
	cout << "Se ha repartido." << endl;
	cartas manoJugador;
	manoJugador.contador = 0;
	annadeCarta(manoJugador, sacarCartaMazo(mazo));
	cartas manoCrupier;
	manoCrupier.contador = 0;
	annadeCarta(manoCrupier, sacarCartaMazo(mazo));
	cout << "La primera carta del crupier es un " << manoCrupier.carta[0] << endl;
	cout << "------------------" << endl;
	cout << "Turno jugador: " << endl;
	cout << "------------------" << endl;
	juegaHumano(mazo, manoJugador);
	cout << "------------------" << endl;
	cout << "Turno crupier: " << endl;
	cout << "------------------" << endl;
	juegaCrupier(mazo, manoCrupier);
	cout << "----------------" << endl;
	cout << "Puntuacion Final" << endl;
	cout << "----------------" << endl;
	cout << "Crupier: " << calculaPuntos(manoCrupier) << "      ";
	cout << "Jugador: " << calculaPuntos(manoJugador) << endl;
	int resganador;
	resganador = ganador(calculaPuntos(manoJugador), calculaPuntos(manoCrupier), manoJugador.contador, manoCrupier.contador);
	if (resganador == 0) {
		cout << "Empate!" << endl;
		if (manoJugador.contador > manoCrupier.contador) {
			cout << "Gana Crupier" << endl;
		}
		else {
			cout << "Ganas" << endl;
		}
	}
	else if (resganador == 1) {
		cout << "Ganas" << endl;
		j.saldo = j.saldo + apuesta;
		j.partidasGanadas++;
	}
	else {
		cout << "Gana Crupier" << endl;
		j.saldo = j.saldo - apuesta;
		j.partidasPerdidas++;
	}
	actualizaJugador(d, j);
}

//función encargada de mostrar el menú por pantalla, comprobar que la opción escogida sea válida, y devolver la opción escogida
int menu() {

	string nombre;
	Jugador jugador;
	int pos;

	DatosPartidas partidas;

	partidas.contador = 0;

	cargaDatos(partidas);

	cout << partidas.contador;

	int menu;
	cout << "Introduce tu nick: ";
	cin >> nombre;
	cout << "Bienvenido " << nombre << endl;
	pos = buscaJugador(partidas, nombre);
	if (pos == -1) {
		jugador.nick = nombre;
		jugador.partidasGanadas = 0;
		jugador.partidasPerdidas = 0;
		jugador.saldo = 100;
		guardaJugador(partidas, jugador);
		cout << "El contador tiene el valor : " << partidas.contador;
		cout << "El jugador que esta en el array es" << partidas.jugadores[partidas.contador].nick << endl;
	}
	else {
		jugador = partidas.jugadores[buscaJugador(partidas, nombre)];
	}

	cout << "---------------------------------" << endl;
	cout << "BIENVENIDO A LA MESA DE BLACKJACK" << endl;
	cout << "---------------------------------" << endl;
	cout << "Elige una opcion: ";
	cout << "1- Jugar" << endl;
	cout << "2- Ver informacion de todos los jugadores" << endl;
	cout << "3- Ver informacion jugador" << endl;
	cout << "4- Saldo" << endl;
	cout << "5- Reglas de juego" << endl;
	cout << "0- Salir" << endl;

	cin >> menu;

	switch (menu) {
	case 1:
		juegoBlackjack(partidas, jugador);

		return 1;
		break;
	case 2:
		infoJugadores(partidas);
		return 2;
		break;
	case 3:
		muestraInfo(partidas, nombre);
		return 3;
		break;
	case 4:
		gestionSaldo(partidas, partidas.jugadores[buscaJugador(partidas, nombre)]);

		return 4;
		break;
	case 5:
		muestraReglas();
		return 5;
		break;
	case 0:
		cout << "Hasta la proxima" << endl;

		return 0;
		break;
	default:
		cout << "*****ERROR****** Numero introducido no valido, indica un numero adecuado" << endl;
	}
	guardaDatos(partidas);
}

//función encargada de mostrar el menú por pantalla, comprobar que la noción escogida sea válida, y devolver la opción escogida 
int main() {
	srand((unsigned)time(NULL));



	while (menu() != 0) {
		menu();
	}
}