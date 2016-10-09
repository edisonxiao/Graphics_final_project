//  main.cpp
//  
//  Created by Tabitha Peck on 7/16/13.
//  Copyright (c) 2013 Tabitha Peck. All rights reserved.
//
// HW by Edison Xiao and Thomas Gettliffe.  
// Netid: yx26, tg72
#include <iostream>
#include <math.h>
#include "Particle.h"
#include "Source.h"
#include "HashCell.h"
#include <vector>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

Source* s;
float win_width = 512;
float win_height = 512;
float waterPressure;
std::vector<Particle*> particles;   // the vector to hold all of our existing particles
float gravity; // rate at which particles are pulled downward
float sceneFloor; // the surface (y coordinates) off of which the particles will "bounce"
float sceneCeiling; // only particles below this line will be checked for collisions
float dampening; // a constant dampening coefficient which reduces the energy of particles after collisions with surfaces
float pdamp; // dampening for particle collisions
float emissionRate; // rate at which particles are emitted from the source
float visualrad; // the apparent radius of the particles
float collisionrad; // the actual radius of the particles (as counted for collisions)
int pCount; // the number of particles allowed to exist at one time
std::clock_t start; // the beginning of the simulation
double previousTime=0; // used to track intervals of time for particle creation
double previousCollisionTime=0; //the time when last collision happened
int collisionDX; // width of collision region
int collisionDY; //height of collision region
float cupCoord;  //coordinate of the brim of cup
int waterFlow;   //whether water is flowing
float collisionTimer; //the time interval for every collision check
float deltaPressure; //change in pressure
float deltaCupHeight;

// standard OpenGL initializations
void init( void ){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glLoadIdentity();
	gluLookAt(0, 0, 60, 0, 0, 0, 0, 1, 0);

	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

}



// the idle function covers a lot of the simulation, including (in this order):
// 
// - particle movement (position updating)
// - particle collision with cup detection and calculation
// - particle-to-particle collision detection and calculation
// - particle generation
void idle( void ){

	//slow down the program in the beginning when the number of particles is small to even out speed of particles
	clock_t prestart=0;
	double deltatime=(16.0-(prestart-start)/(double) CLOCKS_PER_SEC)*150000.0;
	int i=0;
	while(i<deltatime){
		i++;
	}

	// particle movement
	for (int i=0; i<particles.size(); i++){
		clock_t now = clock();
		Particle* temp=particles[i];
		int x = temp->xpos;
		int y = temp->ypos;
		bool beyond = false;  // default case, used later
		double yVel = temp->yVelocity;
		double xVel = temp->xVelocity;
		double v = sqrt(pow(yVel,2.0) + pow(xVel,2.0)); //// magnitude of velocity
		temp->lifeCheck();   //kill particles outside of window
		if(temp->alive==0)    
			particles.erase(particles.begin()+i);
		
		temp->xpos+=temp->xVelocity; // update x position
		temp->yVelocity-=gravity;   // update y velocity (acceleration due to gravity)
		temp->ypos+=temp->yVelocity;  // update y position

		if (temp->ypos<=sceneFloor && x>-7 && x<7){ //bottom of cup: -6 to 6
			temp->yVelocity=abs(dampening*temp->yVelocity); // move it up to the floor 
			temp->ypos = sceneFloor;
		}

		if (y>-2.5*x-32 &&  y < -2.5*x -25 && y<cupCoord){ //cup left wall
			beyond=true;
			temp->ypos+=yVel;
			temp->xpos+=abs(xVel);
			temp->xVelocity=(v/2); //bounce off the wall
			temp->yVelocity=(v/2);
		}

		if (y>2.5*x-32 &&  y < 2.5*x-25 && y<cupCoord){  //cup right wall
			beyond = true;
			temp->ypos += yVel;
			temp->xpos -= abs(xVel);
			temp->xVelocity = -(v/2.f);  //bounce off the wall
			temp->yVelocity = (v/2.f);
		}

		if (abs(temp->yVelocity) < .01 && beyond){ // special case to fix a bug, particles would get stuck. This case says if the particle is moving very slowly ("stuck")
			temp->yVelocity = .1;  // then we give it some up velocity to get it moving
		}
	}





	// particle collision checking
	HashCell* hc= new HashCell(collisionDX,collisionDY); //creating hashCell to loop through all particles
	int particleNum=particles.size(); //number of particles currently alive
	printf(" %u",particleNum);
	double collisionTimePassed=(clock()-start)/(double) CLOCKS_PER_SEC; //collision timer that only checks collision every interval time
	if(collisionTimePassed-previousCollisionTime>=collisionTimer){
		for (int i=0; i<particleNum-1; i++){ //insert particles into hashcell
			Particle* p1=particles[i];
			if(p1->ypos>sceneFloor+collisionDY ||p1->ypos<sceneFloor ||p1->xpos<-collisionDX/2||p1->xpos>collisionDX/2){
				continue;}
			hc->insert(p1);
		}

		for (int i=0; i<particleNum-1; i++){ //loop through all particles
			Particle* p1=particles[i];
			//skip particles outside of collision region
			if(p1->ypos>sceneFloor+collisionDY ||p1->ypos<sceneFloor ||p1->xpos<-collisionDX/2||p1->xpos>collisionDX/2){
				continue;}
			vector<Particle*> nearbyParticles=hc->getSurroundingParticles(p1); //retreive nearby particles

			for (int j=0;j<nearbyParticles.size(); j++){
				Particle* p2=nearbyParticles[j];
				double v1 = sqrt(pow(p1->xVelocity,2.0) + pow(p1->yVelocity,2.0)); //calculate velocity of particle for later use
				double v2 = sqrt(pow(p2->xVelocity,2.0) + pow(p2->yVelocity,2.0));
				float xdiff=p1->xpos-p2->xpos;
				float ydiff=p1->ypos-p2->ypos;
				float zdiff=p1->zpos-p2->zpos;

				float distance=sqrt(xdiff*xdiff+ydiff*ydiff+zdiff*zdiff); //distance between particles


				if(distance<1.1*p1->radius){  // if the distance is less than the radius of the particle (in this case, the "radius" is actually the diameter, as we are using cubes and not spheres)
					float tempX=p1->xVelocity;  // switch velocity components (and dampen them slightly). This is consistent for collisions between particles of identical mass
					float tempY=p1->yVelocity;
					float tempZ=p1->zVelocity;

					p1->xVelocity=pdamp*p2->xVelocity;
					p1->yVelocity=pdamp*p2->yVelocity;
					p1->zVelocity=pdamp*p2->zVelocity;
					p2->xVelocity=pdamp*tempX;
					p2->yVelocity=pdamp*tempY;
					p2->zVelocity=pdamp*tempZ;

					if (v1<0.01){   //prevent particles from stucking together
						p1->xVelocity*=1/pdamp;
						p1->yVelocity*=1/pdamp;
						p1->zVelocity*=1/pdamp;
					}
					if (v2<0.01){
						p2->xVelocity*=1/pdamp;
						p2->yVelocity*=1/pdamp;
						p2->zVelocity*=1/pdamp;
					}

					//setting up sliding between two particles
					float p1x=p1->xpos;
					float p1y=p1->ypos;
					float p2x=p2->xpos;
					float p2y=p2->ypos;
					float xFactor;
					float yFactor;

					if (waterFlow==1){ //different sliding factor depending on whether the water is flowing
						xFactor=8.0;
						yFactor=8.0;
					}
					if (waterFlow==-1){
						xFactor=18.0;
						yFactor=18.0;
					}

					p1->xVelocity+=(p1x-p2x)*(v1+0.1)/xFactor;
					p2->xVelocity+=(p2x-p1x)*(v2+0.1)/xFactor;
					p1->yVelocity+=(p1y-p2y)*(v1)/yFactor;
					p2->yVelocity+=(p2y-p1y)*(v2)/yFactor;
				}
			}
		}

		delete hc; //delete hashCell
		previousCollisionTime=collisionTimePassed; //update previous collision time
	}

	double timePassed=(clock()-start)/(double) CLOCKS_PER_SEC;  //generating new particles very time interval
	if(timePassed-previousTime>=1.0/emissionRate && particleNum<=pCount && waterFlow==1){

		float randXV=(float) (rand()%200-100) / 1000.f;    //give particles random x velocity
		float randOPAC = (float)( rand()%25 +50) / 100.f;  //give particles random transparency
		particles.push_back(s->emit(collisionrad,visualrad,randXV,-waterPressure-deltaPressure,0,0,25,0,randOPAC));
		previousTime=timePassed;
	}
	glutPostRedisplay();

}

