#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <cstring>
#define VEL 0.04
#define initiallife 12
#define PI 3.14159265
#define Pi 3.14159265
using namespace std;


float ww=1368,wh=768;
GLdouble width=1368,height=768,ycord_first=0.05,ycord_second=0.45,ycord_third=0.85,xcord_first=0.49,xcord_second=0.23,xcord_third=0.79,deltax_first=0.005,deltax_second=0.003,deltax_third=0.004;
GLdouble eggcentrex=xcord_first+0.03,eggcentrey=ycord_first+0.045,egginbasketnumber=1,velocity=VEL,g=0.0018,eggcentrexnext,eggcentreynext,delta=0.025,cross_count;
GLdouble ellipsecordx=xcord_first+0.03,ellipsecordy=0.08+0.0045;
bool flag=false,slideFlag=false,gameoverflag=true;
int basketBelow=1,basketAbove=3,life=initiallife,points=0;

void Initialize();
void reshape(int w,int h);
void Draw();
void DrawBucket(GLdouble xcord,GLdouble ycord);
void ellipse(float x, float y, float a, float b);
void mouse(int button,int state,int x,int y);
void mouse1(int button,int state,int x,int y);
void launch();
void Idle();
void Background();
void decrement();
void addToBasket(int);
void changeSpeed();
void DrawNum(int num,GLdouble cordx,GLdouble cordy);
void DrawDigit(int num,GLdouble cordx,GLdouble cordy);
void GameOver();
void display();
void printb(char c[], int x, int y);
//background
struct star {
    float x, y;
    float vx, vy;
};
struct star stars[150];

//Printb
void printb(char c[], int x, int y)
{
    glRasterPos2i(x, y);
    //glColor3f(0.0, 0.0, 0.0);
    for(int i=0;i<strlen(c);i++)
    	{ 
  		  	glutBitmapCharacter(GLUT_BITMAP_9_BY_15,c[i]);
  		  
    	}
    glFlush();
}

void Background(void)
{
    int i;     
    for (i = 0; i < 100; i++) 
    {
		stars[i].x += stars[i].vx;
		if (stars[i].x < glutGet(GLUT_WINDOW_WIDTH)) 
		{
	    	glBegin(GL_LINE_STRIP);
	    	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	    	glMaterialfv(GL_FRONT, GL_AMBIENT, qaWhite);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
			glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
			glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	    	glColor3f(0, 0, 0);
	    	glVertex3f((stars[i].x-stars[i].vx*4)/width, (stars[i].y)/height, -0.5);
	    	glColor3f(1.0, 1.0, 1.0);
	    	glVertex3f((stars[i].x)/width, (stars[i].y)/height, -0.5);
	    	glEnd();
        } 
		else
	    stars[i].x = 0;
    }
}

//Intialize
void reshape(int w,int h)	
{
	width=(GLdouble)w;
	height=(GLdouble)h;
	glViewport(0,0, (GLsizei)w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    //glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0,1.0,1.0);

}
void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	// Set lighting intensity and color
	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);
	glLightfv(GL_LIGHT2, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT2, GL_SPECULAR, qaSpecularLight);
	// Set the light position
	GLfloat qaLightPosition1[]	= {xcord_first, 0.02, 0.15, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition1);
	GLfloat qaLightPosition2[]	= {xcord_second, 0.5, 0.15, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition2);
	GLfloat qaLightPosition3[]	= {xcord_third, 0.92, 0.15, 1.0};
	glLightfv(GL_LIGHT2, GL_POSITION, qaLightPosition3);
}
void Idle()
{
	glutPostRedisplay();
}

