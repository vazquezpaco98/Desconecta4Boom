#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
	double MiniMax(const Environment &estadi,int jugador, int & accion, int profundidad,double alp,double bet);
    private:
      int jugador_;
      Environment actual_;
};
#endif