void display( void )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.f, win_width/win_height, .1f, 500.f);
	glMatrixMode(GL_MODELVIEW);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_LINE, GL_FILL);

	glPushMatrix();
	glLineWidth(20);
	glColor3f(.8, .8, .8);

	//draw the cup
	glBegin(GL_LINE_STRIP);
	glVertex3f((-cupCoord-32.f)/2.5, cupCoord, 0.0);
	glVertex3f(-5,sceneFloor-2.0,0.0);
	glVertex3f(5,sceneFloor-2.0,0);
	glVertex3f((cupCoord+32.f)/2.5,cupCoord,0);
	glEnd();

	//draw each particle currently alive
	for (int i=0; i<particles.size(); i++){
		Particle* temp=particles[i];
		float tempX=temp->xpos;
		float tempY=temp->ypos;
		float tempZ=temp->zpos;
		temp->draw(tempX,tempY,tempZ);
	}
	glPopMatrix();
	glutSwapBuffers();
}

void reshape( int w, int h )
{
	win_width = w;
	win_height = h;
	glViewport( 0, 0, w, h );

	glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {
	switch(key) {
	case 27: // Escape key
		exit(0);
		break;
	case 'f': waterFlow=-waterFlow; break;  //turn on/off the faucet
	case 'w': deltaPressure+=0.05; break;   //change water pressure
	case 's': 
		if (deltaPressure<=0.05){
			break;
		}
		deltaPressure-=0.05; break;
	}
	glutPostRedisplay();
}


int main(int argc, char * argv[])
{
	// this is where we assign the variables defined above
	collisionrad = 2;
	visualrad = 3;
	float cupHeight=25; //height of cup
	waterPressure=0.0;
	deltaPressure=0;
	pCount=400;
	collisionTimer=0;
	gravity=0.008;
	sceneFloor=-20.0;
	dampening=0.6;
	pdamp=0.98;
	start=std::clock();
	cupCoord=cupHeight+sceneFloor;
	waterFlow=1;
	Source* s=new Source(100,0,0);
	emissionRate=s->rate;
	collisionDX=cupCoord+30;  
	collisionDY=cupCoord+30;

	if (collisionDX<=12){
		collisionDX=12;
	}

	srand ( time(NULL));
	glutInit( &argc, argv ); // Inialize Glut processes 

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( win_width, win_height );

	glutCreateWindow( "Particles" );

	init();

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutIdleFunc( idle );
	glutMainLoop();

	return 0;
}