//Draw
void Draw() 
{	

    
    if(gameoverflag==true)
      GameOver();
  else
  { 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);   
    Background();
	
	if(xcord_first+0.06>=1.0 || xcord_first<=0.0)
		deltax_first=-deltax_first;
	xcord_first+=deltax_first;
	if(xcord_second+0.06>=1.0 || xcord_second<=0.0)
		deltax_second=-deltax_second;
	xcord_second+=deltax_second;
	if(xcord_third+0.06>=1.0 || xcord_third<=0.0)
		deltax_third=-deltax_third;
	xcord_third+=deltax_third;
	
	if(slideFlag==true)
	  decrement();
	
  DrawBucket(xcord_first,ycord_first);
	DrawBucket(xcord_second,ycord_second);
	DrawBucket(xcord_third,ycord_third);
	
	if(egginbasketnumber==1)
	{
		eggcentrex=xcord_first+0.03;
		if(flag==true)
		{
         eggcentrey+=velocity;
         velocity-=g;
        }
	}
	else if(egginbasketnumber==2) 
	{
		eggcentrex=xcord_second+0.03;
		if(flag==true)
		{
         eggcentrey+=velocity;
         velocity-=g;
        }
	}
	else
	{
		eggcentrex=xcord_third+0.03;
 	    if(flag==true)
        {
         eggcentrey+=velocity;
         velocity-=g;
        }
	}               
                     	
	if(flag==true)
	{
    if(velocity<0 && eggcentrex>=(xcord_first+0.01) && eggcentrex<=(xcord_first+0.05) && eggcentrey>=(ycord_first+0.06) && eggcentrey<=(ycord_first+0.08))
    {
       if(egginbasketnumber==1)
       life--;
       else
       points++;           
       egginbasketnumber=1;
       eggcentrex=xcord_first+0.03;
       eggcentrey=ycord_first+0.045;
       velocity=VEL;
       flag=false;
    }
    if(velocity<0 && eggcentrex>=(xcord_second +0.01)&& eggcentrex<=(xcord_second+0.05) && eggcentrey>=(ycord_second+0.06) && eggcentrey<=(ycord_second+0.08))
    {
        if(egginbasketnumber==2)
       life--;          
       else
       points++;
       egginbasketnumber=2;
       eggcentrex=xcord_second+0.03;
       eggcentrey=ycord_second+0.045;
       velocity=VEL;
       flag=false;
    }
    if(velocity<0 && eggcentrex>=(xcord_third+0.01) && eggcentrex<=(xcord_third+0.05) && eggcentrey>=(ycord_third+0.06) && eggcentrey<=(ycord_third+0.08))
    {
       if(egginbasketnumber==3)
       life--;
       else
       points++;
       egginbasketnumber=3;
       eggcentrex=xcord_third+0.03;
       eggcentrey=ycord_third+0.045;
       velocity=VEL;
       flag=false;
    }
    if(eggcentrey<0.05)
    {

      addToBasket(basketBelow);
      velocity=VEL;
      flag=false;
    }
    }
       
    ellipse(eggcentrex, eggcentrey, 0.015, 0.025);
    DrawNum(life,0.025,0.95);
    DrawNum(points,0.87,0.95);
    if(life==0)
    { 
      printf("Game Over and u have scored %d points\n",points);
      gameoverflag=true;
    }
    if(egginbasketnumber==basketAbove)
      slideFlag=true;
    glutSwapBuffers();
	glutPostRedisplay();
  }
}

void DrawBucket(GLdouble xcord,GLdouble ycord)
{
	glColor3f(102.0/255.0, 51.0/255.0, 0.0);
	// Set material properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaBrown[] = {102.0/255.0, 51.0/255.0, 0.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaBrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBrown);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_POLYGON);
		glVertex3f(xcord, ycord+0.03, 0.0);
		glVertex3f(xcord+0.02, ycord, 0.0);
		glVertex3f(xcord+0.04, ycord, 0.0);
		glVertex3f(xcord+0.06, ycord+0.03, 0.0);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(xcord-0.005,ycord+0.008+0.03,0.0);
		glVertex3f(xcord-0.005,ycord+0.03,0.0);
		glVertex3f(xcord+0.06+0.005,ycord+0.03,0.0);
		glVertex3f(xcord+0.06+0.005,ycord+0.008+0.03,0.0);
	glEnd();
}

