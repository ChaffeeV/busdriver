#ifndef __IMAGE_H__
#define __IMAGE_H__
// класс, используемы для загрузки изображения текстур в массив текстур texture[3]
class Image{
public:
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
	int k;

    Image();

    int ImageLoad(int f);
	GLvoid LoadGLTextures(GLvoid);
};
	
#endif
