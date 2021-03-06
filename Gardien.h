#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include <iostream>
#include <cmath>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */
#include <vector>

class Labyrinthe;

class Gardien : public Mover {
private:
	//       				 				-x(move_angle = 180)
	//		  				 				^
	//	  	  				 				|
	//        				 				|
	//  -y(move_angle = 270)   <----------------------> y(move_angle = 90)
	//	      				 				|
	//	      				 				|
	//	      				 				x(move_angle = 0)

	int move_angle;

	int last_not_meetenmy_move_angle;

	// la position original une fois le gardien crée
	float origin_x, origin_y;
	// la distance max que le gardien puisse move
	float distance_max;
	// la distance de la position de gardien à la position original
	float distance;
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
//==============capital de survie============
	//capital initial de survie currant
	int current_blood;
	//capital initial de survie maximal
	int max_blood;

	int armor;
//==============capital de survie============
//
	// la largeur and longeur du labyrinthe. 
	int lab_width, lab_height;


//==============recover variables============
	// le temps de derniere fois subir de blessure et le temps currant
	time_t last_injured_time,current_time;
	//le personnage reste un certain temps sans subir de blessure
	time_t recover_interval_time;
	// le temps de dernier fois de incrementer le capital initial de survie
	time_t last_recover_time;
	//le persontage de capital initial de survie est incrémenté par seconde
	float recover_percent_per_second;
//==============recover variables============
//
//==============attack variables============
	// Intervalle entre les attaques(seconde)
	time_t attack_interval_time;
	// le temps de dernier attaque
	time_t last_attack_time;
	// le puissance de pistolet
	int power;
	// la distance max dont le pistolet peut arriver
	int  max_shoot_dist;
//==============attack variables============
//
//==============potentiel protection variables=========
	// potentiel protection currant
	float potentiel_protection;
	// seuil de potentiel protection qui separe defend et chercher le chasseur
	float potentiel_seuil;
	// binaire variable signifique si mon potentiel protection a ete inferieur a seuil
	bool danger;


	// le temps de derniere fois d'executer l'algo A*
	time_t last_calculate_route_time;

public:
	

	Gardien (Labyrinthe* l, const char* modele);

	// mon gardien pense tr�s mal!
	void update (void);
	// et ne bouge pas!
	bool move (double dx, double dy);
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical);
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy);

	/**
	 * calcule le potentiel de protection
	 * @return : le potentiel de protection
	 */
	float calculate_potentiel_protection();

	/**
	 * si le gardien est vivant, renvoie true
	 */
	bool is_alive(){return isDead == false;}

	/**
	 * calculate le nombre de gardien qui est vivant
	 * @return : le nombre
	 */
	int calculate_alive_gardien();

	/**
	 * mettre id pour chaque cas de _l -> _data en utilisant (x,y)
	 * @param  x : l'indice de ce cas sur la dimension du largeur du labyrinthe
	 * @param  y : l'indice de ce cas sur la dimension du longeur du labyrinthe
	 * @return   id de ce cas
	 */
	int get_id_by_x_y(int x, int y) {
		return x*lab_height+y;
	}

	/**
	 * obtenir l'indice de le cas sur la dimension du largeur du labyrinthe
	 * @param  id : id de ce cas
	 * @return  l'indice de le cas sur la dimension du largeur du labyrinthe
	 */
	int get_x_by_id(int id) {
		return id/lab_height;
	}

	/**
	 * obtenir l'indice de le cas sur la dimension du longeur du labyrinthe
	 * @param  id : id de ce cas
	 * @return   l'indice de le cas sur la dimension du longeur du labyrinthe
	 */
	int get_y_by_id(int id) {
		return id%lab_height;
	}

	/**
	 * obtenir le cout marchant de le cas now a end
	 * @param  now : le cas courant
	 * @param  end : le cas de destination
	 * @return     : le cout
	 */
	float get_HN(float now, float end){
		return abs(get_x_by_id(now) - get_x_by_id(end))+abs(get_y_by_id(now) - get_y_by_id(end));
	}

	/**
	 * verifier que s'il exist 1 dans tous les cas dans le minimun rectangle qui contient les cas start et end
	 * @param  start : id du cas dans _l -> _data
	 * @param  end   : id du cas dans _l -> _data
	 * @return   true si il y a au mois un 1, false sinon
	 */
	bool is_blocked(int start, int end);

	/**
	 * verifier que si le cas est 1 ou 0
	 * @param  id : id de ce cas
	 * @return   true si le cas est 0, false sinon
	 */
	bool is_accessiabl(int id){
		int x = get_x_by_id(id);
		int y = get_y_by_id(id);
		return _l -> data(x,y)  == 0;
	}
	/**
	 * calcule angle de point start a point end
	 * @param  start [description]
	 * @param  end   [description]
	 * @return       [description]
	 */
	int calculate_angle(int start, int end);
	int calculate_angle(float x1, float y1, float x2, float y2);
	int* get_points_of_4_direction(int id);
	bool in_vector(std::vector<int *> close_list, int id);
	int get_index_of_element(std::vector<int *> close_list, int id);
	void update_vector(std::vector<int *>& open_list, int index, int newFN, int newGN);
	int get_route(std::vector<int *> close_list, int start, int id);
	void put(std::vector<int *>& open_list, int p[]);

	int find_direction(int start, int end);

	void initParameters();

	/**
	 * the gardien is hited by chasseur
	 * @param  power    : 
	 * @param  max_dist [description]
	 * @param  dist     [description]
	 * @return          [description]
	 */
	bool hited(int power, float max_dist, float dist);

	/**
	 * si le gardien est blesse, le capital de survie va augementer 5% par seconde 
	 * si il reste un certain temps sans subir de blessure
	 */
	void recover();

	/**
	 * attack le chasseur 
	 */
	void attack();

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

	// transformation entre move_angle and _angle
	void change_direction(int angle){
		move_angle = angle;
		_angle = (move_angle+270) % 360;
	}

	float calculate_distance(float x, float y) {
		return sqrt(pow(_x - x, 2) + pow(_y - y, 2));
	}

	void update_distance(){
		distance = sqrt(pow(_x - origin_x, 2) + pow(_y - origin_y, 2));
	}

	/**
	 * operation mod entre deux flottant
	 * @param  x1 : float
	 * @param  x2 : float
	 * @return    : x2 mod x1
	 */
	float my_mod(float x1, float x2) {
		if(x1 < x2) {
			return x1;
		} else {
			return (x2-x1);
		}
	}

	/**
	 * la transformer angle a PI
	 * @param  angle : int angle
	 * @return       : numerique associe a PI
	 */
	float transfomer_angle_a_pi(int angle){
		return (angle/360.0)*2*M_PI;
	}
};

#endif
