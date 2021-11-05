// Marisa Mills G01075052
// CS 262, Lab Section 215
// Lab 2


//lab 2 Simple I/O and Credit Card Interest
#include<stdio.h>
#include <math.h>
int main()
{
	char inputBuf[20]; // A buffer to hold the input from the keyboard
	double expenses = 0;
	double apr = 0;
	int days = 0;
	double newsum = 0;
	double interest = 0;
	printf("\n Enter a value for the amount of expenses on the credit card.\n");
	fgets(inputBuf, 20, stdin); // Read the input from the keyboard and store it in inBuf
	sscanf(inputBuf, "%lf", &expenses); // Extract the numerical value from inBuf and store it in someInt

	printf("\n Enter a value for the APR on the credit card.\n");
	fgets(inputBuf, 20, stdin); // Read the input from the keyboard and store it in inBuf
	sscanf(inputBuf, "%lf", &apr); // Extract the numerical value from inBuf and store it in someInt
	
	printf("\n Enter a value for the days money is borrowed\n");
	fgets(inputBuf, 20, stdin); // Read the input from the keyboard and store it in inBuf
	sscanf(inputBuf, "%lf", &days);
	
	newsum= pow((expenses*((1+(apr/12)))),(12*(days/365.0)));
	// P' = P(1 + r/n)^(nt) 
	
	// Finally, it will print all five variables 
	//(expense amount, APR, number of days, amount of interest and total amount to be paid back
	fprintf("Your expense amount is %d, APR is %d, number of days is %d, amount of interest is %d,and total amount to be paid back is %d",&expenses,&apr,&days, &interest, &newsum);
}