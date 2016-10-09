#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Particle.h"
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif
using namespace std;

class HashCell{
public:
	HashCell(int, int );  //constructor
	~HashCell();  //destructor
	vector<Particle*> getSurroundingParticles(Particle* ); //get surrounding particles
	int oneDHash(float pos,int d); //hash function
	void insert(Particle* p); //insert particle into HashCell
	int width; //width of collision region
	int height; //height of colission region
	void print();
	vector<Particle*>** universe; //data structure that stores particles
	int size();

private:

};
