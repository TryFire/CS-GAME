#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

float get_id_by_x_y(float x, float y) {
	return x*10+y;
}

float get_x_by_id(float id) {
	return (float)(((int)id)/10);
}

float get_y_by_id(float id) {
	return (float)(((int)id)%10);
}

float get_HN(float now, float end){
	return abs(get_x_by_id(now) - get_x_by_id(end))+abs(get_y_by_id(now) - get_y_by_id(end));
}

float* get_points_of_4_direction(float id){
	float x = get_x_by_id(id);
	float y = get_y_by_id(id);
	float id1 = get_id_by_x_y(x, y-1);
	float id2 = get_id_by_x_y(x, y+1);
	float id3 = get_id_by_x_y(x-1, y);
	float id4 = get_id_by_x_y(x+1, y);
	float * res = new float[4];
	res[0] = id1;
	res[1] = id2;
	res[2] = id3;
	res[3] = id4;
	return res;
}

void put(std::vector<float *>& open_list, float p[]){
	bool is_insert = false;
	for (std::vector<float *>::iterator it = open_list.end(); it != open_list.begin(); --it)
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

bool is_accessiabl(int ** _data, float id){
	int x = (int)get_x_by_id(id);
	int y = (int)get_y_by_id(id);
	return _data[x][y] == 0;
}

bool in_vector(std::vector<float *> close_list, float id) {
	for (int i = 0; i < close_list.size(); ++i)
	{
		if(close_list[i][0] == id) {
			return true;
		}
	}
	return false;
}

int get_index_of_element(std::vector<float *> close_list, float id){
	for (int i = 0; i < close_list.size(); ++i)
	{
		if(close_list[i][0] == id) {
			return i;
		}
	}
	return -1;
}

void update_vector(std::vector<float *>& open_list, int index, float newFN, float newGN) {
	open_list[index][2] = newFN;
	open_list[index][3] = newGN;
	int i = index+1;
	while(i < open_list.size()) {
		if (newFN < open_list[i][2]){
			float * temp = open_list[i];
			open_list[i] = open_list[index];
			open_list[index] = temp;
			index = i;
			++i;
		} else {
			break;
		}
	}
	
}

void show_vector(std::vector<float *> v){
	for (int i = 0; i < v.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout <<v[i][j] << "  "; 
		}
		cout << endl;
	}
}

void get_route(std::vector<float *> close_list, float start, float id) {
	std::vector<float> route;

	while(id != start) {
		for (int i = close_list.size()-1; i > 0; --i)
		{
			if (close_list[i][0] == id)
			{
				route.push_back(id);
				id = close_list[i][1];
				break;
			}
		}
	}

	for (int i = route.size()-1; i >= 0; --i)
	{
		float x = get_x_by_id(route[i]);
		float y = get_y_by_id(route[i]);
		cout << "(" << x << "," << y << ") -> ";
	}
}

int main(int argc, char const *argv[])
{

	int ** _data = new int*[10];
	for (int i = 0; i < 10; ++i)
	{
		_data[i] = new int[10];
		for (int j = 0; j < 10; ++j)
		{
			_data[i][j] = 0;
		}
	}

	for (int i = 2; i < 7; ++i)
	{
		_data[i][4] = 1;
	}

	for (int i = 0; i < 10; ++i)
	{
		_data[i][0] = 1;
		_data[i][9] = 1;
		for (int j = 0; j < 10; ++j)
		{
			_data[0][j] = 1;
			_data[9][j] = 1;
			cout << _data[i][j] << " ";
		}
		cout << endl;
	}

	int X,Y = 10;

	float start = get_id_by_x_y(4,2);
	float end = get_id_by_x_y(3,7);

	cout << start << endl;
	cout << end << endl;

	float DPN = 1.0;

	float HN = get_HN(start, end);
	float S[4] = {start, start, HN, 0};
	std::vector<float *> open_list;
	std::vector<float *> close_list;

	put(open_list, S);
	
	int iter = 0;
	while(!open_list.empty()) {
		iter+= 1;
		cout << "fois" << endl;
		float* P = open_list.back();
		open_list.pop_back();
		float* Ns = get_points_of_4_direction(P[0]);
		for (int i = 0; i < 4; ++i)
		{
			float x = get_x_by_id(Ns[i]);
			float y = get_y_by_id(Ns[i]);
			cout << "(" << x << "," << y << ") ";

			if(!is_accessiabl(_data, Ns[i])) {
				continue;
			}
			if(in_vector(close_list, Ns[i])) {
				continue;
			}
			if(in_vector(open_list, Ns[i])) {
				int index = get_index_of_element(open_list, Ns[i]);
				float newGN = P[3] + DPN;
				float newFN = newGN + get_HN(Ns[i], end);

				if (newFN < open_list[index][2]) {
					update_vector(open_list, index, newFN, newGN);
				}
			} else {
				float GN = P[3] + DPN;
				float FN = GN + get_HN(Ns[i], end);
				float *N = new float[4];
				N[0] = Ns[i];
				N[1] = P[0];
				N[2] = FN;
				N[3] = GN;
				// for (int k = 0; k < 4; ++k)
				// {
				// 	cout << N[k] << "  ";
				// }
				// cout << endl;
				// cout << "=== one ssss ===" << endl;
				put(open_list, N);
				// show_vector(open_list);
				// cout << "=== one eeee ===" << endl;
			}
			
		}
		cout << endl;
		close_list.push_back(P);
		if(P[0] == end) {
			cout << "=====found====" << endl;
			get_route(close_list, start, P[0]);

			break;
		}

		

		// if (iter > 5) 		
		// {
		// 	break;
		// }

	}

	
	return 0;
}