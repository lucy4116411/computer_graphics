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

int state=0;

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
int width=720, height=600;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx=0.0, eyeDy=0.0, eyeDz=0.0;
float   eyeAngx=0.0, eyeAngy=0.0, eyeAngz=0.0;
double  Eye[3]={0.0, 0.0, 30.0}, Focus[3]={0.0, 0.0, 0.0}, 
        Vup[3]={0.0, 1.0, 0.0};

float   u[3][3]={{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}};
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */
float   ang_fovy=90.0;
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0.0, 0.2, 0.5, 1.0);      /*set the background color BLACK */
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

   /*---- Compute cos(5.0) and sin(5.0) ----*/
  cv = cos(5.0*PI/180.0);
  sv = sin(5.0*PI/180.0);
  /*---- Copy eye position ---*/
  eye[0] = Eye[0];
  eye[1] = Eye[1];
  eye[2] = Eye[2];
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
	glColor3f(0.9,0.8,0.3);
	glBegin(GL_POLYGON);
     glVertex3f(30.0, -20.1,-50.0);
     glVertex3f(30.0, -50.0, -50.0);
     glVertex3f(30.0, -50.0, 100.0);
     glVertex3f(30.0, -20.1, 100.0);
    glEnd();
	glBegin(GL_POLYGON);
     glVertex3f(-100.0, -50.0,30.0);
     glVertex3f(-100.0, -20.1,30.0);
     glVertex3f(100.0, -20.1,30.0);
     glVertex3f(100.0, -50.0, 30.0);
    glEnd();
}

/*---------------------------------------------------------
 * Procedure to draw the background.
 */
