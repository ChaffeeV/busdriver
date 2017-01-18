
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <math.h>       // Header file for trigonometric functions.


#define ESCAPE 27
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

/* Номер окна glut */
int window; 
const float piover180 = 0.0174532925f;

// Глобальные переменные
GLuint texture[3];       // хранилище для 3-ух текстур 

GLfloat yrot;            // для поворотов

float xpos, zpos;    // координаты x и z
float speed = 0;    // скорость

//Эти функции используются только в классе Image
static unsigned int getint(FILE *fp)
{
  int c, c1, c2, c3;

  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
  int c, c1;
  
  c = getc(fp);  
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

// Находит расстояние между 2-умя точками (используется в методах draw классов way и house)
float dlinna(float x1, float z1, float x2, float z2){
	float c = sqrt((x2-x1)*(x2-x1)+(z2-z1)*(z2-z1));
	return(c);
}

   
// класс, используемы для загрузки изображения текстур в массив текстур texture[3]
class Image{
public:
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
	int k;

    Image(){
        sizeX=0;
        sizeY=0;
        data = NULL;
	k = 0;
    }

    int ImageLoad(int f) 
    {
        FILE *file;
        unsigned long size;                 // Размер изображения в байтах.
        unsigned long i;                    
        unsigned short int planes;          // number of planes in image (must be 1) 
        unsigned short int bpp;             // number of bits per pixel (must be 24)
        char temp;                          // used to convert bgr to rgb color.

	char filename;
	k = f;
	sprintf(&filename, "Data/%d.bmp", k);
        if ((file = fopen(&filename, "rb"))==NULL) {
          printf("File Not Found : %s\n",&filename);
          return 0;
        }
    
        // seek through the bmp header, up to the width/height:
        fseek(file, 18, SEEK_CUR);

        // read the width
        sizeX = getint (file);
    
        // read the height 
        sizeY = getint (file);


        // calculate the size (assuming 24 bits or 3 bytes per pixel).
        size = sizeX * sizeY * 3;

        // read the planes
        planes = getshort(file);
        if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", &filename, planes);
        return 0;
        }

        // read the bpp
        bpp = getshort(file);
        if (bpp != 24) {
          printf("Bpp from %s is not 24: %u\n", &filename, bpp);
          return 0;
        }
    
        // seek past the rest of the bitmap header.
        fseek(file, 24, SEEK_CUR);

        // read the data. 
        data = (char *) malloc(size);
        if (data == NULL) {
            printf("Error allocating memory for color-corrected image data");
            return 0;   
        }

        if ((i = fread(data, size, 1, file)) != 1) {
            printf("Error reading image data from %s.\n", &filename);
            return 0;
        }

        for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = data[i];
        data[i] = data[i+2];
        data[i+2] = temp;
        }


        return 1;
    }  
	
	GLvoid LoadGLTextures(GLvoid) 
	{	
	    glBindTexture(GL_TEXTURE_2D, texture[k-1]); 
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}  
};

// Класс, содержащий координаты дороги или здания, соответственно, для зданий наследуется House, для дорог - way. В наследуемых классах описана виртуальная функция draw.
class Object{
	public:
	GLfloat cx[100];
	GLfloat cz[100];
	int n;

	GLvoid getcx(GLfloat* x){
		for (int i=0; i<n; i++){
			cx[i] = x[i];
		}
	}

	GLvoid getcz(GLfloat* z){
		for (int i=0; i<n; i++){
			cz[i] = z[i];
		}
	}

	GLvoid getn(int p){
		n=p;
	}

	virtual void draw(){}
};

