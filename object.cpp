#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <math.h>
#include "object.h"
#include "globals.h"

float dlinna(float x1, float z1, float x2, float z2){
	float c = sqrt((x2-x1)*(x2-x1)+(z2-z1)*(z2-z1));
	return(c);
}
  
// Класс, содержащий координаты дороги или здания, соответственно, для зданий наследуется House, для дорог - way. В наследуемых классах описана виртуальная функция draw.


	GLvoid Object::getcx(GLfloat* x){
		for (int i=0; i<n; i++){
			cx[i] = x[i];
		}
	}

	GLvoid Object::getcz(GLfloat* z){
		for (int i=0; i<n; i++){
			cz[i] = z[i];
		}
	}

	GLvoid Object::getn(int p){
		n=p;
	}
	
	void Object::draw(){}





	void Way::draw(){
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


	void House::draw(){
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

