#ifndef __MAP_H__
#define __MAP_H__

//Класс, содержащий массив объектов, метод load используется для загрузки карты из файла
class Map{
	public:
	Object* m[1000];
	Way w[1000];
	House h[1000];
	int N;
	GLfloat x[100];
	GLfloat z[100];

	void Load();
	void DrawAll();
};

#endif
