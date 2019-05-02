#include "Gardien.h"
#include <vector>
#include <iostream>

using namespace std;

#define PI 3.14159265

void show_vector(std::vector<int *> v);



Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele){
	isInitPara = false;
	lab_width = _l -> width();
	lab_height = _l -> height();
	iter = 0;
}

void Gardien::update (void) {

	if(!isInitPara) {
		initParameters();
		isInitPara = true;
		find_direction(get_id_by_x_y(2,10), get_id_by_x_y(5,20));
		
		
	}


	if (isDead)
	{
		/* code */
		
	}
	else {if(!isMeetEnmy) {
		recover();
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
		//Mover * chasseur = _l -> _guards [0];
		//float distanca_with_chasseur = calculate_distance(chasseur-> _x, chasseur -> _y);
		/*if(distanca_with_chasseur <= distance_see) {
			isMeetEnmy = true;
		}*/
	} else {

	}
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
	max_blood = 100;
	current_blood = max_blood;
	armor = 20;

	recover_interval_time = 5;
	recover_percent_per_second = 0.05;
}

bool Gardien::hited(int power, float max_dist, float dist){
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
		current_blood = 0;
		message ("dead");
		isDead = true;
		return true;
	}
}

void Gardien::recover(){
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
		cout << "blod  " << current_blood << endl;
		if(current_blood > max_blood) {
			current_blood = max_blood;
		}
	}
}

bool Gardien::is_blocked(int start, int end) {
	int x1 = (int)get_x_by_id(start);
	int y1 = (int)get_y_by_id(start);
	int x2 = (int)get_x_by_id(end);
	int y2 = (int)get_y_by_id(end);
	if (x2 < x1) {
		int t = x2;
		x2 = x1;
		x1 = t;
	}
	if (y2 < y1) {
		int t = y2;
		y2 = y1;
		y1 =t;
	}
	for (int i = x1; i <= x2; ++i)
	{
		for (int j = y1; j <= y2; ++j)
		{
			if (_l -> data(i,j) == 1)
			{
				return true;
			}
		}
	}
	return false;
}

int Gardien::find_direction(int start, int end) {
	int direction = move_angle;

	int DPN = 1;

	cout << "start : " << get_x_by_id(start) << "," << get_y_by_id(start) << endl;

	int HN = get_HN(start, end);
	int * S = new int[4];
	S[0] = start;
	S[1] = start;
	S[2] = HN;
	S[3] = 0;
	std::vector<int *> open_list;
	std::vector<int *> close_list;

	put(open_list, S);

	while(!open_list.empty()) {
		int* P = open_list.back();
		open_list.pop_back();
		int* Ns = get_points_of_4_direction(P[0]);
		for (int i = 0; i < 4; ++i)
		{

			if(!is_accessiabl( Ns[i])) {
				continue;
			}
			if(in_vector(close_list, Ns[i])) {
				continue;
			}
			if(in_vector(open_list, Ns[i])) {
				int index = get_index_of_element(open_list, Ns[i]);
				int newGN = P[3] + DPN;
				int newFN = newGN + get_HN(Ns[i], end);

				if (newFN < open_list[index][2]) {
					update_vector(open_list, index, newFN, newGN);
				}
			} else {
				int GN = P[3] + DPN;
				int FN = GN + get_HN(Ns[i], end);
				int *N = new int[4];
				N[0] = Ns[i];
				N[1] = P[0];
				N[2] = FN;
				N[3] = GN;
				put(open_list, N);
			}
			
		}
		close_list.push_back(P);

		if(P[0] == end) {
			
			cout << "=====found====" << endl;
			//show_vector(close_list);
			direction = get_route(close_list, start, P[0]);
			break;
		}
	}

	return direction;
}

// 
// ============ A* 
// 



/**
 * obtenir les 4 cas au tour de le cas donne
 * @param  id : la cas donne
 * @return    unt tableau de id : 4 cas au tour de le cas donne
 */
int* Gardien::get_points_of_4_direction(int id){
	int x = get_x_by_id(id);
	int y = get_y_by_id(id);
	int id1 = get_id_by_x_y(x, y-1);
	int id2 = get_id_by_x_y(x, y+1);
	int id3 = get_id_by_x_y(x-1, y);
	int id4 = get_id_by_x_y(x+1, y);
	int * res = new int[4];
	res[0] = id1;
	res[1] = id2;
	res[2] = id3;
	res[3] = id4;
	return res;
}

