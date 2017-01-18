#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include "object.h"
#include "map.h"

	void Map::Load(){
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
	void Map::DrawAll(){
		for(int i=0; i<N; i++){
			m[i]->draw();
			
		}
	}