class Way: public Object{
	public:
	void draw(){
		glBindTexture(GL_TEXTURE_2D, texture[0]);
    		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		GLfloat x11;
		GLfloat z11;
		GLfloat x12;
		GLfloat z12;
		GLfloat x21;
		GLfloat z21;
		GLfloat x22;
		GLfloat z22;
	
		float c;
		float a;
		float b;
		float cos;
		float sin;
	
		for(int i=0; i<n-1; i++){
			c = dlinna(cx[i], cz[i], cx[i+1], cz[i+1]);
			a = cx[i+1] - cx[i];
				b = cz[i+1] - cz[i];
			cos = b/c;
			sin = a/c;

			x11 = cx[i]-1.0f*cos;
			z11 = cz[i]+1.0f*sin;
			x12 = cx[i]+1.0f*cos;
			z12 = cz[i]-1.0f*sin;
		
			if(i>0){
				glBegin(GL_QUADS);
				glTexCoord2f(0.0, c/2); glVertex3f(x11, -0.98f, z11);  // Top Right Of The Texture and Quad
   				glTexCoord2f(0.0, 0.0); glVertex3f(x21, -0.98f, z21);  // Top Left Of The Texture and Quad
   				glTexCoord2f(1.0, 0.0); glVertex3f(x22, -0.98f, z22);  // Bottom Left Of The Texture and Quad
				glTexCoord2f(1.0, c/2); glVertex3f(x12, -0.98f, z12);  // Bottom Right Of The Texture and Quad
				glEnd();
			}

			x21 = cx[i+1]-1.0f*cos;
			z21 = cz[i+1]+1.0f*sin;
			x22 = cx[i+1]+1.0f*cos;
			z22 = cz[i+1]-1.0f*sin;

 			glBegin(GL_QUADS);
   			glTexCoord2f(0.0, c/2); glVertex3f(x11, -0.99f, z11);  // Top Right Of The Texture and Quad
   			glTexCoord2f(0.0, 0.0); glVertex3f(x21, -0.99f, z21);  // Top Left Of The Texture and Quad
   			glTexCoord2f(1.0, 0.0); glVertex3f(x22, -0.99f, z22);  // Bottom Left Of The Texture and Quad
   			glTexCoord2f(1.0, c/2); glVertex3f(x12, -0.99f, z12);  // Bottom Right Of The Texture and Quad
    			glEnd();
		}
	}
};

class House: public Object{
	public:
	void draw(){
	       glBindTexture(GL_TEXTURE_2D, texture[2]);
	    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


		float c;
		glBegin(GL_QUADS);
    
		for(int i=0; i<n-1; i++){
		    	c = dlinna(cx[i], cz[i], cx[i+1], cz[i+1]);
		    	glTexCoord2f(0.0, 0.0); glVertex3f(cx[i], -1.0f,  cz[i]);  // Bottom Left Of The Texture and Quad
		    	glTexCoord2f(c/2, 0.0); glVertex3f(cx[i+1],-1.0f,  cz[i+1]);  // Bottom Right Of The Texture and Quad
		    	glTexCoord2f(c/2, 1.0); glVertex3f(cx[i+1],1.0f,  cz[i+1]);  // Top Right Of The Texture and Quad
		    	glTexCoord2f(0.0, 1.0); glVertex3f(cx[i], 1.0f,  cz[i]);  // Top Left Of The Texture and Quad
		}

    		glEnd();
	}
};

//Класс, содержащий массив объектов, метод load используется для загрузки карты из файла
class Map{
	public:
	Object* m[1000];
	Way w[1000];
	House h[1000];
	int N;
	GLfloat x[100];
	GLfloat z[100];

	void Load(){
		int type;
		int n;
				
		FILE *file;
        	if ((file = fopen("Data/map", "r"))==NULL) {
       			printf("map:File Not Found");
        	}
		
		fscanf(file,"%d", &N);
		
		for(int i=0; i<N; i++){	
			
			fscanf(file, "%d", &type);
			fscanf(file, "%d", &n);
			for (int j=0; j<n; j++){
				fscanf(file, "%f", &z[j]);
			}
			
			for (int j=0; j<n; j++){
				fscanf(file, "%f", &x[j]);
			}
			if (type==1){
				m[i]= &w[i];	
			}
			
			else{
				m[i]= &h[i];
			}
			m[i]->getn(n);
			m[i]->getcx(x);
			m[i]->getcz(z);
		}	
	}
	void DrawAll(){
		for(int i=0; i<N; i++){
			m[i]->draw();
			
		}
	}
};



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

Map map;  // Класс объявляется глобально, так какя не могу его передать в качестве аргумента в функцию отрисовки

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
