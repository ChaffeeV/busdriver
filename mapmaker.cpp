#include <string.h>
#include <stdio.h>      // Header file for standard file i/o.

int main(){
	char stop[5];
	char arr[30];
	double min1;
	double min2;
	double x[8000];
	double z[8000];
	int s[1000];
	for (int i=0; i<20; i++){
	s[i] =0;
	}
	int n=0;
	FILE *file;
	if ((file = fopen("Data/build.geojson", "r"))==NULL) {
       		printf("File Not Found");
       	}
	int k = 0;
	while(feof(file) == 0){
		fscanf(file,"%s", arr);
		if (strcmp(arr,"\"coordinates\":")==0){
		fscanf(file,"%*s%*s%*s%lf%*s%lf", &x[k], &z[k]);
		s[n]=s[n]+1;
		k++;
		fscanf(file,"%*s%s", stop);
			while(strcmp(stop,"[")==0){
				fscanf(file,"%lf%*s%lf", &x[k], &z[k]);
				s[n]=s[n]+1;
				k++;
				fscanf(file,"%*s%s", stop);
			}
		n++;
		}
		
	}

	min1 = x[0];
	
	for (int i = 1; i<k; i++){
		if(x[i]<min1){
			min1 = x[i];
		}
	}

	min2 = z[0];
	
	for (int i = 1; i<k; i++){
		if(z[i]<min2){
			min2 = z[i];
		}
	}
	
	for (int i = 0; i<k; i++){
		x[i]= x[i] - min1;
		x[i]=x[i]*20000;

	}
	for (int i = 0; i<k; i++){
		z[i]= z[i] - min2;
		z[i]= z[i]*20000;

	}
		fclose(file);

	

	if ((file = fopen("Data/high.geojson", "r"))==NULL) {
       		printf("File Not Found");
       	}
	int o = n;
	int w = k;
	while(feof(file) == 0){
		fscanf(file,"%s", arr);
		if (strcmp(arr,"\"coordinates\":")==0){
		fscanf(file,"%*s%*s%lf%*s%lf", &x[w], &z[w]);
		s[n]=s[n]+1;
		w++;
		fscanf(file,"%*s%s", stop);
			while(strcmp(stop,"[")==0){
				fscanf(file,"%lf%*s%lf", &x[w], &z[w]);
				s[n]=s[n]+1;
				w++;
				fscanf(file,"%*s%s", stop);
			}
		n++;
		}
		
	}

	
	
	for (int i = k; i<w; i++){
		x[i]= x[i] - min1;
		x[i]=x[i]*20000;

	}
	for (int i = k; i<w; i++){
		z[i]= z[i] - min2;
		z[i]= z[i]*20000;

	}
		fclose(file);




	if ((file = fopen("Data/map", "w"))==NULL) {
       		printf("File Not Found");
       	}
	fprintf(file, "%d", n);
	int t=0;
	for (int j=0; j<o; j++){

		fprintf(file, "\n2 %d\n", s[j]);
		for (int i=t; i<t+s[j]; i++){
			fprintf(file, "%.3lf ", x[i]);
		}
		fprintf(file,"\n");
		for (int i=t; i<t+s[j]; i++){
			fprintf(file, "%.3lf ", z[i]);
		}
		t = t + s[j];
	}
	for (int j=o; j<n; j++){

		fprintf(file, "\n1 %d\n", s[j]);
		for (int i=t; i<t+s[j]; i++){
			fprintf(file, "%.3lf ", x[i]);
		}
		fprintf(file,"\n");
		for (int i=t; i<t+s[j]; i++){
			fprintf(file, "%.3lf ", z[i]);
		}
		t = t + s[j];
	}
	return 0;
}
