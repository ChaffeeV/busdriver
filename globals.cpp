
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include "globals.h"

/* Номер окна glut */
int window; 
// Глобальные переменные
GLuint texture[3];       // хранилище для 3-ух текстур 

GLfloat yrot;            // для поворотов

float xpos, zpos;    // координаты x и z
float speed = 0;    // скорость
