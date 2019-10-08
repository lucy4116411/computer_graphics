/******************************************************************
 * This program illustrates the fundamental instructions for handling 
 * mouse and keyboeard events as well as menu buttons.
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glut.h>
#include <WinGDI.h>/  //Save Image as BMP

#define    SIZEX   800
#define    SIZEY   800

#define    MY_QUIT -1
#define    MY_CLEAR -2
#define    MY_SAVE  -3
#define    MY_BLEND -4
#define    MY_LOAD  -5
#define    MY_SAVEIMAGE -6

#define    WHITE   1
#define    RED     2
#define    GREEN   3
#define    BLUE    4
#define    SELF_DEFINED 5

#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    CURVE   5
#define    TEXT    6
#define    ERASER   7
#define    TRIANGLE 8
#define    RECTANGLE 9

#define    FILL 0
#define    UNFILL 1

#define    GRID_LINE 1
#define    RULER  2
#define    NONE 0

typedef    int   menu_t;
menu_t     top_m, color_m, file_m, type_m,fill_m,show_m;

static long long int timer=0;

int        height=512, width=512;
unsigned char  image[SIZEX*SIZEY][4];  /* Image data in main memory */

int        pos_x=-1, pos_y=-1;
float      myColor[3]={0.0,0.0,0.0};
float      eraserColor[3]={0.0,0.0,0.0};
int        obj_type = -1;
int        first=0;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side=0;         /*num of sides of polygon */
float      pnt_size=1.0;
int		   fillMode = 0; // 0:fill 1:empty
int		   showMode=0;
/*------------------------------------------------------------
 * Callback function for display, redisplay, expose events
 * Just clear the window again
 */
void display_func(void)
{
  /* define window background color */
	//glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h)
{
  height = new_h;
  width = new_w;

  printf("%d %d\n",height,width);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width,height);
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}


/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
  if(key=='Q'||key=='q') exit(0);
}


/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon()
{
  int  i;

  if(fillMode==0){
	 glPolygonMode(GL_FRONT, GL_FILL);
  }
  else{
	  glPolygonMode(GL_FRONT, GL_LINE);
	  /*glBegin(GL_LINES);
	  for(i=0;i<=side;i++){
		glVertex2f(vertex[i][0], height-vertex[i][1]);
	  }
	  glEnd();*/
  }   
    glBegin(GL_POLYGON);
	 for(i=0;i<side;i++)
	   glVertex2f(vertex[i][0], height-vertex[i][1]);
    glEnd();
    glFinish();
	side = 0;
  /* set side=0 for next polygon */
}



/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle()
{
  static GLUquadricObj *mycircle=NULL;

  if(mycircle==NULL){
    mycircle = gluNewQuadric();
    gluQuadricDrawStyle(mycircle,GLU_FILL);
  }
  glPushMatrix();
  glTranslatef(pos_x, height-pos_y, 0.0);

  if(fillMode==0){
	  gluDisk(mycircle,
	       0.0,           /* inner radius=0.0 */
		  9.0+pnt_size,          /* outer radius=9.0+pnt_size */
		  16,            /* 16-side polygon */
		  3);
  }
  else{
	  gluDisk(mycircle,8.0+pnt_size,9.0+pnt_size,16,3);
  }
  glPopMatrix();
}

// Put on a text  
void draw_text(){
	char text_string[200] ;
	char *c,*tmp=NULL;
	printf("Please input the Text:");
	fgets(text_string,sizeof(text_string),stdin);
	tmp=strchr(text_string,'\n');    // fgets會讀入換行造成bug 所以要去掉換行符號
	if(tmp){
		*tmp='\0';
	}
	glRasterPos2f(pos_x,pos_y);
	for(c=&text_string[0];*c!='\0';c++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*c);
	}
}

