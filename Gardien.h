#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include <iostream>
#include <cmath>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */


class Labyrinthe;

class Gardien : public Mover {
private:
	//       				 				-x(direction = pi)
	//		  				 				^
	//	  	  				 				|
	//        				 				|
	//  -y(direction = 3pi/2)   <----------------------> y(direction = pi/2)
	//	      				 				|
	//	      				 				|
	//	      				 				x(direction = 0)

	int move_angle;

	// la position original une fois le gardien crée
	float origin_x, origin_y;
	// la distance max que le gardien puisse move
	float distance_max;
	// la distance de la position de gardien à la position original
	float distance;
	// la direction de marche de gardien
	//float direction;
	// la vitess de gardien
	float vitess;

	// si initialisé ou non
	bool isInitPara;

	// region visuable
	float distance_see;
	// si voir enmy dans la distance de visuable
	bool isMeetEnmy;
	// si le gardien is dead
	bool isDead;

	int index;

	int blood;

	int armor;

public:
	

	Gardien (Labyrinthe* l, const char* modele);

	// mon gardien pense tr�s mal!
	void update (void);
	// et ne bouge pas!
	bool move (double dx, double dy);
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical){}
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy) { return false; }

	void initParameters();

	bool hited(int power, float max_dist, float dist);

	void inverse_direction(){
		//direction = my_mod((direction + M_PI), (2*M_PI));
		move_angle = (move_angle + 180) % 360;
		_angle = (move_angle+270) % 360;
	}

	void tourner_direction(){
		//direction = my_mod((direction + M_PI/2), (2*M_PI));
		move_angle = (move_angle + 90) % 360;
		_angle = (move_angle+270) % 360;
	}

	void change_direction(){
		move_angle = (move_angle + 45) % 360;
		_angle = (move_angle+270) % 360;
	}

	float calculate_distance(float x, float y) {
		return sqrt(pow(_x - x, 2) + pow(_y - y, 2));
	}

	void update_distance(){
		distance = sqrt(pow(_x - origin_x, 2) + pow(_y - origin_y, 2));
	}

	float my_mod(float x1, float x2) {
		if(x1 < x2) {
			return x1;
		} else {
			return (x2-x1);
		}
	}

	float transfomer_angle_a_pi(int angle){
		return (angle/360.0)*2*M_PI;
	}
};

#endif
