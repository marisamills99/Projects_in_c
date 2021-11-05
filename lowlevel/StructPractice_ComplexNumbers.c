
#include <stdio.h>
//f(n, c) = f(n-1, c)*f(n-1, c) + c, and f(0, c) = c
typedef struct complex_t
{
    float real;
    float imagin;
} complex_t;

complex_t scan_complex(){
	complex_t result;
	float real;
	float imag;
	printf("\n What value do you want for real? \n");
	scanf("%f",&real);
	printf("\n What value do you want for imaginary? \n ");
	scanf("%f",&imag);
	
	result.real = real;
    result.imagin = imag;
	return result;
}
void print_complex(complex_t n1){
	printf("Complex Number is %.1f + %.1fi\n", n1.real, n1.imagin);
}
complex_t add_complex(complex_t n1, complex_t n2){
	complex_t newNum;

      newNum.real = n1.real + n2.real;
      newNum.imagin = n1.imagin + n2.imagin;

      return(newNum);	
}

complex_t multiply_complex(complex_t n1, complex_t n2){
	complex_t newNum;

    newNum.real = n1.real * n2.real;
    newNum.imagin = n1.imagin * n2.imagin;

    return(newNum);	
}
complex_t f(int n, complex_t c) {
	complex_t re; 
	if(n == 0) {
      return c;
	}
    re=f(n-1,c);
	re= multiply_complex(re,re);
	re=add_complex(re,c);
	return (re);
}
complex_t abs_complex(complex_t oldNum){
	complex_t newNum;
	if(oldNum.real <0){
    newNum.real = oldNum.real * -1;
	}
	else{
		newNum.real= oldNum.real;
	}
	if (oldNum.imagin <0){
    newNum.imagin = oldNum.imagin * -1;
	}
	else{
		newNum.imagin= oldNum.imagin;
	}

    return(newNum);	
}
int  main() {
	complex_t n1;
	int n;
	printf("Welcome to imaginary number calculator\n");
	printf("Type value for int n \n");
	scanf("%d",&n);
	printf("Insert real and imaginary value for your number\n");
	n1=scan_complex();
	printf("Calculate f(n) given f(n)= f(n-1, c)*f(n-1, c) + c, and f(0, c) = c\n");
	n1=f(n,n1);
	print_complex(n1);
	n1= abs_complex(n1);
	printf("The absolute value of your number is being computed\n \n");
	print_complex(n1);
   return 0;

}