void ellipse(float x, float y, float a, float b) {
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glPointSize(1.0);
	glBegin(GL_TRIANGLE_FAN);
	glClearColor(0.0, 0.0,0.0,0.0);
	// glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); 
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaEGG[] = {240.0/255.0, 234.0/255.0, 214.0/255.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaBrown[] = {102.0/255.0, 51.0/255.0, 0.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaEGG);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaEGG);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glVertex3f(x, y, -0.2);
	for(int i = 0; i <= 360; ++i) {
		glVertex3f((x + a * cos(i*Pi/180)), (y + b * sin(i*Pi/180)), -0.2);
		//printf("%f %f\n", x + a * cos(i*Pi/180.0), y + b * sin(i*Pi/180.0));
	}
	glEnd();
	
}

//GameOver
void GameOver()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);
	glColor3f(1,1,0);
	glLineWidth(3.0);
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaYellow[] = {1.0,1.0,0.0,1.0};
	GLfloat qaRed[]={1.0,0.0,0.0,1.0};

	glBegin(GL_QUADS);
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaYellow);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		glVertex3f(0.2,0.35,0.0);
		glVertex3f(0.2,0.75,0.0);
		glVertex3f(0.72,0.75,0.0);
		glVertex3f(0.72,0.35,0.0);
	glEnd();

	glBegin(GL_QUADS);
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaYellow);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		glVertex3f(0.2,0.1,0.0);
		glVertex3f(0.2,0.3,0.0);
		glVertex3f(0.72,0.3,0.0);
		glVertex3f(0.72,0.1,0.0);
	glEnd();
		glColor3f(1,0,0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.25,0.5,0.0);
		glVertex3f(0.25,0.65,0.0);
		glVertex3f(0.35,0.65,0.0);
		glVertex3f(0.35,0.6,0.0);
		glVertex3f(0.25,0.6,0.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(0.45,0.5,0.0);
		glVertex3f(0.36,0.5,0.0);
		glVertex3f(0.36,0.5,0.0);
		glVertex3f(0.36,0.65,0.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.47,0.6,0.0);
		glVertex3f(0.47,0.65,0.0);
		glVertex3f(0.56,0.65,0.0);
		glVertex3f(0.56,0.6,0.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(0.47,0.6,0.0);
		glVertex3f(0.47,0.5,0.0);
		glVertex3f(0.56,0.6,0.0);
		glVertex3f(0.56,0.5,0.0);
		glVertex3f(0.60,0.65,0.0);
		glVertex3f(0.65,0.6,0.0);
		glVertex3f(0.65,0.6,0.0);
		glVertex3f(0.70,0.65,0.0);
		glVertex3f(0.65,0.6,0.0);
		glVertex3f(0.65,0.5,0.0);
		glVertex3f(0.25,0.12,0.0);
		glVertex3f(0.25,0.28,0.0);
		glVertex3f(0.25,0.12,0.0);
		glVertex3f(0.35,0.12,0.0);
		glVertex3f(0.25,0.2,0.0);
		glVertex3f(0.35,0.2,0.0);
		glVertex3f(0.25,0.28,0.0);
		glVertex3f(0.35,0.28,0.0);
		glVertex3f(0.36,0.28,0.0);
		glVertex3f(0.46,0.12,0.0);
		glVertex3f(0.36,0.12,0.0);
		glVertex3f(0.46,0.28,0.0);
		glVertex3f(0.47,0.12,0);
		glVertex3f(0.57,0.12,0);
		glVertex3f(0.47,0.28,0);
		glVertex3f(0.57,0.28,0);
		glVertex3f(0.52,0.12,0);
		glVertex3f(0.52,0.28,0);
		glVertex3f(0.58,0.28,0.0);
		glVertex3f(0.68,0.28,0.0);
		glVertex3f(0.63,0.12,0.0);
		glVertex3f(0.63,0.28,0.0);
	glEnd();
	glutSwapBuffers();
	glutPostRedisplay();
}

//Launch
void launch()
{
	ellipsecordy+=velocity;
	velocity-=g;
	
	if(ellipsecordy>=eggcentreynext)
		cross_count=1;
	if(ellipsecordy<=eggcentreynext && cross_count==1)
		cross_count=2;

	if((abs(ellipsecordx - eggcentrexnext) <= delta) && (abs(ellipsecordy -eggcentreynext)<=delta) && cross_count==2)
	{
		cross_count=0;
		flag=false;
		velocity=0.085;
		egginbasketnumber=((int)(egginbasketnumber)%3)+1;
	}
	if(ellipsecordy<=0.001)
	{
		cross_count=0;
		flag=false;
		velocity=0.085;
		egginbasketnumber=1;
	}
}


//Decrement
void changeSpeed()
{
 deltax_first=((rand()%700)+100)/((GLdouble)(100000));
 deltax_second=((rand()%700)+100)/((GLdouble)(100000));
 deltax_third=((rand()%700)+100)/((GLdouble)(100000));
}

void decrement()
{
 ycord_first-=0.01;
 ycord_second-=0.01;
 ycord_third-=0.01;
 eggcentrey-=0.01;
 
 if(ycord_first<0.05 && egginbasketnumber==1)
  {
  ycord_first=0.05;
  slideFlag=false;
  basketBelow=1;
  basketAbove=3;
  ycord_second+=0.01;
  ycord_third+=0.01;
  changeSpeed();
  return;
  }
  if(ycord_second<0.05 && egginbasketnumber==2)
  {
   ycord_second=0.05;
   slideFlag=false;
   basketBelow=2;
   basketAbove=1;
   ycord_first+=0.01;
   ycord_third+=0.01;
   changeSpeed();
   return;
  }
  if(ycord_third<0.05 && egginbasketnumber==3)
  {
  ycord_third=0.05;
  slideFlag=false;
  basketBelow=3;
  basketAbove=2;
  ycord_first+=0.01;
  ycord_second+=0.01;
  changeSpeed();
  return ;
  }
  if(ycord_first <-0.34)
    ycord_first=0.85;
  if(ycord_second<-0.34)
    ycord_second=0.85;
  if(ycord_third<-0.34)
    ycord_third=0.85;
  return ;
}

//mouse function
void mouse(int button,int state,int x,int y){
   if(button==GLUT_LEFT_BUTTON && state==GLUT_UP ) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("CG_project");
	Initialize();
	
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutDisplayFunc(Draw);
    glutIdleFunc(Idle);
   	glutMouseFunc(mouse1);
  }
  else if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP ) {
    {
    	glutDestroyWindow(1);
    }
  }
 } 	

