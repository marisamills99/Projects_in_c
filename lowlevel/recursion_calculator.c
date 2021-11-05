#include <stdio.h>
//Given f(0) = c and f(n) = f(n-1)*f(n-1) + c for n>0
float recursion(int n, float c) {
	float re; 
	
   if(n == 0) {
      return c;
   }
    re=recursion(n-1,c);
   return (re*re+ c);
}
int  main() {
	printf("Welcome to recursion calculator\n");
	printf("Calculate f(n) given f(0) = c and f(n) = f(n-1)*f(n-1) + c for n>0\n");
	int n;
	float c;
	do {
	printf("\n What value do you want for n? \n");
	scanf("%d",&n);
	printf("\n What value do you want for c? \n ");
	scanf("%f",&c);
	if(n<0){
		printf("\n n must be greater than zero");
	}
	}while(n<=-1);
	
      printf("\n The value for f(%d) is %f", n,recursion(n,c));
   return 0;

}