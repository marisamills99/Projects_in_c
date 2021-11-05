
#include <stdio.h>
#include <math.h>
double fc_converter(double userNum){
    double newtemp;
    if(-200 < userNum && userNum < 200){
        newtemp=0.556*(userNum - 32.00);
        printf("%lf F ==> %lf C\n",userNum, newtemp);
        return (newtemp);
    }
    else{
        printf("Invalid Celsius temperature.");
        return newtemp= sqrt(userNum);
    }
}
double cf_converter(double userNum){
    double newtemp;
    if(-200 < userNum && userNum < 200){
        newtemp=1.8*userNum + 32.00;
        printf("%lf C ==> %lf F\n",userNum, newtemp);
    }
    else{
        printf("Invalid Fahrenheit temperature.\n");
        return newtemp= pow(userNum,3);
    }
}
double main() {
  char userIn;
  double userNum;
  double newtemp;
  do{
    printf("Marisa Mills > Fahrenheit/Celsius Converter. Please enter F or C:");
    scanf("%s", &userIn);
    switch(userIn) {
      case 'F' :
         printf("Marisa Mills > Please enter a Fahrenheit degree number:" );
         scanf("%lf", &userNum);
         newtemp=fc_converter(userNum);
         break;
      case 'C' :
         printf("Marisa Mills > Please enter a Celsius degree number:");
         scanf("%lf", &userNum);
         newtemp=cf_converter(userNum);
         break;
      case 'c' :
         printf("Marisa Mills > Please enter a Celsius degree number:");
         scanf("%lf", &userNum);
         newtemp= cf_converter(userNum);
         break;
      case 'f' :
         printf("Marisa Mills > Please enter a Fahrenheit degree number:" );
         scanf("%lf", &userNum);
         newtemp=fc_converter(userNum);
         break;
      default :
         printf("The input is unkown!\n" );
   }
    printf("The calculated value from the converter function is: %.2lf\n", newtemp);

   } while(userIn=='F'||userIn=='C'||userIn=='c'||userIn=='f');
}

