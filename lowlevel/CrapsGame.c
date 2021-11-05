#include<stdio.h>
#include<time.h>
#include <stdlib.h>
//declare variables outside to access in multiple functions
int win=0,loss=0;
int betFor = 0, betAgainst = 0;
int bank = 100;
int ForOrAgainst=0;


int rolling(int seed)
{
int dice1,dice2, dicetotal;  
int diceroll;
  
printf("Press 1 to roll\n");
scanf("%d",&diceroll);

if(diceroll==1){
	
	dice1 = (random()%6)+1;
	dice2 = (random()%6)+1; 
	dicetotal=dice1+dice2;
	printf("\n The first number you rolled is : %d\n",dice1);
	printf("\n The second number you rolled is : %d\n",dice2);
	return dicetotal;
}

}


int playing(int balance, int seed)
{
    int inputNum,res=0;
    int val,bet,valueRolled;
	int total_bet_winning=0;
	int ForOrAgainst,letroll;
	int win=0;

	do{
		printf("\n Do you to bet for yourself? type 1 for yes or 0 for no \n ");
		scanf("%d",&inputNum);
		printf("\n How much do you want to bet? \n ");
		scanf("%d",&bet);
	if(bet>balance){
		//check if bet is bigger than bank
		printf("\n Bet is greater than bank");
		//ask again
		printf("\n How much do you want to bet?\n ");
		scanf("%d",&bet);
	}
	if (bet < 5){
		//check for minimum bet
		printf(" \n Bets must be above 5 dollars");
		//ask again
		printf("\n What amount do you want to bet?\n ");
		scanf("%d",&bet);
	}
	}while(bet < 5 || bet>balance);
	do{
	total_bet_winning=total_bet_winning+ bet;
    if(inputNum==0)
    {
		// keep track of if they bet for or against themselves 
		ForOrAgainst = 1;
		//roll the dice
        val = rolling(seed);

		if(val ==2|| val==3||val==12)
			{
			printf("\n You won %d dollars",bet);
			total_bet_winning=total_bet_winning+ bet;
			letroll=LetRoll();
			}
				
		else if(val==7|| val==11){
            printf("You lost %d dollars from your bank \n", bet);
			
        }

        else{
			//set the value they rolled = to the last roll
                valueRolled = val;
				// reset inputNum
                inputNum = 0;
				// only double bet if they have the money
                if(bet*2<=balance)
                {
                    printf("\n Would you like to double your bet? press 1 for yes and 0 for no : ");
                    scanf("%d",&inputNum);
                }
				//did they want to double the bet?
                if(inputNum==1){
                    bet= bet * 2;
					
				}
                do
                {
					//re roll the dice
                    val = rolling(seed);
                    if(val==7)
                    {
                        printf("\n You won %d dollars",bet);
						total_bet_winning=total_bet_winning+ bet;
						letroll=LetRoll();
						win=1;
						break;
                    }
					//check if they get the "point" 
					if (val==valueRolled)
					{
						printf("\n You lost %d dollars",bet);
						win=0;
						break;
					}
                }while(val!=valueRolled);
				// only stop rolling when val= the "point" value
				
        }

    }
    else
    {
        ForOrAgainst = 0;
		
        val = rolling(seed);
		letroll=0;

		if(val==7|| val==11)
		{
                printf("\n You won %d dollars",bet);
				total_bet_winning=total_bet_winning+ bet;
				letroll=LetRoll();
				win=1;
				
        }

		else if(val ==2|| val==3||val==12)
		{
                printf("\n You lost %d dollars",bet);
                win=0;
				letroll=0;
				
		}
        else
		{
			//keep track of the value rolled and set it = to last value 
			valueRolled = val;
			//reset inputNum
			inputNum =0;
			//check that bet can bet doubled
			if(bet*2<=balance)
			{
				printf("\n Would you like to double your bet? press 1 for yes and 0 for no : ");
				scanf("%d",&inputNum);
			}
			//do they want to double their bet?
			if(inputNum==1){
				bet = bet*2;
			}
			// after asking if they want to double their bet keep re-rolling until you either roll a 7 or the point
			do
			{
				val = rolling(seed);
				if(val==valueRolled)
				{
					printf("\n You won %d dollars",bet);
					total_bet_winning=total_bet_winning+ bet;
					letroll=LetRoll();
					win=1;
					break;
				}
				if(val==7)
				{
					printf("\n You lost %d dollars",bet);
					win=0;
					letroll=0;
					break;
				}
			
			}while(val!=7);

        }
    }
	printf("your winnings + initial bets are %d\n",total_bet_winning);
	if (letroll==1){
		total_bet_winning=0;
		bet=total_bet_winning;
	}
	}while(letroll==1);
	
    if(win==0)
    {

		// if you bet against bank balance is updated
        balance = balance-bet;
    }
    else
		// if you bet for bank balance is updated
    {
        balance = balance+bet;
    }
	
	
	

    return balance;
}
int LetRoll(){
	int roll;
	printf("\n do you want to let it roll? press 1 for yes otherwise press 0\n");
	scanf("%d",&roll);
	return roll;
}
int main()
{
	int seed = 0;
  printf("Welcome to craps please enter a random number seed: \n");
  scanf("%d", &seed);
	srand(seed); 
	//resets variables
	int ForOrAgainst=0, betAgainst=0, betFor=0;
    int inputNum;
    do
    {
		//call playing and the value returned is our current bank
        bank = playing(bank,seed);
			
        printf("\n Current Status");
		printf("\n balance: %d \n", bank);
		

        printf("\n Press 1 to quit or any other number to continue");
        scanf("%d",&inputNum);
    }while(inputNum!=1);

}
