#include "Chasseur.h"
#include "Gardien.h"
#include <iostream>
#include <cmath>
/*
 *	Tente un deplacement.
 */
using namespace std;
bool Chasseur::move_aux (double dx, double dy)
{
	if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
							 (int)((_y + dy) / Environnement::scale)))
	{
		_x += dx;
		_y += dy;
		//std::cout << "x : " << _x << " y : " << _y << std::endl; 
		return true;
	}
	return false;
}

/*
 *	Constructeur.
 */

Chasseur::Chasseur (Labyrinthe* l) : Mover (20, 50, l, 0)
{
	_hunter_fire = new Sound ("sons/hunter_fire.wav");
	_hunter_hit = new Sound ("sons/hunter_hit.wav");
	if (_wall_hit == 0)
		_wall_hit = new Sound ("sons/hit_wall.wav");

	max_blood = 100;
	current_blood = max_blood;
	armor = 20;
	max_shoot_dist = 200;
	power = 50;

	recover_interval_time = 5;
	recover_percent_per_second = 0.02;

	runaway_mode = false;
	runaway_mode_last_time = 5;
}

/*
 *	Fait bouger la boule de feu (ceci est une exemple, � vous de traiter les collisions sp�cifiques...)
 */

bool Chasseur::process_fireball (float dx, float dy)
{

	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float	y = (_y - _fb -> get_y ()) / Environnement::scale;
	float	dist2 = sqrt(x*x + y*y);

	// calculer la distance maximum en ligne droite.
	float	dmax2 = sqrt((_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ()));

	float forward_ball_x = _fb -> get_x () + dx;
	float forward_ball_y = _fb -> get_y () + dy;

	int pos_forward_ball_x = (int)(forward_ball_x / Environnement::scale);
	int pos_forward_ball_y = (int)(forward_ball_y / Environnement::scale);


	for (int i = 1; i < _l -> _nguards; ++i)
	{
		Mover * g = _l -> _guards[i];
		// si le gardien est dead, ignore
		if (!((Gardien *)g) -> is_alive()) {
			continue;
		}
		
		int pos_g_x = (int)(g -> _x / Environnement::scale);
		int pos_g_y = (int)(g -> _y / Environnement::scale);

		// si on bouge sur un gardien
		if(pos_forward_ball_x == pos_g_x 
		   && pos_forward_ball_y == pos_g_y) {\
		   	int curr_power = power;
		   	if (runaway_mode)
		   	{
		   		curr_power = power * 2;
		   		time(&current_time);
				if (current_time - start_runaway_mode_time >= runaway_mode_last_time)
				{
					runaway_mode = false;
				}
		   	}
		   	// le gardien var etre blesse
		   	if (((Gardien *)g) -> hited(curr_power, max_shoot_dist, dist2)){
		   		// si le gardien est dead
		   		g -> rester_au_sol();
		   	} else {
		   		g -> tomber();
		   	}
			_hunter_hit -> play(1. - dist2/dmax2);
			return false;
		}
	}

	
	// on bouge que dans le vide!
	if (EMPTY == _l -> data (pos_forward_ball_x, pos_forward_ball_y))
	{
		message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}

	// test si on a touche un box
	for (int i = 0; i < _l -> _nboxes; ++i)
	{
		// si on a touche un box
		if (pos_forward_ball_x == (_l -> _boxes[i])._x
			&& pos_forward_ball_y == (_l -> _boxes[i])._y)
		{
			
			// Le capital de survie revient au maximum
			current_blood = max_blood;
			message ("current_blood left : %d", (int) current_blood);
			// le box va appartenir
			cout << "number of box: " <<  _l -> _nboxes << endl;
			for (int j = i; j < _l -> _nboxes-1; ++j)
			{
				_l -> _boxes[j] = _l -> _boxes[j+1];
			}
			_l -> _nboxes --;
			cout << "number of box: " <<  _l -> _nboxes << endl;
			return false;
		}
	}


	// collision...
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	message ("Booom...");
	// teste si on a touch� le tr�sor: juste pour montrer un exemple de la
	// fonction � partie_terminee �.
	if ((int)((_fb -> get_x () + dx) / Environnement::scale) == _l -> _treasor._x &&
		(int)((_fb -> get_y () + dy) / Environnement::scale) == _l -> _treasor._y)
	{
		partie_terminee (true);
	}
	return false;
}

/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
	message ("Woooshh...");
	_hunter_fire -> play ();
	_fb -> init (/* position initiale de la boule */ _x, _y, 10.,
				 /* angles de vis�e */ angle_vertical, _angle);
	cout << _angle << endl;
}

bool Chasseur::hited(int power, float max_dist, float dist){
	if(dist > max_dist) {
		return false;
	}

	// left power of the gun
	int power_left = (int)((1.0 - dist/max_dist)*power);
	int curr_armor = armor;
	if (runaway_mode)
	{
		curr_armor = armor * 1.5;
		time(&current_time);
		if (current_time - start_runaway_mode_time >= runaway_mode_last_time)
		{
			runaway_mode = false;
		}
	}
	// le nombre que capital de survie va diminuer
	int current_blood_decreased = power_left - curr_armor;
	_hunter_hit -> play();
	if (current_blood > current_blood_decreased
		&& current_blood_decreased > 0) {
		current_blood -= current_blood_decreased;
		time(&last_injured_time);
		message ("current_blood left : %d", (int) current_blood);
		return false;
	} else {
		partie_terminee (false);
		return true;
	}
}

void Chasseur::recover(){
	if (current_blood == max_blood)
	{
		return;
	}
	time(&current_time);
	if (current_time - last_recover_time < 1)
	{
		return;
	}
	if (current_time - last_injured_time >= recover_interval_time)
	{
		int blood_recover = max_blood * recover_percent_per_second;
		if (runaway_mode){
			// en mode d'emballement
			// récupération du volume sanguin augmentée de 100%
			blood_recover = max_blood * recover_percent_per_second*2;
			time(&current_time);
			if (current_time - start_runaway_mode_time >= runaway_mode_last_time)
			{
				runaway_mode = false;
			}
		}
		current_blood += blood_recover;
		// mise a jour le temps de recover
		last_recover_time = current_time;
		if(current_blood > max_blood) {
			current_blood = max_blood;
		}
		message ("current_blood left : %d", (int) current_blood);
	}
}

/*
 *	Clic droit: par d�faut fait tomber le premier gardien.
 *
 *	Inutile dans le vrai jeu, mais c'est juste pour montrer
 *	une utilisation des fonctions � tomber � et � rester_au_sol �
 */

void Chasseur::right_click (bool shift, bool control) {

	if (shift) {
		message("Vous entrez en mode d'emballement, puissance d'attaque augmentée de 100%, volume sanguin augmenté de 100%, armure augmentée de 50%");
		time(&start_runaway_mode_time);
		runaway_mode = true;
	}
	else {
	}
}