//Mouse1
void mouse1(int button,int state,int x,int y) 
{
	float x1=x/(float)width;
	float y1=y/(float)height;
 	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP ) 
 	{
 		if(gameoverflag==true)
 		{
 		
 			if(x1>=0.2 && x1<=0.72 && (1-y1)>=0.35 && (1-y1)<=0.75)
 				{
 					egginbasketnumber=basketBelow;
 					if(basketBelow==1)
 						eggcentrey=ycord_first+0.045;
 					else if(basketBelow==2)
 						eggcentrey=ycord_second+0.045;
 					else
 						eggcentrey=ycord_third+0.045;
 					gameoverflag=false;
 					life=initiallife;
 					points=0;
  				}
 		
 			if(x1>=0.2 && x1<=0.72 && (1-y1)>=0.1 && (1-y1)<=0.3)
 			exit(0);
 			
 		}
 		else
 		{
 			if(slideFlag==false)
 			flag=true;
 		}
	}
}

//DrawDigit/Num

void DrawDigit(int num,GLdouble cordx,GLdouble cordy)
{	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);
	glColor3f(0.0,1.0,0.0);
	glLineWidth(4.0);
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaGreen[]=	{0.0, 1.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	switch(num)
	{
		case 0:	
				glBegin(GL_LINE_LOOP);
				glVertex3f(cordx,cordy,-0.3);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glEnd();
				break;
				
		case 1:	
				glBegin(GL_LINES);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;

		case 2: 
				glBegin(GL_LINE_STRIP);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 3:	
				glBegin(GL_LINES);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 4:
				glBegin(GL_LINE_STRIP);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glEnd();
				glBegin(GL_LINES);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 5:
				glBegin(GL_LINE_STRIP);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx,cordy,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 6:
				glBegin(GL_LINE_STRIP);										
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 7:
				glBegin(GL_LINE_STRIP);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;
		case 8:
				glBegin(GL_LINE_LOOP);
				glVertex3f(cordx,cordy,-0.3);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glEnd();
				glBegin(GL_LINES);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glEnd();				
				//glutSwapBuffers();
				break;
		case 9:
				glBegin(GL_LINE_LOOP);
				glVertex3f(cordx,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.04,-0.3);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx,cordy+0.02,-0.3);
				glEnd();
				glBegin(GL_LINE_STRIP);
				glVertex3f(cordx+0.04,cordy+0.02,-0.3);
				glVertex3f(cordx+0.04,cordy,-0.3);
				glVertex3f(cordx,cordy,-0.3);
				glEnd();
				//glutSwapBuffers();
				break;			
	}		
	glLineWidth(1.0);
}

