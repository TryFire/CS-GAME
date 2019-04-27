#include "Gardien.h"

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele){
	isInitPara = false;
}

void Gardien::update (void) {
	if(!isInitPara) {
		initParameters();
		isInitPara = true;
	}
	if (isDead)
	{
		/* code */
		
	}
	else if(!isMeetEnmy) {
		if(distance > distance_max) {
			inverse_direction();
		}
		//std::cout << " angle : " <<  _angle << "  move_angle : " << move_angle << std::endl;
		float dx = vitess*cos(transfomer_angle_a_pi(move_angle));
		float dy = vitess*sin(transfomer_angle_a_pi(move_angle));
		if(!move(dx, dy)){
			inverse_direction();
		} else {
			update_distance();
		}

		// test if meet enmy
		Mover * chasseur = _l -> _guards [0];
		float distanca_with_chasseur = calculate_distance(chasseur-> _x, chasseur -> _y);
		/*if(distanca_with_chasseur <= distance_see) {
			isMeetEnmy = true;
		}*/
	} else {

	}
}

bool Gardien::move (double dx, double dy) {
	float forward_x = _x + dx;
	float forward_y = _y + dy;
	if(calculate_distance(forward_x, forward_y) > distance_max) {
		return false;
	}
	if (EMPTY == _l -> data ((int)((forward_x) / Environnement::scale),
						 (int)((forward_y) / Environnement::scale)))
	{
		_x += dx;
		_y += dy;

		if(calculate_distance(origin_x, origin_y) < 0.0005){
			//std::cout << move_angle << std::endl;
			tourner_direction();
			
		}
	//std::cout << "x : " << _x << " y : " << _y << std::endl; 
		return true;
	}
	return false; 
}
void Gardien::initParameters() {
	std:: cout << "scal :" << Environnement::scale << std::endl;
	origin_x = _x;
	origin_y = _y;
	distance_max = 3.0*Environnement::scale;
	distance = 0;
	vitess = 0.1;
	isMeetEnmy = false;
	move_angle = 0;
	_angle = move_angle + 270;
	distance_see = 3.5*Environnement::scale;
	index = 0;

	isDead = false;
	blood = 100;
	armor = 20;
}

bool Gardien::hited(int power, float max_dist, float dist){
	if(dist > max_dist) {
		return false;
	}

	// left power of the gun
	int power_left = (int)((1.0 - dist/max_dist)*power);
	std::cout<< dist << "  " << max_dist << "power lefr : " << power_left << std::endl; 
	int blood_decreased = power_left - armor;
	std::cout << "blood_decreased: " << blood_decreased << std::endl;
	if (blood > blood_decreased) {
		blood -= blood_decreased;
		message ("blood left : %d", (int) blood);
		return false;
	} else {
		blood = 0;
		message ("dead");
		isDead = true;
		return true;
	}
}