void draw_triangle(){
	if(fillMode==0){
		glBegin(GL_TRIANGLES);
			glVertex2f(vertex[0][0], height-vertex[0][1]);
			glVertex2f(vertex[1][0], height-vertex[1][1]);
			glVertex2f(vertex[2][0], height-vertex[2][1]);
		glEnd();
	}
	else{
		glBegin(GL_LINES);
			//glLineWidth(pnt_size);
			glVertex2f(vertex[1][0],height-vertex[side-1][1]);
			glVertex2f(vertex[2][0],height-vertex[2][1]);
			glVertex2f(vertex[2][0],height-vertex[2][1]);
			glVertex2f(vertex[0][0],height-vertex[0][1]);
		glEnd();
	}
	side=0;
	glFlush();
}

void draw_rectangle(){
	if(fillMode==0){
		glBegin(GL_POLYGON);
		glVertex2f(vertex[0][0],height-vertex[0][1]);
		glVertex2f(vertex[2][0],height-vertex[0][1]);
		glVertex2f(vertex[2][0],height-vertex[2][1]);
		glVertex2f(vertex[0][0],height-vertex[2][1]);
		glEnd();
	}
	else{
		glBegin(GL_LINES);

		glVertex2f(vertex[0][0],height-vertex[0][1]);
		glVertex2f(vertex[2][0],height-vertex[0][1]);

		glVertex2f(vertex[2][0],height-vertex[0][1]);
		glVertex2f(vertex[2][0],height-vertex[2][1]);

		glVertex2f(vertex[2][0],height-vertex[2][1]);
		glVertex2f(vertex[0][0],height-vertex[2][1]);

		glVertex2f(vertex[0][0],height-vertex[2][1]);
		glVertex2f(vertex[0][0],height-vertex[0][1]);
		glEnd();
	}
}



/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y)
{
  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;

  switch(obj_type){
  case POINT:
    glPointSize(pnt_size);     /*  Define point size */
    glBegin(GL_POINTS);     /*  Draw a point */
       glVertex2f(x, height-y);
    glEnd();
    break;
  case LINE:
    if(first==0){
      first = 1;
      pos_x = x; pos_y = y;
	  glPointSize(pnt_size);  //set Point size 
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	    glVertex3f(x, height-y, 0);
      glEnd();
    }else{
      first=0;
      glLineWidth(pnt_size);     /* Define line width */
      glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(pos_x, height - pos_y);
	    glVertex2f(x, height - y);
      glEnd();
    }
    break;
  case POLYGON:  /* Define vertices of poly */
    if(side==0){
      vertex[side][0] = x; vertex[side][1] = y;
      side ++;
    }else{
      if(abs(vertex[side-1][0]-x) + abs(vertex[side-1][1]-y)<2){   //當前一條線的尾巴重複點擊
	     printf("drawPolyGon\n");
		 vertex[side][0]=vertex[0][0];
		 vertex[side][1]=vertex[0][1];
		 draw_polygon();
	  }
      else{
		//glPointSize(pnt_size);
		glBegin(GL_POINTS);
			glVertex2f(vertex[side-1][0], height-vertex[side-1][1]);
		glEnd();
		//glLineWidth(pnt_size);  
	    glBegin(GL_LINES);
          glVertex2f(vertex[side-1][0], height-vertex[side-1][1]);
	      glVertex2f(x, height-y);
	    glEnd();
	    vertex[side][0] = x; 
		vertex[side][1] = y;
		side++;
	  }
    }
    break;
  case CIRCLE:
    pos_x = x; pos_y = y;
    draw_circle();
    break;
  case TEXT:
	pos_x=x;
	pos_y=height-y;
	draw_text();
	glFlush();
	break;
  case TRIANGLE:
	  if(side==0){
		  vertex[side][0]=x;
		  vertex[side][1]=y;
		  glBegin(GL_POINT);
			//glPointSize(pnt_size);
			glVertex2f(x,height-y);
		  glEnd();
		  side++;
	  }
	  else if(side==1){
		  vertex[side][0]=x;
		  vertex[side][1]=y;
		  glBegin(GL_LINES);
		    //glLineWidth(pnt_size);
			glVertex2f(vertex[side-1][0],height-vertex[side-1][1]);
			glVertex2f(x,height-y);
		  glEnd();
		  side++;
	  }
	  else{
		  vertex[side][0]=x;
		  vertex[side][1]=y;
		  printf("drawTriangle\n");
		  draw_triangle();
		  glFlush();
	  }
	  break;
  case RECTANGLE:
	  if(first==0){
		  vertex[0][0]=x;
		  vertex[0][1]=y;
		  glBegin(GL_POINTS);
			glVertex2f(x,height-y);
		  glEnd();
		  first=1;
	  }
	  else{
		  first=0;
		  vertex[2][0]=x;
		  vertex[2][1]=y;
		  draw_rectangle();
		  glFlush();
	  }
	  break;
  default:
    break;
  }
  glColor3f(myColor[0],myColor[1],myColor[2]);
  glFinish();
}


    
/*-------------------------------------------------------------
 * motion callback function. The mouse is pressed and moved.
 */
