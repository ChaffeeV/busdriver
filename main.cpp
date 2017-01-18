
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for standard file i/o.
#include <math.h>       // Header file for trigonometric functions.
#include "globals.h"
#include "image.h"
#include "object.h"
#include "map.h"

#define ESCAPE 27
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77


const float piover180 = 0.0174532925f;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLsizei Width, GLsizei Height)	// We call this right after our OpenGL window is created.
{
	glGenTextures(3, &texture[0]);    
	Image buf[3];
	for(int i=0; i<3; i++){
		buf[i].ImageLoad(i+1);
		buf[i].LoadGLTextures();
	}
	
    glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Set the blending function for translucency (note off at init time)
    glClearColor(0.46f, 0.73f, 0.99f, 0.0f);	// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                       // type of depth test to do.
    glEnable(GL_DEPTH_TEST);                    // enables depth testing.
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix
    
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
	Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

Map map;  // Класс объявляется глобально, так как я не могу его передать в качестве аргумента в функцию отрисовки

/* Собственно функция отрисовки */
GLvoid DrawGLScene(GLvoid)
{
    GLfloat xtrans, ztrans, ytrans;
    GLfloat sceneroty;

    // Вычисление перемещений и поворотов
	xpos = xpos + (float)sin(yrot*piover180) * speed;
	zpos = zpos + (float)cos(yrot*piover180) * speed;
    xtrans = xpos;
    ztrans = zpos;
    ytrans = -0.25f;
    sceneroty = 360.0f - yrot;
    	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();

    glRotatef(0, 1.0f, 0, 0);
    glRotatef(sceneroty, 0, 1.0f, 0);



    glTranslatef(xtrans, ytrans, ztrans);

	

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    	
           glBindTexture(GL_TEXTURE_2D, texture[1]);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBegin(GL_QUADS);
    	
    	glNormal3f( 0.0f, -1.0f, 0.0f); 
     	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1.0f, -1000.0f);  // Top Right Of The Texture and Quad
     	glTexCoord2f(2000.0f, 0.0f); glVertex3f(1000.0f, -1.0f, -1000.0f);  // Top Left Of The Texture and Quad
     	glTexCoord2f(2000.0f, 2000.0f); glVertex3f(1000.0f, -1.0f, +1000.0f);  // Bottom Left Of The Texture and Quad
     	glTexCoord2f(0.0f, 2000.0f); glVertex3f(-1000.0f, -1.0f, +1000.0f);  // Bottom Right Of The Texture and Quad
    glEnd();

	map.DrawAll();

    // Показывает изображения нарисованное в буфере сейчас
    glutSwapBuffers();
}


/* Функция вызывается когда нормальная клавиша нажимается  */
void keyPressed(unsigned char key, int x, int y) 
{
    usleep(100);

    switch (key) {    
    case ESCAPE: // Выход из программы
	exit(1);                   	
	break;

    default:
      printf ("Key %d pressed. No action there yet.\n", key);
      break;
    }	
}

/* Функция вызывается, если нажата одна из специальных клавиш */
void specialKeyPressed(int key, int x, int y) 
{
    /* Хрень */
    usleep(100);

    switch (key) {    

    case GLUT_KEY_UP: // Увеличить скорость
	if (speed<0.2){
		speed += 0.01f;
	}
		
	break;

    case GLUT_KEY_DOWN: // Уменьшить скорость
	if (speed>-0.05){
	speed -= 0.01f;
	}
	
	break;

    case GLUT_KEY_LEFT: // Повернуть налево
	yrot += 1.5f;
	break;
    
    case GLUT_KEY_RIGHT: // Повернуть направо
	yrot -= 1.5f;
	break;

    default:
	printf ("Special key %d pressed. No action there yet.\n", key);
	break;
    }	
}



int main(int argc, char **argv) 
{    

	map.Load();
    glutInit(&argc, argv);   
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  
    glutInitWindowSize(1268, 768);  
    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);  
    /* Open a window */  
    window = glutCreateWindow("BusDriver");  
    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);  
    /* Go fullscreen.  This is as soon as possible. */
    glutFullScreen();
    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene); 
    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);
    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);
    /* Register the function called when special keys (arrows, page down, etc) are pressed. */
    glutSpecialFunc(&specialKeyPressed);
    /* Initialize our window. */
    InitGL(1280, 768);
    /* Start Event Processing Engine */  
    glutMainLoop();  
    return 1;
}