void draw_background()
{
  
  glColor3f(0.0, 0.2, 0.5);
   glBegin(GL_POLYGON);
     glVertex3f(-1000.0, 80.0,-50.0);
	 glVertex3f(1000.0, 80.0, -50.0);
     glVertex3f(1000.0, -20.0, -50.0);
     glVertex3f(-1000.0, -20.0, -50.0);
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
void draw_scene()
{
  //static float  ang_self=0.0;  /*Define the angle of self-rotate */
  //static float  angle=0.0;


  draw_floor();

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

  return;
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(x==4){
	  gluPerspective(ang_fovy, (double) width/ (double) height,1.5, 100.0);
  }else{
      if(width>height)
	glOrtho(-40.0, 40.0, -40.0*(float)height/(float)width, 
		40.0*(float)height/(float)width, 
		-0.0, 100.0);
      else
	glOrtho(-40.0*(float)width/(float)height, 
		40.0*(float)width/(float)height, -40.0, 40.0, 
		-0.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
}



void make_view(int mode)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	switch(mode){
	case 4:
		 gluLookAt(eye[0], eye[1], eye[2], 
	      eye[0]-u[2][0], eye[1]-u[2][1], eye[2]-u[2][2],
	      u[1][0], u[1][1], u[1][2]);
		 break;
	case 1:       /* X direction parallel viewing */
		 gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	     break;
	case 2:       /* Y direction parallel viewing */
		 gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
		 break;
	case 3:
		 gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		 break;
	}

}
void draw_view_volumne()
{
	float fc[3],nc[3];
	float ftl[3],ftr[3],fbl[3],fbr[3],ntl[3],ntr[3],nbl[3],nbr[3];
	float fh,fw,tan_ang,nh,nw;
	tan_ang = (float)tan(ang_fovy*0.5*(PI/180.0));

	// far plane
	fh = (50.0*tan_ang);
	fw = fh * (width/height);
	fc[0] = eye[0] - u[2][0]* 50;
	fc[1] = eye[1] - u[2][1]* 50;
	fc[2] = eye[2] - u[2][2]* 50;

	ftl[0] = fc[0] + (u[1][0]*fh) - (u[0][0] * fw);
	ftl[1] = fc[1] + (u[1][1]*fh) - (u[0][1] * fw);
	ftl[2] = fc[2] + (u[1][2]*fh) - (u[0][2] * fw);
	ftr[0] = fc[0] + (u[1][0]*fh) + (u[0][0] * fw);
	ftr[1] = fc[1] + (u[1][1]*fh) + (u[0][1] * fw);
	ftr[2] = fc[2] + (u[1][2]*fh) + (u[0][2] * fw);
	fbl[0] = fc[0] - (u[1][0]*fh) - (u[0][0] * fw);
	fbl[1] = fc[1] - (u[1][1]*fh) - (u[0][1] * fw);
	fbl[2] = fc[2] - (u[1][2]*fh) - (u[0][2] * fw);
	fbr[0] = fc[0] - (u[1][0]*fh) + (u[0][0] * fw);
	fbr[1] = fc[1] - (u[1][1]*fh) + (u[0][1] * fw);
	fbr[2] = fc[2] - (u[1][2]*fh) + (u[0][2] * fw);

	/*glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(ftl[0],ftl[1],ftl[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(ftr[0],ftr[1],ftr[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(fbr[0],fbr[1],fbr[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(fbl[0],fbl[1],fbl[2]);
	glEnd();*/


	//near plane

	nh = (1.5*tan_ang);
	nw = nh * (width/height);
	nc[0] = eye[0] - u[2][0]* 1.5;
	nc[1] = eye[1] - u[2][1]* 1.5;
	nc[2] = eye[2] - u[2][2]* 1.5;

	ntl[0] = nc[0] + (u[1][0]*nh) - (u[0][0] * nw);
	ntl[1] = nc[1] + (u[1][1]*nh) - (u[0][1] * nw);
	ntl[2] = nc[2] + (u[1][2]*nh) - (u[0][2] * nw);
	ntr[0] = nc[0] + (u[1][0]*nh) + (u[0][0] * nw);
	ntr[1] = nc[1] + (u[1][1]*nh) + (u[0][1] * nw);
	ntr[2] = nc[2] + (u[1][2]*nh) + (u[0][2] * nw);
	nbl[0] = nc[0] - (u[1][0]*nh) - (u[0][0] * nw);
	nbl[1] = nc[1] - (u[1][1]*nh) - (u[0][1] * nw);
	nbl[2] = nc[2] - (u[1][2]*nh) - (u[0][2] * nw);
	nbr[0] = nc[0] - (u[1][0]*nh) + (u[0][0] * nw);
	nbr[1] = nc[1] - (u[1][1]*nh) + (u[0][1] * nw);
	nbr[2] = nc[2] - (u[1][2]*nh) + (u[0][2] * nw);

	glColor3f(1.0,0.0,0.0);
	/*glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(ntl[0],ntl[1],ntl[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(ntr[0],ntr[1],ntr[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(nbr[0],nbr[1],nbr[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(nbl[0],nbl[1],nbl[2]);
	glEnd();*/
	glBegin(GL_LINES);
		glVertex3f(ntl[0],ntl[1],ntl[2]);
		glVertex3f(ntr[0],ntr[1],ntr[2]);
		glVertex3f(ntr[0],ntr[1],ntr[2]);
		glVertex3f(nbr[0],nbr[1],nbr[2]);
		glVertex3f(nbr[0],nbr[1],nbr[2]);
		glVertex3f(nbl[0],nbl[1],nbl[2]);
		glVertex3f(nbl[0],nbl[1],nbl[2]);
		glVertex3f(ntl[0],ntl[1],ntl[2]);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(ftl[0],ftl[1],ftl[2]);
		glVertex3f(ntl[0],ntl[1],ntl[2]);
		glVertex3f(ftr[0],ftr[1],ftr[2]);
		glVertex3f(ntr[0],ntr[1],ntr[2]);
		glVertex3f(fbr[0],fbr[1],fbr[2]);
		glVertex3f(nbr[0],nbr[1],nbr[2]);
		glVertex3f(fbl[0],fbl[1],fbl[2]);
		glVertex3f(nbl[0],nbl[1],nbl[2]);
	glEnd();
}

void draw_view()
{
	int i;
	glMatrixMode(GL_MODELVIEW);

	//Draw eye position
	glPushMatrix();
	glTranslatef(eye[0],eye[1],eye[2]);
	glColor3f(0.4,0.1,0.4);
	glutWireSphere(1.0,15,15);
	glPopMatrix();

	//printf("u[0]:%f %f %f,u[1]:%f %f %f,u[2]:%f %f %f\n",u[0][0],u[0][1],u[0][2],u[1][0],u[1][1],u[1][2],u[2][0],u[2][1],u[2][2]);

	draw_view_volumne();

	//draw 3 axes
	//Xe
	glColor3f(0.6,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(eye[0]+15.0*u[0][0],eye[1]+15.0*u[0][1],eye[2]+15.0*u[0][2]);
	glEnd();

	//Ye
	glColor3f(0.7,1.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(eye[0]+15.0*u[1][0],eye[1]+15.0*u[1][1],eye[2]+15.0*u[1][2]);
	glEnd();
	
	//Ze
	glColor3f(0.8,0.0,1.0);
	glBegin(GL_LINES);
		glVertex3f(eye[0],eye[1],eye[2]);
		glVertex3f(eye[0]+15.0*u[2][0],eye[1]+15.0*u[2][1],eye[2]+15.0*u[2][2]);
	glEnd();
}

void draw_state(){
	make_view(state);
	make_projection(state);
	glViewport(0,0,width,height);
	draw_scene();
	make_view(state);
	draw_view();
}

//display callback procedure to draw the scene
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	switch(state){
	case 0:
		 make_view(4);
		 make_projection(4);
		 glViewport(width/2, 0, width/2, height/2);
		 draw_scene();

		 make_view(1);
		 make_projection(1);
		 glViewport(0, height/2, width/2, height/2);
		 draw_scene();
		 make_view(1);
		 draw_view();

		 make_view(2);
		 glViewport(width/2, height/2, width/2, height/2);
		 draw_scene();
		 make_view(2);
		 draw_view();

		 make_view(3);
		 glViewport(0, 0, width/2, height/2);
		 draw_scene();
		 make_view(3);
		 draw_view();
		 break;
	case 1:
		draw_state();
		break;
	case 2:
		draw_state();
		break;
	case 3:
		draw_state();
		break;
	case 4:
		draw_state();
		break;

	}
	
	glutSwapBuffers();
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
  float tmpx[3],tmpy[3],tmpz[3];
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
  }else if(key=='C'){
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
	  if(position[1]!=-15){   //if do not touch the floor
		  goD=1;
		  position[1]-=0.25;
	  }
	  else{
		  landing = 1;
	  }
  }    
  else if(key=='9'){                 //speed up
	  if(ang_fac < 5.0){
		  ang_fac+=0.5;
	  }
  }  
  else if(key=='8'){                 //speed down
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
  else if(key=='0'){
	  state = 0;
  }
  else if(key=='1'){
	  state = 1;
  }
  else if(key=='2'){
	  state = 2;
  }
  else if(key=='3'){
	  state = 3;
  }
  else if(key=='4'){
	  state = 4;
  }

  /*------transform the EYE coordinate system ------*/
  else if(key=='O'){
    eyeDy += 0.5;       /* eye move up */
    for(int i=0;i<3;i++) eye[i] -= 0.5*u[1][i];
  }else if(key=='o'){
    eyeDy += -0.5;       /* eye move down */
    for(int i=0;i<3;i++) eye[i] += 0.5*u[1][i];
  }else if(key=='l'){
    eyeDx += -0.5;       /* move left */
    for(int i=0;i<3;i++) eye[i] += 0.5*u[0][i];
  }else if(key=='L'){
    eyeDx += 0.5;        /* move right */
    for(int i=0;i<3;i++) eye[i] -= 0.5*u[0][i];
  }
  else if(key=='z'){
	eyeDz += 0.5;        /* zoom in */
	for(int i=0;i<3;i++) eye[i] -= 0.5*u[2][i];
  }
  else if(key=='Z'){
		eyeDz += -0.5;       /* zoom out */
		for(int i=0;i<3;i++) eye[i] += 0.5*u[2][i];
  }
  else if(key=='p'){             /* pitching */
		eyeAngx += 5.0;
		if(eyeAngx > 360.0) eyeAngx -= 360.0;
		tmpy[0] = u[1][0]*cv - u[2][0]*sv;
		tmpy[1] = u[1][1]*cv - u[2][1]*sv;
		tmpy[2] = u[1][2]*cv - u[2][2]*sv;

		tmpz[0] = u[2][0]*cv + u[1][0]*sv;
		tmpz[1] = u[2][1]*cv + u[1][1]*sv;
		tmpz[2] = u[2][2]*cv + u[1][2]*sv;

		for(int i=0;i<3;i++){
		  u[1][i] = tmpy[i];
		  u[2][i] = tmpz[i];
		}
  }
  else if(key=='P'){          
		eyeAngx += -5.0;
		if(eyeAngx<0.0) eyeAngx += 360.0;
		tmpy[0] = u[1][0]*cv + u[2][0]*sv;
		tmpy[1] = u[1][1]*cv + u[2][1]*sv;
		tmpy[2] = u[1][2]*cv + u[2][2]*sv;

		tmpz[0] = u[2][0]*cv - u[1][0]*sv;
		tmpz[1] = u[2][1]*cv - u[1][1]*sv;
		tmpz[2] = u[2][2]*cv - u[1][2]*sv;

		for(int i=0;i<3;i++){
		  u[1][i] = tmpy[i];
		  u[2][i] = tmpz[i];
		}
  }
  else if(key=='h'){            /* heading */
		eyeAngy += 5.0;
		if(eyeAngy>360.0) eyeAngy -= 360.0;
		for(int i=0;i<3;i++){
		  tmpx[i] = cv*u[0][i] - sv*u[2][i];
		  tmpz[i] = sv*u[0][i] + cv*u[2][i];
		}
		for(int i=0;i<3;i++){
		  u[0][i] = tmpx[i];
		  u[2][i] = tmpz[i];
		}
  }
  else if(key=='H'){
    eyeAngy += -5.0;
    if(eyeAngy<0.0) eyeAngy += 360.0;
    for(int i=0;i<3;i++){
      tmpx[i] = cv*u[0][i] + sv*u[2][i];
      tmpz[i] = -sv*u[0][i] + cv*u[2][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = tmpx[i];
      u[2][i] = tmpz[i];
    }
  }
  else if(key=='r'){            /* rolling */
    eyeAngz += 5.0;
    if(eyeAngz>360.0) eyeAngz -= 360.0;
    for(int i=0;i<3;i++){
      tmpx[i] = cv*u[0][i] - sv*u[1][i];
      tmpy[i] = sv*u[0][i] + cv*u[1][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = tmpx[i];
      u[1][i] = tmpy[i];
    }
  }
  else if(key=='R'){
    eyeAngz += -5.0;
    if(eyeAngz<0.0) eyeAngz += 360.0;
    for(int i=0;i<3;i++){
      tmpx[i] = cv*u[0][i] + sv*u[1][i];
      tmpy[i] = -sv*u[0][i] + cv*u[1][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = tmpx[i];
      u[1][i] = tmpy[i];
	}
  }
  else if(key=='>'){
	  if(ang_fovy<175.0){
		  ang_fovy+=5.0;
	  }
  }
  else if(key=='<'){
	  if(ang_fovy>50.0){
		  ang_fovy-=5.0;
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
  glutCreateWindow("ROV_HW3");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);
  glutIdleFunc(display); 
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_quit);

  glutMainLoop();
}