void motion_func(int  x, int y)
{
  if(obj_type==CURVE){
	if(first==0){
	    first = 1;
	    pos_x = x; pos_y = y;
	  }else{
	    glBegin(GL_LINES);
	      glVertex3f(pos_x, height-pos_y, 0.0);      //begin point
	      glVertex3f(x, height - y, 0.0);            //end point
	    glEnd();
	    pos_x =	x; pos_y = y;
	}
	  glFinish();
  }
  if(obj_type==ERASER){
	  glPointSize(pnt_size+1.0);
	  glBegin(GL_POINTS);
	    glColor3f(0.0,0.0,0.0);
		glVertex2f(x,height-y);
	  glEnd();
	  glFlush();
  }
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void)
{
  /*Do nothing else but clear window to black*/

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}


/*------------------------------------------------------
 * Procedure to initialize data alighment and other stuff
 */
void init_func()
{   glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*-----------------------------------------------------------------
 * Callback function for color menu
 */
void  color_func(int value)
{
  switch(value){
  case WHITE:
    myColor[0] = myColor[1] = myColor[2] = 1.0;
    break;

  case RED:
    myColor[0] = 1.0;
    myColor[1] = myColor[2] = 0.0;
    break;

  case GREEN:
    myColor[0] = myColor[2] = 0.0;
    myColor[1] = 1.0;
    break;
    
  case BLUE:
    myColor[0] = myColor[1] = 0.0;
    myColor[2] = 1.0;
    break;
  case SELF_DEFINED:
	  float sr,sg,sb;
	  printf("input R,G,B  (0.0<= R,G,B <= 1.0):");
	  scanf("%f %f %f",&sr,&sg,&sb);
	  while(sr>1.0 || sr<0.0 || sg>1.0 || sg<0.0 || sb>1.0 || sb<0.0){
		printf("input R,G,B  (0.0<= R,G,B <= 1.0):");
		scanf("%f %f %f",&sr,&sg,&sb);
	  }
	  myColor[0]=sr;
	  myColor[1]=sg;
	  myColor[2]=sb;
	  break;
  default:
    break;
  }
  glColor3f(myColor[0], myColor[1], myColor[2]);
}

void saveImageAsBMP()   
{
	FILE* pDummyFile;  //指向另一BMP文件,用於複製他的header數據
	FILE* pWritingFile; //指向要保存截圖的bmp文件
	GLubyte* pPixelData;
	GLubyte BMP_HEADER[54];  //BMP_HEADER_LENGTH = 54
	GLint i,j;
	GLint PixelDataLength; //BMP文件數據總長度

	i=width*3;                  //每一行像素數據長度
	while(i%4!=0){
		++i;                    //補充數據直到是4的倍數
	}
	PixelDataLength=i*width;

	printf("save\n");
	pPixelData=(GLubyte*)malloc(PixelDataLength);
	if(pPixelData==0) { printf("pPixelData fails\n"); return;}

	pDummyFile=fopen("bitmap00.bmp","rb");
	if(pDummyFile==0){ printf("pDummy File fails\n"); return;}

	pWritingFile=fopen("bitmap01.bmp","wb");
	if(pWritingFile==0) { printf("pWriting File fails\n"); return;}

 
	fread(BMP_HEADER, sizeof(BMP_HEADER), 1, pDummyFile);  
	fwrite(BMP_HEADER, sizeof(BMP_HEADER), 1, pWritingFile);    
	fseek(pWritingFile, 0x0012, SEEK_SET); 
	i = width;    
	j = height;    
	fwrite(&i, sizeof(i), 1, pWritingFile);    
	fwrite(&j, sizeof(j), 1, pWritingFile);     
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  
	glReadPixels(0, 0, width, height,GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);  
	fseek(pWritingFile, 1, SEEK_END);	 
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);      
	 
	fclose(pDummyFile);    
	fclose(pWritingFile);    
	free(pPixelData);
}


