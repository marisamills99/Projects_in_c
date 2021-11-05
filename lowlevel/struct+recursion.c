
#include "mandelbrot.h"
complex_t mandelbrot(complex_t c,int n){
	complex_t re; 
	if(n == 0) {
      return c;
	}
	if (re.real>10,000){
		return re;
	}
    re=mandelbrot(c,n-1);
	re= multiply_complex(re,re);
	re=add_complex(re,c);
	return (re);
}