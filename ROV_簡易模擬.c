/*-----------------------------------------------------------
 * An example program to draw a car
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#define  PI   3.141592653

#define Step  0.5
 

int goR=0;
int goL=0;
int goU=0;
int goD=0;
int goF=0;
int landing = 0;

float ang_fac=1.0;

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int    face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
float  colors[6][3]={{1.0,1.0,1.0}, {0.9,1.0,0.1}, {0.4,0.6,0.5},
                     {1.0,1.0,1.0}, {0.9,1.0,0.1}, {0.4,0.6,0.5}};

float stonePlace[6][3]={{-34.0,-20.0,12.0},{-10.0,-20.0,-12.0},{-35.0,-20.0,0.0},{10.0,-20.0,16.0},{30.0,-18.0,45.0},{25.0,-19.0,-3.0}};
float stoneRadius[6]={1.0,1.0,1.5,1.3,1.0,2.0};

float ballPlace[3]={8.0,-18.0,30.0};
float obstacle1[3]={20.0,-20.0,-30.0};
float obstacle2[3]={-30.0,-20.0,10.0};

float blade_ang=10.0;
float up_blade_ang=10.0;
float front_blade_ang = 10.0;
float left_front_arm_ang=0.0;
float right_front_arm_ang=0.0;
float left_body_arm_ang=0.0;
float right_body_arm_ang=0.0;
int catch_ball=0;
float drop=5.0;


/* indices of the box faces */
int    cube[6] = {0, 1, 2, 3, 4, 5};

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj  *sphere=NULL, *cylind=NULL, *disk;


/*-Declare car position, orientation--*/
float  self_ang=-90.0, glob_ang=0.0;
float  position[3]={8.0, 0.0, 0.0};

/*-----Define window size----*/
int width=512, height=512;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  /*---Create quadratic objects---*/
  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }
  if(disk==NULL){
    disk = gluNewQuadric();
    gluQuadricDrawStyle(disk, GLU_FILL);
    gluQuadricNormals(disk, GLU_SMOOTH);
  }
}

/*--------------------------------------------------------
 Procedure to draw a radius=0.5 circle. */
void draw_circle()
{
  gluDisk(disk,
       0.0,           /* inner radius=0.0 */
	  0.5,          /* outer radius=0.5 */
	  16,            /* 16-side polygon */
	   3);
}


