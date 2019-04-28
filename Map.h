#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Map{
private:
    char** map;
    int maxheight;
    int maxwidth;
    
    void readFile(char* filename){
        cout << filename;
        fstream fin(filename, fstream::in);
        maxheight = 0;
        maxwidth = 0;
        bool isStart = false;
        char str[256];
        while(fin.getline(str, 256)){
            if(isStart){
                maxwidth += 1;
                int len = strlen(str);
                maxheight = maxheight < len ? len : maxheight;
            } else {
                int len = strlen(str);
                for(int i=0; i<len; i++){
                    if(str[i] == '+'){
                        isStart = true;
                        maxwidth += 1;
                        maxheight = maxheight < len ? len : maxheight;
                        break;
                    }
                }
            }
            
        }
        fin.close();
        
        char ch;
        map = new char*[maxwidth];
        for(int i = 0; i<maxwidth; i++){
            map[i] = new char[maxheight];
        }

        for(int i = 0; i<maxwidth; i++){
            for(int j = 0; j <maxheight; j++){
                map[i][j] = ' ';
            }
        }

        isStart = false;
        fstream ffin(filename, fstream::in);
        int col = -1;
        int raw = 0;
        while(ffin.get(ch)) {
            if(!isStart){
                if(ch == '+'){
                    isStart = true;
                    col += 1;
                    //cout <<
                    map[raw][col] = ch;
                }
            } else {
                if(ch == '\n'){
                    raw += 1;
                    col = -1;
                } else {
                    col += 1;
                    map[raw][col] = ch;
                }
                
            }
        
        }

        //cout << "maxwidth : " << maxwidth << " maxheight : " <<  maxheight << endl;

        ffin.close();

        //delete [] str;
    };

public:

    Map(char * fn){
        cout << fn << endl;
        readFile(fn);
    };

    char** getMap(){
        /*for(int i = 0; i<maxwidth; i++) {
            for(int j = 0; j<maxheight; j++){
                if(map[i][j])
                //cout << map[i][j]; 
            }
            cout << endl;
        }
        cout << "return map"<< endl;*/
        return map;
    };
    int getMapWidth(){return maxwidth;};
    int getMapHeight(){return maxheight;};
};


#endif