//  CS222  Debugging & generating a random number
//
//  Name:  ________________________________
// 
//   Generate a sequence of random numbers between 1 and 6 as rolling a dice
// 

#include <stdio.h>
#include <stdio.h> //#incldue <stdlib.h>

int main() {

 	int i, count; // int i, count,
  long my_random_num = 0; // declare your own var to hold random number

  srandom((unsigned int)(time(NULL))); 
  // "Seed" the random number generator with the current time 
 
  printf("Enter the number of rolling  -> ");
  //add an & change to %i
 // scanf("%d", count);
 scanf("%i", &count);

    for (i = 1; i < count; i++) //for (i == 1; i < count; i++);
	{
	my_random_num = random();  

     	system("stty raw -echo"); getchar(); system("stty cooked echo");
		printf("This rolling you get: %d.\nPress a key to continue:\n",1+my_random_num % 6);  
    	//printf("This rolling you get: %d.\nPress a key to continue:\n", 
            
	}
  printf("\n  Thanks for playing!  \n"); 
  return;
}

