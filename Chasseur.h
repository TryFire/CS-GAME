#ifndef CHASSEUR_H
#define CHASSEUR_H

#include <stdio.h>
#include <iostream>
#include "Mover.h"
#include "Sound.h"
#include <ctime>

class Labyrinthe;

class Chasseur : public Mover {
private:
	// accepte ou non un deplacement.
	bool move_aux (double dx, double dy);

	//capital initial de survie currant
	int current_blood;
	//capital initial de survie maximal
	int max_blood;
	// Armure
	int armor;
	// la distance max dont le pistolet
	int max_shoot_dist;
	// le puissance de pistolet
	int power;

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

	time_t start_runaway_mode_time;
	time_t runaway_mode_last_time;
	bool runaway_mode;
public:
	/*
	 *	Le son...
	 */
	static Sound*	_hunter_fire;	// bruit de l'arme du chasseur.
	static Sound*	_hunter_hit;	// cri du chasseur touché.
	static Sound*	_wall_hit;		// on a tapé un mur.

	Chasseur (Labyrinthe* l);
	// ne bouger que dans une case vide (on 'glisse' le long des obstacles)
	bool move (double dx, double dy) {
		recover();
		return move_aux (dx, dy) || move_aux (dx, 0.0) || move_aux (0.0, dy);
	}
	// le chasseur ne pense pas!
	void update (void) {};
	// fait bouger la boule de feu (ceci est une exemple, à vous de traiter les collisions spécifiques...)
	bool process_fireball (float dx, float dy);
	// tire sur un ennemi (clic gauche).
	void fire (int angle_vertical);
	// clic droit.
	void right_click (bool shift, bool control);

	/**
	 * the gardien is hited by chasseur
	 * @param  power    : 
	 * @param  max_dist [description]
	 * @param  dist     [description]
	 * @return          [description]
	 */
	bool hited(int power, float max_dist, float dist);

	void recover();
};

#endif
