#ifndef __OBJECT_H__
#define __OBJECT_H__

// Класс, содержащий координаты дороги или здания, соответственно, для зданий наследуется House, для дорог - way. В наследуемых классах описана виртуальная функция draw.
class Object{
	public:
	GLfloat cx[100];
	GLfloat cz[100];
	int n;

	GLvoid getcx(GLfloat* x);

	GLvoid getcz(GLfloat* z);
	GLvoid getn(int p);

	virtual void draw();
};

class Way: public Object{
	public:
	void draw();
};

class House: public Object{
	public:
	void draw();
};

#endif