/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_cube()
{
  int   i;

  for(i=0;i<6;i++){
    glColor3fv(colors[i]);  /* Set color */
    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 blade cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_blade()
{
  int i;
  glColor3f(0.5,0.5,0.5);  /* Set color */
  for(i=0;i<6;i++){
    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}



/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{

	glColor3f(0.9,0.8,0.4);
    glBegin(GL_POLYGON);
     glVertex3f(-50.0, -20.0,-50.0);
     glVertex3f(100.0, -20.0, -50.0);
     glVertex3f(100.0, -20.0, 100.0);
     glVertex3f(-50.0, -20.0, 100.0);
    glEnd();
}

/*---------------------------------------------------------
 * Procedure to draw the background.
 */
void draw_background()
{
  
  glColor3f(0.0, 0.2, 0.5);
   glBegin(GL_POLYGON);
     glVertex3f(-50.0, 80.0,-50.0);
	 glVertex3f(50.0, 80.0, -50.0);
     glVertex3f(50.0, -20.0, -50.0);
     glVertex3f(-50.0, -20.0, -50.0);
   glEnd();
   glColor3f(0.0, 0.2, 0.3);
   glBegin(GL_POLYGON);
     glVertex3f(-50.0, 80.0,-50.0);
	 glVertex3f(-50.0, -20.0, -50.0);
     glVertex3f(-50.0, -20.0, 50.0);
     glVertex3f(-50.0, 80.0, 50.0);
   glEnd();
}
void draw_cyli(){
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      20.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */  
}

/*---------------------------------------------------------
 * Procedure to draw the background.
 */
void draw_obstacle()
{
	int i;
	glPushMatrix();
	glColor3f(0.3,0.3,0.4);
	glTranslatef(stonePlace[0][0],-20.0,stonePlace[0][2]);
	gluSphere(sphere, stoneRadius[0],  
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
	for(i=1;i<6;i++){
		glTranslatef(stonePlace[i][0]-stonePlace[i-1][0],stonePlace[i][1]-stonePlace[i-1][1],stonePlace[i][2]-stonePlace[i-1][2]);
		gluSphere(sphere, stoneRadius[i],   
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
	}
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9,0.9,0.9);
	glTranslatef(obstacle1[0],obstacle1[1],obstacle1[2]);
	glRotatef(-90,1.0,0.0,0.0);
	draw_cyli();
	glPushMatrix();
	glRotatef(15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-35,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(40,1.0,0.0,0.0);
	glRotatef(15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPopMatrix();

	glColor3f(0.0,0.9,0.1);
	glTranslatef(obstacle2[0],obstacle2[1],obstacle2[2]);
	glRotatef(-90,1.0,0.0,0.0);
	draw_cyli();
	glPushMatrix();
	glRotatef(15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-35,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	glPushMatrix();
	glRotatef(40,1.0,0.0,0.0);
	glRotatef(15,0.0,1.0,0.0);
	draw_cyli();
	glPopMatrix();
	
}

/*---------------------------------------------------------
 * Procedure to draw the background.
 */
void draw_ball()
{
	int i;
	glColor3f(0.9,0.0,0.3);
	if(catch_ball==0){
		glTranslatef(ballPlace[0],-18.0,ballPlace[2]);
	}
	else{
		if(position[1]-drop>-18){
			//glRotatef(-self_ang/2,0.0,1.0,0.0);
			glTranslatef(position[0],position[1]-drop,position[2]+12.0);
			//glRotatef(self_ang,0.0,1.0,0.0);
			printf("%f %f %f %f\n",position[0],position[1],position[2],self_ang);
			drop+=0.02;
		}
		else{
			//glRotatef(-self_ang/2,0.0,1.0,0.0);
			glTranslatef(position[0],-18.0,position[2]+12.0);
			//glRotatef(self_ang,0.0,1.0,0.0);
			ballPlace[0]=position[0];
			ballPlace[1]=-18.0;
			ballPlace[2]=position[2]+12.0;
			drop=5.0;
			catch_ball=0;
		}
	}
	gluSphere(sphere, 2.0,   /* radius=2.0 */
	    12,            /* composing of 12 slices*/
	    12); 
	//glTranslatef(0.0,0.0,-4.0);
	//glRotatef(self_ang,0.0,1.0,0.0);
	//gluSphere(sphere, 2.0,   /* radius=2.0 */
	//    12,            /* composing of 12 slices*/
	//    12); 
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{

  /*----Draw a white sphere to represent the original-----*/
  glColor3f(0.9, 0.9, 0.9);

  gluSphere(sphere, 2.0,   /* radius=2.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
  glColor3f(0.0, 0.95, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  
  /* Draw Y axis */
  glPushMatrix();
  glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
  glColor3f(0.95, 0.0, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	     3);               /* Divide it into 3 sections */
  glPopMatrix();

  /* Draw X axis */
  glColor3f(0.95, 0.95, 0.0);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
  gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  glPopMatrix();
  /*-- Restore the original modelview matrix --*/
  glPopMatrix();
}


/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
  static float  ang_self=0.0;  /*Define the angle of self-rotate */
  static float  angle=0.0;

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(15.0, 5.0, 15.0, 4.0, 0.0, 0.0, 0.0, 1.0, 0.0); //viewing.
  //gluLookAt(4.0, 15.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //viewingTest.

   

  //draw_floor();

  draw_axes();

  draw_background();  // draw the dark blue sea

  glPushMatrix();
  draw_obstacle();
  glPopMatrix();

  if(catch_ball!=1){
	glPushMatrix();
	draw_ball();
	glPopMatrix();
  }

  /*-------Draw the ROV body, a cube----*/
  glTranslatef(position[0],position[1], position[2]); 
  //printf("%f %f %f\n",position[0],position[1],position[2]);
  //printf("%d",catch_ball);



  glRotatef(self_ang, 0.0, 1.0, 0.0);
  glPushMatrix();              /* Save M1 coord. sys */
  glScalef(16.0, 6.0, 10.0);    /* Scale up the axes */
  draw_cube();
  glPopMatrix();               /* Get M1 back */

  /*-------Draw the up bar -----*/
  glColor3f(0.0, 0.0, 0.0);
  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(0.0, 3.0, 0.0); /* Go to left wheel position */
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      2.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */

  //draw the up joint
  glTranslatef(0.0,0.0,2.0);
  gluSphere(sphere, 1.0,   /* radius=1.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  
  up_blade_ang += ang_fac;
  if(up_blade_ang>360.0){
    up_blade_ang -= 360.0;
  }
  //draw 3 blades
  glPushMatrix();
  glRotatef(up_blade_ang,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); //back to joint
  
  glPushMatrix();
  glRotatef(up_blade_ang,0.0,0.0,1.0);
  glRotatef(120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 

  glPushMatrix();
  glRotatef(up_blade_ang,0.0,0.0,1.0);
  glRotatef(-120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 


  glPopMatrix();  //back to M1 coord (position[0],0.0,position[2])

  blade_ang+=ang_fac;
	if(blade_ang>360.0){
		blade_ang-=360.0;
	}

  /*------Draw the right bar. ----*/
  glColor3f(0.0, 0.0, 0.0);
  glPushMatrix();              /* Save M1 coord. sys */
  glTranslatef(0.0, 0.0, -5.0); /* Go to right bar position */
  glRotatef(180.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      2.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);

   //draw the right joint
  glTranslatef(0.0,0.0,2.0);
  gluSphere(sphere, 1.0,   /* radius=1.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  
  //draw 3 blades
  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); //back to joint
  
  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glRotatef(120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 

  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glRotatef(-120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 
  glPopMatrix();   //back to M1 coord (position[0],0.0,position[2])

  goL=0;
//------------------------- Draw the left bar.----------------------------
  glPushMatrix();       /* Save M1 coord. sys */
  glColor3f(0.0,0.0,0.0);
  glTranslatef(0.0,0.0,5.0);/* Go to left bar position */
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      2.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  //draw the left joint
  glTranslatef(0.0,0.0,2.0);
  gluSphere(sphere, 1.0,   /* radius=1.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  

  //draw 3 blades
  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); //back to joint
  
  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glRotatef(120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 

  glPushMatrix();
  glRotatef(blade_ang,0.0,0.0,1.0);
  glRotatef(240,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 
  glPopMatrix();   //back to M1 coord (position[0],0.0,position[2])

  goR=0;

  //------------------------- Draw the back bar.----------------------------
  glPushMatrix();       /* Save M1 coord. sys */
  glColor3f(0.0,0.0,0.0);
  glTranslatef(-8.0,0.0,0.0);/* Go to left bar position */
  glRotatef(-90,0.0,1.0,0.0);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      2.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  //draw the left joint
  glTranslatef(0.0,0.0,2.0);
  gluSphere(sphere, 1.0,   /* radius=1.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
 
  if(goF==1){
	  front_blade_ang+=ang_fac;
	  if(front_blade_ang>360){
		  front_blade_ang-=360;
	  }
  }

  //draw 3 blades
  glPushMatrix();
  glRotatef(front_blade_ang,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); //back to joint
  
  glPushMatrix();
  glRotatef(front_blade_ang,0.0,0.0,1.0);
  glRotatef(120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 

  glPushMatrix();
  glRotatef(front_blade_ang,0.0,0.0,1.0);
  glRotatef(-120,0.0,0.0,1.0);
  glTranslatef(1.5,0.0,0.0);
  glScalef(3.0,1.0,0.5);
  draw_blade();
  glPopMatrix(); 
  glPopMatrix();   //back to M1 coord (position[0],0.0,position[2])


  // draw the right arm ----------------------------------------

  glColor3f(1,0.4,0.4);
  glTranslatef(8.0,2.0,0.0);
  glPushMatrix();   //M2=(8.0,3.0,8.0) in  world coord
  glTranslatef(0.0,0.0,4.0);
  gluSphere(sphere, 0.75,   /* radius=0.75 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */

  glRotatef(90,0.0,1.0,0.0);
  glRotatef(45,1.0,0.0,0.0);
  glRotatef(right_body_arm_ang,1.0,0.0,0.0);
  gluCylinder(cylind, 0.75, 0.75, /* radius of top and bottom circle */
	      6.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  glTranslatef(0.0,0.0,6.0);
  gluSphere(sphere, 0.75,   /* radius=0.75 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  glRotatef(-45-right_body_arm_ang,1.0,0.0,0.0);
  glRotatef(right_front_arm_ang,0.0,1.0,0.0);
  gluCylinder(cylind, 0.75, 0.75, /* radius of top and bottom circle */
	      6.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  if(catch_ball==1){                       // if catch the ball
	  glColor3f(0.9,0.0,0.3);
	  glTranslatef(0.0,0.0,6.0);
	  gluSphere(sphere, 2.0,   /* radius=2.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  }
  glPopMatrix();   //back to M2

  // draw the left arm
  glColor3f(1,0.4,0.4);
  glPushMatrix();
  glTranslatef(0.0,0.0,-4.0);
  gluSphere(sphere, 0.75,   /* radius=0.75 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */

  glRotatef(90,0.0,1.0,0.0);
  glRotatef(45,1.0,0.0,0.0);
  glRotatef(left_body_arm_ang,1.0,0.0,0.0);
  gluCylinder(cylind, 0.75, 0.75, /* radius of top and bottom circle */
	      6.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  glTranslatef(0.0,0.0,6.0);
  gluSphere(sphere, 0.75,   /* radius=0.75 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */
  glRotatef(-45-left_body_arm_ang,1.0,0.0,0.0);
  glRotatef(left_front_arm_ang,0.0,1.0,0.0);
  gluCylinder(cylind, 0.75, 0.75, /* radius of top and bottom circle */
	      6.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  glPopMatrix();   //back to M2
  
  // Draw the Camera
  glColor3f(0.9,0.1,0.5);
  glPushMatrix();
  glTranslatef(-1.0,1.0,0.0);
  glRotatef(-90,1.0,0.0,0.0);
  gluCylinder(cylind, 0.2, 0.2, /* radius of top and bottom circle */
	      2.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  glTranslatef(0.0,0.0,3.0);
  glColor3f(0.9,0.2,0.5);
  glPushMatrix();
  glScalef(1.0,4.0,2.0);
  draw_cube();
  glPopMatrix();
  glColor3f(0.0,0.0,0.0);
  glTranslatef(0.0,0.0,0.0);
  glRotatef(90,0.0,1.0,0.0);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      1.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
  glTranslatef(0.0,0.0,1.0);
  draw_circle();
  glPopMatrix();

  if(landing==1){   //landing part
	  glPushMatrix();
	  glTranslatef(-5.0,-3.0,-5.0);
	  glColor3f(0.3,0.2,0.9);
	  glPushMatrix();
	  glRotatef(180,0.0,1.0,0.0);
	  glRotatef(60,1.0,0.0,0.0);
	  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      5.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
	  glPopMatrix();
	  glTranslatef(-8.0,0.0,0.0);
	  glPushMatrix();
	  glRotatef(180,0.0,1.0,0.0);
	  glRotatef(60,1.0,0.0,0.0);
	  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      5.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
	  glPopMatrix();
	  glTranslatef(0.0,0.0,10.0);
	  glRotatef(60,1.0,0.0,0.0);
	  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      5.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
	  glTranslatef(8.0,0.0,0.0);
	  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      5.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);
	  glPopMatrix();
  }
  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  return;
}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w>h)
    glOrtho(-40.0, 40.0, -40.0*(float)h/(float)w, 40.0*(float)h/(float)w, 
             -100.0, 100.0);
  else
    glOrtho(-40.0*(float)w/(float)h, 40.0*(float)w/(float)h, -40.0, 40.0, 
            -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int x, int y)
{
  if(key=='Q'||key=='q') exit(0);
  if(key=='f'){
	goF=1;
	position[0] += Step*cos(self_ang*PI/180.0);
	position[2] -= Step*sin(self_ang*PI/180.0);
	if(position[1]<1){
	  if(pow((float)(abs(position[0]-obstacle1[0])+abs(position[2]-obstacle1[2])),(float)0.5) < 5.5){    //ª«Åé¸I¼²°»´ú
		  position[0] -= Step*cos(self_ang*PI/180.0);
		  position[2] += Step*sin(self_ang*PI/180.0);
	  }
	  else if(pow((float)(abs(position[0]-obstacle2[0])+abs(position[2]-obstacle2[2])),(float)0.5) < 5.5){    //ª«Åé¸I¼²°»´ú
		  position[0] -= Step*cos(self_ang*PI/180.0);
		  position[2] += Step*sin(self_ang*PI/180.0);
	  }  
	}
  }else if(key=='b'){  
	goF=1;
    position[0] -= Step*cos(self_ang*PI/180.0);
    position[2] += Step*sin(self_ang*PI/180.0);
	if(position[1]<1){
	  if(pow((float)(abs(position[0]-obstacle1[0])+abs(position[2]-obstacle1[2])),(float)0.5) < 5.5){    //ª«Åé¸I¼²°»´ú
		  position[0] += Step*cos(self_ang*PI/180.0);  
		  position[2] -= Step*sin(self_ang*PI/180.0);
	  }  
	  else if(pow((float)(abs(position[0]-obstacle2[0])+abs(position[2]-obstacle2[2])),(float)0.5) < 5.5){    //ª«Åé¸I¼²°»´ú
		  position[0] += Step*cos(self_ang*PI/180.0);  
		  position[2] -= Step*sin(self_ang*PI/180.0);
	  }  
	}
  }else if(key=='r'){
    goR=1;
	goF=0;
	self_ang += 5.0;
  }else if(key=='c'){
	goL=1;
	goF=0;
	self_ang -= 5.0;
  }
  else if(key=='u'){          //go up
	  if(position[1]!=76){
		  goU=1;
		  position[1]+=0.25;
	  }
	  landing = 0 ;
  }    
  else if(key=='d'){                 //go down
	  if(position[1]!=-14){   //if do not touch the floor
		  goD=1;
		  position[1]-=0.25;
	  }
	  else{
		  landing = 1;
	  }
  }    
  else if(key=='p'){                 //speed up
	  if(ang_fac < 5.0){
		  ang_fac+=0.5;
	  }
  }  
  else if(key=='l'){                 //speed down
	  if(ang_fac > 0.5){
		  ang_fac-=0.5;
	  }
  }  
  else if(key=='i'){
	  if(right_body_arm_ang!=-90){
		   right_body_arm_ang-=1.0;
		   left_body_arm_ang-=1.0;
	  }
  }
  else if(key=='k'){
	  if(right_body_arm_ang!=45){
		   right_body_arm_ang+=1.0;
		   left_body_arm_ang+=1.0;
	  }
  }
  else if(key=='j'){
	  if(right_front_arm_ang!=45){
		right_front_arm_ang+=1.0;
		left_front_arm_ang-=1.0;
	  }
	  else if(pow((float)(abs(position[0]-ballPlace[0])+ abs(position[2]-ballPlace[2])),(float)0.5)<5.0){
		  catch_ball=1;
	  }
  }
  else if(key=='m'){
	  if(left_front_arm_ang!=45){
		right_front_arm_ang-=1.0;
		left_front_arm_ang+=1.0;
	  }
	  if(catch_ball==1){
		  catch_ball=2;
	  }
  }

  display();
}

void idle_func()
{
  
  display(); /* show the scene again */
}


/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);

  glutInitWindowSize(width, height);
  glutCreateWindow("ROV");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  glutIdleFunc(display); 
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_quit);

  glutMainLoop();
}

