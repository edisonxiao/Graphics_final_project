// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#ifndef Particle_H 
#define Particle_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif
using namespace std;

class Particle{	
public:
	Particle(float rad, float vrad, float xV, float yV, float zV, float x, float y, float z, float opacity);
	void draw(float xPos, float yPos, float zPos);
	void lifeCheck();
	float radius; // radius of the particle used for collision calculations
	float vradius; // radius of the particle used for visual representation
	float xpos; // x position
	float ypos; // y position
	float zpos; // z position
	float xVelocity; 
	float yVelocity;
	float zVelocity;
	float lifetime; // how long the particle is allowed to "live" (measured in clock ticks)
	clock_t birth; // the time at which the particle is created
	float alpha; // opacity of the particle (visual representation)
	int alive;  //whether the particle is alive, i.e. in the window
	


private:
	

};


#endif