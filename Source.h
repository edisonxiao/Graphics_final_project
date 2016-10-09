// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#ifndef Filter_H 
#define Filter_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif
using namespace std;

class Source{	
public:
	Source(float rate, float yVelocity, int trans); // constructor
	Particle* emit(float rad, float vrad, float xV, float yV, float zV, float x, float y, float z, float opacity);
	float rate; // rate at which particles are "emitted"
	float velocity; //initial downwards velocity of particle


private:
};


#endif