#include "Chasseur.h"
#include "Gardien.h"
#include <iostream>
/*
 *	Tente un deplacement.
 */

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
}

/*
 *	Fait bouger la boule de feu (ceci est une exemple, � vous de traiter les collisions sp�cifiques...)
 */

bool Chasseur::process_fireball (float dx, float dy)
{
	float forward_ball_x = _fb -> get_x () + dx;
	float forward_ball_y = _fb -> get_y () + dy;

	int pos_forward_ball_x = (int)(forward_ball_x / Environnement::scale);
	int pos_forward_ball_y = (int)(forward_ball_y / Environnement::scale);


	for (int i = 1; i < _l -> _nguards; ++i)
	{
		Mover * g = _l -> _guards[i];
		//std :: cout << "dis : "<< ((Gardien *)g) ->calculate_distance(forward_ball_x, forward_ball_y) << std::endl;
		//int pos_g_x = (int)(g -> _x / Environnement::scale);
		//int pos_g_y = (int)(g -> _y / Environnement::scale);

		if(((Gardien *)g) ->calculate_distance(forward_ball_x, forward_ball_y) < 3) {
			g -> tomber();
			_hunter_hit -> play();
			return false;
		}
	}


	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float	y = (_y - _fb -> get_y ()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
							 (int)((_fb -> get_y () + dy) / Environnement::scale)))
	{
		message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}
	// collision...
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
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