/*------------------------------------------------------------
 * Callback function for top menu.
 */
void file_func(int value)
{ 
  int i, j;
  char fileName[20];
  char* find=NULL;

  if(value==MY_QUIT) exit(0);
  else if(value==MY_CLEAR) init_window();
  else if(value==MY_SAVE){ /* Save current window */
	timer=0;
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
	for(i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  for(j=0;j<height;j++)
		if(image[i*width+j][0]==0 &&
           image[i*width+j][1]==0 &&
           image[i*width+j][2]==0) image[i*width+j][3] = 0;
		else image[i*width+j][3] = 127; /* Other pixels have A=127*/
  }else if(value==MY_LOAD){ /* Restore the saved image */
	 glRasterPos2i(0, 0);
     glDrawPixels(width, height, 
	       GL_RGBA, GL_UNSIGNED_BYTE, 
	       image);
  }else if(value==MY_BLEND){ /* Blending current image with the saved image */	
	  glEnable(GL_BLEND);   // 開啟Blend
	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glRasterPos2i(0, 0);
      glDrawPixels(width, height, 
	       GL_RGBA, GL_UNSIGNED_BYTE, 
	       image);
	  glDisable(GL_BLEND);  // 關閉Blend
  }else if(value==MY_SAVEIMAGE){
	  /*printf("Please input the file name to save:");
	  fgets(fileName,20,stdin);
	  find = strchr(fileName,'\n');
	  if(find){
		  *find='\0';
	  }*/
	  saveImageAsBMP();
  }

  glFlush(); //update
}

void size_func(int value)
{
	if(value==1){
	  if(pnt_size<10.0) pnt_size += 1.0;
	}else if(value==2){
	  if(pnt_size>1.0) pnt_size = pnt_size -1.0;
	}
	else{
		printf("point size (<=50) :");
		scanf("%f",&pnt_size);
		while(pnt_size >50){
			printf("point size again (<=50) :");
			scanf("%f",&pnt_size);
		}
	}
	printf("Size Now: %.2f\n",pnt_size);
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
}


/*-------------------------------------------------------------
 * Callback Func for type_m, define drawing object
 */
void draw_type(int value)
{
  obj_type = value;
  if(value==LINE||value==CURVE||value==RECTANGLE)
    first = 0;
  else if(value==POLYGON || value==TRIANGLE) side = 0;
  /*else{
	  first=0;
	  side=0;
  }*/
}

void fill_mode(int value)
{
	if(value==FILL){
		fillMode=FILL;
	}
	else{
		fillMode = UNFILL;
	}
}

void show_mode(int value)
{
	if(value==GRID_LINE){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		for(int i=0;i<width;i+=10){
			//glClearColor(0.0,0.0,0.0,0.0);
			glColor4f(0.8,0.8,0.5,0.5);
			glBegin(GL_LINES);
			glVertex2f(i,height);
			glVertex2f(i,0);
			glEnd();
		}
		for(int i=0;i<height;i+=10){
			//glClearColor(1.0,1.0,1.0,1.0);
			glColor4f(0.8,0.8,0.5,0.5);
			glBegin(GL_LINES);
			glVertex2f(width,i);
			glVertex2f(0,i);
			glEnd();
		}
		glDisable(GL_BLEND);
	}
}

/****** Self *****/
void idle_func(){
	/*if(timer!=4100000){
		timer++;
		printf("timer:%d\n",timer);
		return;
	}
	else{
		printf("SAVE:%d\n",timer);
		int i,j;
		glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,image);
		for(i=0;i<width;i++)   // Assign 0 opacity to black pixels 
			for(j=0;j<height;j++)
			if(image[i*width+j][0]==0 &&
			   image[i*width+j][1]==0 &&
			   image[i*width+j][2]==0) image[i*width+j][3] = 0;
			else image[i*width+j][3] = 127; // Other pixels have A=127
		timer=0;
		return;
	}*/
}
/*****  Self  *****/

/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */
void main(int argc, char **argv)
{
  int  size_menu;

  glutInit(&argc, argv);    /*---Make connection with server---*/

  glutInitWindowPosition(0,0);  /*---Specify window position ---*/
  glutInitWindowSize(width, height); /*--Define window's height and width--*/

  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA); /*---set display mode---*/
  init_func();

   /* Create parent window */
  glutCreateWindow("Menu"); 

  glutDisplayFunc(display_func); /* Associate display event callback func */
  glutReshapeFunc(my_reshape);  /* Associate reshape event callback func */

  glutKeyboardFunc(keyboard); /* Callback func for keyboard event */

  glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
  glutMotionFunc(motion_func);/* Mouse motion event callback func */

  /******* Self ******/
  glutIdleFunc(idle_func);
  /******* Self End *****/

  color_m = glutCreateMenu(color_func); /* Create color-menu */
  glutAddMenuEntry("white", WHITE);
  glutAddMenuEntry("red", RED);
  glutAddMenuEntry("green", GREEN);
  glutAddMenuEntry("blue", BLUE);
  glutAddMenuEntry("self defined",SELF_DEFINED);

  file_m = glutCreateMenu(file_func);   /* Create another menu, file-menu */
  glutAddMenuEntry("save", MY_SAVE);
  glutAddMenuEntry("load", MY_LOAD);
  glutAddMenuEntry("blend", MY_BLEND);
  glutAddMenuEntry("clear", MY_CLEAR);
  glutAddMenuEntry("quit", MY_QUIT);
  //glutAddMenuEntry("save image",MY_SAVEIMAGE);

  type_m = glutCreateMenu(draw_type);   /* Create draw-type menu */
  glutAddMenuEntry("Point", POINT);
  glutAddMenuEntry("Line", LINE);
  glutAddMenuEntry("Poly", POLYGON);
  glutAddMenuEntry("Curve", CURVE);
  glutAddMenuEntry("Circle", CIRCLE);
  glutAddMenuEntry("Triangle",TRIANGLE);
  glutAddMenuEntry("Rectangle",RECTANGLE);
  glutAddMenuEntry("Text",TEXT);
  glutAddMenuEntry("Eraser",ERASER);

  size_menu = glutCreateMenu(size_func);
  glutAddMenuEntry("Bigger", 1);
  glutAddMenuEntry("Smaller",2);
  glutAddMenuEntry("self defined",SELF_DEFINED);

  fill_m= glutCreateMenu(fill_mode);
  glutAddMenuEntry("Filled",FILL);
  glutAddMenuEntry("Unfilled",UNFILL);

  show_m= glutCreateMenu(show_mode);
  glutAddMenuEntry("grid-line",GRID_LINE);
  //glutAddMenuEntry("ruler",RULER);
  //glutAddMenuEntry("none",NONE);

  top_m = glutCreateMenu(top_menu_func);/* Create top menu */
  glutAddSubMenu("colors", color_m);    /* add color-menu as a sub-menu */
  glutAddSubMenu("type", type_m);
  glutAddSubMenu("Size", size_menu);
  glutAddSubMenu("file", file_m);       /* add file-menu as a sub-menu */
  glutAddSubMenu("fill mode",fill_m);
  glutAddSubMenu("show mode",show_m);
  glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/



  /*---Test whether overlay support is available --*/
  if(glutLayerGet(GLUT_OVERLAY_POSSIBLE)){
    fprintf(stderr,"Overlay is available\n");
  }else{
    fprintf(stderr,"Overlay is NOT available, May encounter problems for menu\n");
  }
  /*---Enter the event loop ----*/
  glutMainLoop();       
}
