#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include "Map.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */

using namespace std;

Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touch�.
Sound*	Chasseur::_wall_hit;	// on a tap� un mur.

Environnement* Environnement::init (char* filename)
{
	return new Labyrinthe (filename);
}

/*
 *	EXEMPLE de labyrinthe.
 */

Labyrinthe::Labyrinthe (char* filename)
{
	static const char* model_name[4] = {"drfreak", "Marvin", "Potator", "garde"}; ;
	
	cout << "start" << endl;
	Map map(filename);
	lab_height = map.getMapHeight();
	lab_width = map.getMapWidth();
	std::cout << lab_height << std::endl;
	std::cout << lab_width << std::endl;

	// 
	char** lab_map = map.getMap();
	/*for(int i = 0; i<lab_width; i++) {
        for(int j = 0; j<lab_height; j++){
            if(lab_map[i][j])
            cout << lab_map[i][j]; 
        }
        cout << endl;
    }*/

	cout << "-- construire matrix data" << endl;
	_data = new char* [lab_width];
	for (int i = 0; i < lab_width; ++i)
		_data [i] = new char [lab_height];
	cout << "-- remplir matrix data" << endl;
	for (int i = 0; i < lab_width; ++i)
		for (int j = 0; j < lab_height; ++j) {
			// murs sur les bords.
			// if (i == 0 || i == lab_width-1 || j == 0 || j == lab_height-1)
			// 	_data [i][j] = 1;
			// else
				// rien dedans.
				_data [i][j] = EMPTY;
		}
	cout << "-- construire mur" << endl;

	// vector de Wall
	vector<Wall> wall_vector; 
	// vector de box
	vector<Box> box_vector;
	// vector de Gardien
	vector<Mover*> gardien_vector;
	Mover *chasseur = new Chasseur (this);

	for(int i = 0; i<lab_width; i++) {
        for(int j = 0; j<lab_height; j++){
			// if find the char +
            if(lab_map[i][j] == '+'){
				_data[i][j] = 1;
				// if in horizental the char after '+' is '-' 
				if (j<lab_height-1){
					if(lab_map[i][j+1] == '-' || lab_map[i][j+1] == '+') {
						//cout << "  j: " << j << endl;
						// get the index of first char which is '+' 
						int k = j+1;
						while(lab_map[i][k] != '+'){
							//_data[i][k] = 1;
							k = k+1;
						}
						Wall w;
						w._x1 = i; w._y1 = j;
						w._x2 = i; w._y2 = k;
						wall_vector.push_back(w);
						//cout << "hortizental : " << endl;
						//cout << "  (" << i <<"," << j <<") -> " << " (" << i <<"," << k <<")"<< endl;
						for (int h = j; h<k; h++){
							
							_data[i][h] = 1;
						}
						//cout << "  j: end" << endl;
					}
					
				}
				// if in vertical the char after '+' is '|'
				//cout << "enter : i : " << i << " lab_width : " <<  lab_width << endl; 
				if (i<lab_width-1){
					if(lab_map[i+1][j] == '|' || lab_map[i+1][j] == '+') {
						//cout << "  i: " << i << endl;
						// get the index of first char which is '+' 
						int k = i+1;
						while(lab_map[k][j] != '+'){
							//_data[k][j] = 1;
							k = k+1;
						}
						Wall w;
						w._x1 = i; w._y1 = j;
						w._x2 = k; w._y2 = j;
						wall_vector.push_back(w);
						//cout << "vertical : " << endl;
						//cout << "  (" << i <<"," << j <<") -> " << " (" << k <<"," << j <<")"<< endl;
						for (int h = i; h<k; h++){
							_data[h][j] = 1;
						}
						//cout << "  i: end" << endl;
					}
					
				}
			} 
			else if(lab_map[i][j] == 'X'){
				cout << " == construire Box" << endl;
				Box box;
				box._x = i; box._y = j;
				box_vector.push_back(box);
				_data[i][j] = 1;
			}
			else if(lab_map[i][j] == 'T'){
				cout << " == construire Tresor" << endl;
				_treasor._x = i;
				_treasor._y = j;
				// indiquer qu'on ne marche pas dessus.
				_data [_treasor._x][_treasor._y] = 1;
			} 
			else if(lab_map[i][j] == 'G'){
				cout << " == construire Gardien" << endl;
				int index = 5;
				srand(time(NULL));
				index = (rand() - lab_height*lab_width + i*j) % 4;
				Mover *gardien = new Gardien (this, model_name[index]);
				gardien -> _x = i*scale;
				gardien -> _y = j*scale;
				gardien_vector.push_back(gardien);

				// indique qu'on ne marche pas gardien
				//_data[i][j] = 1;

			} else if(lab_map[i][j] == 'C'){
				cout << " == construire Chasseur" << endl;
				chasseur -> _x = i*scale;
				chasseur -> _y = j*scale;
				//gardien_vector.push_back(chasseur);
			}
        }
    }

	//cout << "== finir construi le mur" << endl;
	cout << wall_vector.size() << endl;

	_nwall = wall_vector.size();
	_walls = new Wall[_nwall];
	for(int i=0; i<_nwall; i++){
		_walls[i] = wall_vector.at(i);
	}
	//cout << "end _walls" << endl;

	_nboxes = box_vector.size();
	_boxes = new Box[_nboxes];
	for(int i = 0; i<_nboxes; i++){
		_boxes[i] = box_vector.at(i);
	}
	//cout << "end _boxes" << endl;

	int n_gardiens = gardien_vector.size();
	//cout << "gardien size : " << gardien_vector.size() << endl;
	_nguards = n_gardiens+1;
	_guards = new Mover* [_nguards+1];
	_guards[0] = chasseur;
	//_guards[1] = gardien_vector.at(0);
	
	for(int i = 0; i<n_gardiens; i++){
		_guards[i+1] = gardien_vector.at(i);
	}

	// lab_height = 80;
	// lab_width = 25;

	// // les murs: 4 dans cet EXEMPLE!
	// int	n = 0;

	// _walls = new Wall [4];
	// // le premier.
	// _walls [n]._x1 = 0; _walls [n]._y1 = 0; 
	// _walls [n]._x2 = 0; _walls [n]._y2 = lab_height-1;
	// _walls [n]._ntex = 0;
	// ++n;
	// // le deuxieme.
	// _walls [n]._x1 = 0; _walls [n]._y1 = lab_height-1;
	// _walls [n]._x2 = lab_width-1; _walls [n]._y2 = lab_height-1;
	// _walls [n]._ntex = 0;
	// ++n;
	// // le troisieme.
	// _walls [n]._x1 = lab_width-1; _walls [n]._y1 = lab_height-1;
	// _walls [n]._x2 = lab_width-1; _walls [n]._y2 = 0;
	// _walls [n]._ntex = 0;
	// ++n;
	// // le dernier.
	// _walls [n]._x1 = lab_width-1; _walls [n]._y1 = 0;
	// _walls [n]._x2 = 0; _walls [n]._y2 = 0;
	// _walls [n]._ntex = 0;
	// ++n;
	// _nwall = n;

	// une affiche.
	//  (attention: pour des raisons de rapport d'aspect, les affiches doivent faire 2 de long)
	// n = 0;
	// _picts = new Wall [2];
	// // la premi�re (texture par d�faut).
	// _picts [n]._ntex = 0;
	// _picts [n]._x1 = 0; _picts [n]._y1 = 4;
	// _picts [n]._x2 = 0; _picts [n]._y2 = 6;
	// ++n;

	// // la deuxi�me a une texture diff�rente.
	// char	tmp [128];
	// sprintf (tmp, "%s/%s", texture_dir, "feu.jpg");

	// _picts [n]._ntex = wall_texture (tmp);
	// _picts [n]._x1 = 0; _picts [n]._y1 = 8;
	// _picts [n]._x2 = 0; _picts [n]._y2 = 10;
	// ++n;
	// _npicts = n;

	// // 3 caisses.
	// _boxes = new Box [3];

	// int n = 0;
	// // la premi�re.
	// _boxes [n]._x = 12; _boxes [n]._y = 70; _boxes [n]._ntex = 0;
	// ++n;
	// // la deuxieme.
	// _boxes [n]._x = 5; _boxes [n]._y = 10; _boxes [n]._ntex = 0;
	// ++n;
	// // la derni�re.
	// _boxes [n]._x = 22; _boxes [n]._y = 65; _boxes [n]._ntex = 0;
	// ++n;
	// _nboxes = n;


	// // cr�ation du tableau d'occupation du sol.

	// // initialisation du tableau d'occupation du sol.


	// // indiquer qu'on ne marche pas sur une caisse.
	// _data [_boxes [0]._x][_boxes [0]._y] = 1;
	// _data [_boxes [1]._x][_boxes [1]._y] = 1;
	// _data [_boxes [2]._x][_boxes [2]._y] = 1;


	// _boxes = new Box [2];

	// int n = 0;
	// // la premi�re.
	// _boxes [n]._x = 1; _boxes [n]._y = 3; _boxes [n]._ntex = 0;
	// ++n;
	// // la deuxieme.
	// _boxes [n]._x = 1; _boxes [n]._y = 5; _boxes [n]._ntex = 0;
	// ++n;
	// _nboxes = n;
	// _data [_boxes [0]._x][_boxes [0]._y] = 1;
	// _data [_boxes [1]._x][_boxes [1]._y] = 1;

	// coordonn�es du tr�sor.
	

	//le chasseur et les 4 gardiens.
	// _nguards = 1;
	// _guards = new Mover* [_nguards];
	// 			Mover *chasseur = new Chasseur (this);
	// 			chasseur -> _x = 1*scale;
	// 			chasseur -> _y = 5*scale;
	// 			_guards[0] = chasseur;

	// _guards [0] = new Chasseur (this);
	// _guards [0]->_x = 2*scale;
	// _guards [0]->_y = 4*scale;
	// _guards [1] = new Gardien (this, "drfreak");
	// _guards [2] = new Gardien (this, "Marvin"); _guards [2] -> _x = 90.; _guards [2] -> _y = 70.;
	// _guards [3] = new Gardien (this, "Potator"); _guards [3] -> _x = 60.; _guards [3] -> _y = 10.;
	// _guards [4] = new Gardien (this, "garde"); _guards [4] -> _x = 130.; _guards [4] -> _y = 100.;

	// // indiquer qu'on ne marche pas sur les gardiens.
	// _data [(int)(_guards [1] -> _x / scale)][(int)(_guards [1] -> _y / scale)] = 1;
	// _data [(int)(_guards [2] -> _x / scale)][(int)(_guards [2] -> _y / scale)] = 1;
	// _data [(int)(_guards [3] -> _x / scale)][(int)(_guards [3] -> _y / scale)] = 1;
	// _data [(int)(_guards [4] -> _x / scale)][(int)(_guards [4] -> _y / scale)] = 1;
}
