// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#include <algorithm>
#include <iostream>
#include <cmath>
#include "glut.h"
#include "Particle.h"
#include "Source.h"
#include <vector>

	// constructor
	Source::Source(float r, float v, int t){
	rate=r;
	velocity=v;
	
	}

	// creates and returns a new particle with input parameters
	Particle* Source::emit(float rad, float vrad, float xV, float yV, float zV, float x, float y, float z, float opacity){
		Particle* p=new Particle(rad, vrad, xV, yV, zV,x,y,z,opacity);
		return p;
	}