/**
 * push une tableau a la vector pour que le vector decroissant dans l'ordre de la troisième valeur du tableau		
 * @param open_list : le vector
 * @param p         : le tableau qui va etre ajoute dans le vector
 */
void Gardien::put(std::vector<int *>& open_list, int p[]){
	bool is_insert = false;
	for (std::vector<int *>::iterator it = open_list.end(); it != open_list.begin(); --it)
	{		
		//cout << " == " << endl;
		if ( (*(it-1))[2]> p[2] ){
			open_list.insert(it, p);
			is_insert = true;
			break;
		}
	}
	if (!is_insert) {
		open_list.insert(open_list.begin(), p);
	}
}

/**
 * verifier que si un cas est deja dans le vector
 * @param  close_list : le vector
 * @param  id         : id de ce cas
 * @return            : true si le cas est dans le vector
 */
bool Gardien::in_vector(std::vector<int *> close_list, int id)  {
	int size = close_list.size();
	for (int i = 0; i < size; ++i)
	{
		if(close_list[i][0] == id) {
			return true;
		}
	}
	return false;
}

/**
 * obtenir l'indice de le cas dans le vector
 * @param  close_list : le vector
 * @param  id         : id de ce cas
 * @return            : l'indice
 */
int Gardien::get_index_of_element(std::vector<int *> close_list, int id){
	int size = close_list.size();
	for (int i = 0; i < size; ++i)
	{
		if(close_list[i][0] == id) {
			return i;
		}
	}
	return -1;
}
/**
 * d'abord, motifier le cas de index par newFN et newGN,
 * ensuite, trier le vector pour que le vector decroissant dans l'ordre de la troisième valeur du tableau	
 * @param open_list : le vector
 * @param index     : l'indice du cas dans le vector on veux motifier
 * @param newFN     : le valeur de le troisieme cas du tableau 
 * @param newGN     : le valeur de le quatieme cas du tableau 
 */
void Gardien::update_vector(std::vector<int *>& open_list, int index, int newFN, int newGN) {
	open_list[index][2] = newFN;
	open_list[index][3] = newGN;
	int i = index+1;
	int size = open_list.size();
	while(i < size) {
		if (newFN < open_list[i][2]){
			int * temp = open_list[i];
			open_list[i] = open_list[index];
			open_list[index] = temp;
			index = i;
			++i;
		} else {
			break;
		}
	}
}

int Gardien::get_route(std::vector<int *> close_list, int start, int id) {
	std::vector<int> route;

	//show_vector(close_list);

	while(id != start) {
		for (int i = close_list.size()-1; i >= 0; --i)
		{
			if (close_list[i][0] == id)
			{
				//cout << "id : " << id << endl;
				route.push_back(id);
				id = close_list[i][1];
				break;
			}
		}
	}

	int size = route.size();

	/*for (int i = size-1; i >= 0; --i)
	{
		int x = get_x_by_id(route[i]);
		int y = get_y_by_id(route[i]);
		cout << "(" << x << "," << y << ") -> ";
	}
	cout << endl;
	cout << "here=====" << endl;*/

	int critical_point = start;

	for (int i = size-1; i >= 0; --i)
	{
		if (!is_blocked(start, route[i]))
		{
			critical_point = route[i];
		} else {
			break;
		}
	}
	int x = get_x_by_id(critical_point);
	int y = get_y_by_id(critical_point);

	cout << "the critical point : " << "(" << x << "," << y << ")" << endl;

	return calculate_angle(start, critical_point);
}

int Gardien::calculate_angle(int start, int end){
	int x1 = (int)get_x_by_id(start);
	int y1 = (int)get_y_by_id(start);
	int x2 = (int)get_x_by_id(end);
	int y2 = (int)get_y_by_id(end);
	int x = x2-x1;
	int y = y2-y1;
	double result;
	result = atan2 (y,x) * 180 / PI;
	return (int) result + 360;
}

void show_vector(std::vector<int *> v){
	int size = v.size();
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout <<v[i][j] << "  "; 
		}
		int x = (v[i][0])/92;
		int y = (v[i][0])%92;
		cout << "  (" << x << "," << y << ")";
		cout << endl;
	}
}

