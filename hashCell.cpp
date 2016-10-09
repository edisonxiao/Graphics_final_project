// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#include <algorithm>
#include <iostream>
#include <cmath>
#include "glut.h"
#include "Particle.h"
#include "Source.h"
#include <vector>
#include "HashCell.h"
#include <exception>

using namespace std;

//constructor: creating a 10 by 10 grid to divide up the collision region
HashCell::HashCell(int w, int h){ 
	width=w;
	height=h;
	vector<Particle*>** group=new vector<Particle*>* [10];

	for (int i=0;i<10; i++){
		group[i]=new vector<Particle*>[10];  //use vectors to store particles in the same cell
		for (int j=0; j<10;j++){
			group[i][j]=vector<Particle*>();
		}
	}
	universe=group;

}

//destructor
HashCell::~HashCell(){ 
	for (int i=0;i<10; i++){
		for (int j=0; j<10; j++){
			universe[i][j].clear();
		}
		delete [] universe[i];


	}
	delete [] universe;

}


//insert particles in HashCell
void HashCell:: insert(Particle* p){
	int x=oneDHash(p->xpos, 0);
	int y=oneDHash(p->ypos,1);
	universe[y][x].push_back(p);
}

//get particles around the give particle
vector<Particle*> HashCell::getSurroundingParticles(Particle* p){
	//calculate hashed value
	int x=oneDHash(p->xpos,0);
	int y=oneDHash(p->ypos,1);
	
	vector<Particle*> v=vector<Particle*>();
	//loop through particles in cells that are adjacent to the cell that the given particle is in
	for (int i=0; i<3;i++){
		for (int j=0; j<3; j++){
			if (y-1+i<0||y-1+i>10-1||x-1+j<0||x-1+j>10-1){ //edge case
				continue;
			}
			vector<Particle*> temp= universe[y-1+i][x-1+j];
			for(int k=0; k<temp.size(); k++){
				v.push_back(temp[k]);
			}
		}
	}
	return v;
}

//hash function
int HashCell:: oneDHash(float pos, int direction){ //direction: 0:x, 1:y, 2:z

	int ret=0;
	if (direction==0){
		ret=floor((pos+width/2.f)/(width/10.f));
	}
	if(direction==1){
		ret= floor((pos+20.f)/(height/10.f));
	}

	if(ret==10){  //special case when ret=10
		ret=9;
	}

	return ret;
}

//print out the HashCell and number of particles in each cell
void HashCell::print(){
	cout<< "----------------------------------------\n";
	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			int temp=universe[i][j].size();
			if(j<width-1){
				printf("%u!",temp);
			}

			if(j==width-1){
				printf("%u$\n",temp);
			}

		}

	}


}

//get the total number of particles in HashCell
int HashCell::size(){
	int counter=0;
	for (int i=0;i<10;i++){
		for (int j=0;j<10;j++){
			counter+=universe[i][j].size();
		}

	}
	return counter;
}




