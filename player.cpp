#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar




double Jugador1(const Environment &estado){
	int jugador = 1;
	int jug = 2;
	int bomba = 4;
	double suma = 0;


	//intenta hacer uno pa saber si tienes bomba  propia y una propina si hay una de su color en la fila


	for(int i = 0; i < 7; ++i){
		for(int j = 0; j < 7; ++j){
			if(estado.See_Casilla(i, j)==bomba)
				for(int k = 0; k < 7; ++k)
					if(estado.See_Casilla(i, k) == jugador)
						suma += 15;
				suma += 50;
		}
	}

	return suma;
}


double Jugador2(const Environment & estado){
	int jugador = 2;
	int jug = 1;
	int bomba = 5;
	double suma = 0;


	return suma;
}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
	if (jugador == 1)
			return Jugador1(estado) - Jugador2(estado);
	else
			return Jugador2(estado) - Jugador1(estado);
}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Player::MiniMax(const Environment &estado, int jugador, int & accion, int profundidad, double alp, double bet){
		if(estado.JuegoTerminado()){
			//cout << "Antes del dentro de juego terminado"<<endl;
			return ValoracionTest(estado, jugador);
		}
		else if(profundidad== 0){
			//cout << "posible error"<<endl;
			return Valoracion(estado, jugador);
			}	
		else{
			if(profundidad % 2 == 0){
				//cout << "Dentro del max"<<endl;
				int ult = -1;
				int dec;
				double aux;

				Environment hijo = estado.GenerateNextMove(ult);

				while(ult < 8){
					aux = MiniMax(hijo, jugador, dec, profundidad - 1, alp, bet);

					if(aux > alp){
						alp = aux;
						accion = ult;
					}
					
					if(alp >= bet)
							return bet;			// condicion de poda
					
					hijo = estado.GenerateNextMove(ult);
				}

				return alp;
			}
			else{
				cout << "dentro del min"<<endl;
				int ult = -1;
				int dec;
				double aux;

				Environment hijo = estado.GenerateNextMove(ult);

				while(ult < 8){
					
					aux = MiniMax(hijo, jugador, dec, profundidad - 1, alp, bet);

					if(aux < bet){
						bet = aux;
						accion = ult;
					}
					
					if(alp >= bet)
							return alp;			// condicion de poda
					
					hijo = estado.GenerateNextMove(ult);
				}

				return bet;
			}									
			
		}


		
}



// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable


    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    /*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui
	*/

    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    // valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    alpha = menosinf;
    beta = masinf;

    int accion2 = -1;


    cout << "Antes del minimax"<<endl;
    valor = MiniMax(actual_, jugador_, accion2, PROFUNDIDAD_ALFABETA, alpha, beta);

    accion = static_cast<Environment::ActionType>(accion2);


    cout << "Valor minmax: "<< valor << "\t Accion:" <<actual_.ActionStr(accion)<<endl;

    return accion;
}




/*double Alpha_Beta(Environment actual, int jugador, int contador, int LIMITE, Environment::ActionType acc, double alpha, double beta){

		Environment V[];
		int n_opciones = Environment::GenerateAllMoves(V);

		double alp = 0;
		for (int i = 0; i < n_opciones; i++){
			if(V[i].JuegoTerminado())

		}

		return 0.0;
}
*/