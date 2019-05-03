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
		//std :: cout << "dis : "<< ((Gardien *)g) ->calculate_distance(forward_ball_x, forward_ball_y) << std::endl;
		int pos_g_x = (int)(g -> _x / Environnement::scale);
		int pos_g_y = (int)(g -> _y / Environnement::scale);

		if(pos_forward_ball_x == pos_g_x 
		   && pos_forward_ball_y == pos_g_y) {
		   	//std::cout << "dist2 : "<<dist2 << std::endl;
		   	if (((Gardien *)g) -> hited(power, max_shoot_dist, dist2)){
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
}

bool Chasseur::hited(int power, float max_dist, float dist){
	if(dist > max_dist) {
		return false;
	}

	// left power of the gun
	int power_left = (int)((1.0 - dist/max_dist)*power);
	//std::cout<< dist << "  " << max_dist << "power lefr : " << power_left << std::endl; 
	int current_blood_decreased = power_left - armor;
	//std::cout << "current_blood_decreased: " << current_blood_decreased << std::endl;
	if (current_blood > current_blood_decreased) {
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
		current_blood += max_blood * recover_percent_per_second;
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
	std::cout << "shift : " << shift << " control : "<< control << std::endl;
	if (shift)
		_l -> _guards [1] ->tomber (); 
	else {
		_l -> _guards [1] -> rester_au_sol ();
		_l -> _guards [1] ->tomber (); 
	}
}
