// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#include <algorithm>
#include <iostream>
#include <cmath>
#include "glut.h"
#include "Particle.h"
#include <ctime>


//constructor
	Particle::Particle (float rad, float vrad, float xV, float yV, float zV, float x, float y, float z, float opacity){
		radius=rad;
		vradius = vrad;
		xpos=x;
		ypos=y;
		zpos=z;
		xVelocity=xV;
		yVelocity=yV;
		zVelocity=zV;
		birth=clock();
		alpha = opacity;
		alive=1;

	}

//draw particle
	void Particle::draw(float xPos, float yPos, float zPos){
		glPushMatrix();
		glTranslatef(xPos,yPos,zPos);
		glColor4f(76/255.f,150/255.f, 255/255.f, alpha);
		glutSolidCube(vradius);

		glPopMatrix();
	}

//determine whether particle is in the viewing window
	void Particle::lifeCheck(){
		if(xpos<-40 ||xpos>40||ypos<-25||ypos>25){
			alive=0;
		}

	}

