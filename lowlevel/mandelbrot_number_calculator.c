
#include "mandelbrot.h"
#include <stdio.h>

int  main() {
	complex_t c;
	int n;
	printf("Welcome to mandelbrot number calculator\n");
	FILE* newfile=NULL;
	newfile=fopen("output.txt","w");
	
	if (newfile==NULL){
		printf("can not open file");
	}
	for (c.imagin = -1.12; c.imagin < 1.12; c.imagin+=0.077){
		for (c.real = -2.0; c.real < 0.47; c.real+=0.06175 ){
			if (abs_complex(mandelbrot(c,15)).real > 10000){
				printf("0");
				fputc('0',newfile);
				}
			else{
				printf("1");
				fputc('1',newfile);
				}
		}
		printf("\n"); 
		fputs("\r\n",newfile);
	}
	fclose(newfile);
	return (0);
	}