void DrawNum(int num,GLdouble cordx,GLdouble cordy)
{
	int num_of_digits=0,temp=num,digit;
	if(num==0)
	{
		DrawDigit(0,cordx,cordy);
		return ;
	}
	while(temp!=0)
	{
		num_of_digits++;
		temp/=10;
	}
	temp=num;
	while(num_of_digits>0)
	{
		digit=(temp/(int)(pow(10,num_of_digits-1)));
		temp=temp%((int)pow(10,num_of_digits-1));
		num_of_digits--;
		DrawDigit(digit,cordx,cordy);
		cordx+=0.05;	
	}
	
}

//AddtoBasket
void addToBasket(int basket)
{
 if(basket==1)
 {
  eggcentrex=xcord_first+0.03;
  eggcentrey=ycord_first+0.045;
 }
 if(basket==2)
 {
  eggcentrex=xcord_second+0.03;
  eggcentrey=ycord_second+0.045;
 }
 if(basket==3)
 {
  eggcentrex=xcord_third+0.03;
  eggcentrey=ycord_third+0.045;
 }
}

//display
void display(){
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(600.0, 1500.0);
  glVertex2f(1300.0, 1500.0);
  glVertex2f(1300.0, 250.0);
  glVertex2f(600.0, 250.0);
  glEnd();
  printb("The EGG GAME",800,1200);
  printb("Mini Project of Computer Graphics", 710, 1400);
  printb("Mentor: Mr. Pankaj Badoni", 750, 700);
  printb("Mainul Hasan and   Avi Sharma", 700, 1000);
  printb("R134214035              R134214015", 700, 900);
  printb("Left Click to start", 750, 300);
  glFlush();
  glutSwapBuffers();
  glutMouseFunc(mouse);
}

void myinit()
{
  glClearColor(1.0,1.0,1.0,1.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,2000.0,0.0,1600.0);
}

void title(){
  glutPostRedisplay();

}

//Main
int main(int iArgc, char** cppArgv) {

        for (int i = 0; i < 150; i++) {
	stars[i].x = rand() % 768;
	stars[i].y = rand() % 768;
	stars[i].vx = rand() / (float)RAND_MAX * 5 + 2;
	stars[i].vy = 0;
    }
	glutInit(&iArgc, cppArgv);
 	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  	glutInitWindowPosition(0,0);
  	glutInitWindowSize(ww,wh);
  	glutCreateWindow("The EGG Game");
  	glutDisplayFunc(display);
  	glutMouseFunc(mouse);
  	myinit();
	glutMainLoop();
	return 0;
}