  
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include "image.h"
#include "globals.h"

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

Image::Image(){
	sizeX=0;
        sizeY=0;
        data = NULL;
	k = 0;
}

int Image::ImageLoad(int f) {
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
	
GLvoid Image::LoadGLTextures(GLvoid) {	
	glBindTexture(GL_TEXTURE_2D, texture[k-1]);     
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}  

