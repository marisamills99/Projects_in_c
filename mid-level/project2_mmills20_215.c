#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Trip_struct{

     char membershipType[12];

     int startStationId, endStationId, bikeId,

         duration, startHr, startMin;

}Trip;

typedef struct Bike_struct{

   char maintenanceFlag;

   int bikeId, endStationId;

   int numTrips, totalDuration;

   float totalMinutes;

}Bike;
Trip* IntializeTripArray(FILE *infile);
int PrintMetricsSwitch(int inputnum);
void PrintMainMenu(){
	printf("Press 1 to Read trip data\n");
	printf("Press 2 to Run metrics\n");
	printf("Press 3 to Print menu\n");
	printf("Press 4 to Flag bikes for maintenance\n");
	printf("Press 5 to Update bike inventory\n");
	printf("Press any other number to quit\n");
}

void PrintMetricsMenu(){
	printf("Press 1 to print the 10 bikes with the longest duration in minutes\n");
	printf("Press 2 to print where the 10 bikes above are located at the end of the day\n");
	printf("Press 3 to print the 10 bikes with the most trips\n");
	printf("Press 4 to print how many bikes were only used for 2 or less trips\n");
	printf("Press 5 to print a list of the start stations and the number of trips for each of the above bikes\n");
	printf("Press 6 to print average number of trips per hour\n");
	printf("Press 7 to print the hour with largest number of trips\n");
	printf("Press 8 to print a report of the count of trips in each hour\n");
	printf("Press 9 to print average duration of the trips (in both milliseconds and minutes)\n");
	printf("Press 10 to print the trip with the longest duration (in both milliseconds and minutes)\n");
	printf("Press 11 to print the 5 busiest starting stations \n");
	printf("Press 12 to print the 5 busiest ending stations\n");
	printf("Press 13 to print the percentage of trips in each membership type\n");

}
void PrintMenu(){
	printf("Press 1 to print trip data\n");
	printf("Press 2 to print bike inventory\n");
	printf("Press 3 to Print CEO report\n");
	printf("Press 4 to print one bike\n");

}
int compare(const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Trip *)a)->duration;
	int r = ((Trip *)b)->duration;
	return r-l;
}
int compareTrips(const void * a, const void * b){
   /* function that sorts the numbers in ascending order*/
	int l = ((Bike *)a)->numTrips;
	int r = ((Bike *)b)->numTrips;
	return l-r;
}
int compareTripsdescending(const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Bike *)a)->numTrips;
	int r = ((Bike *)b)->numTrips;
	return l-r;
}
int compareDuration(const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Bike *)a)->totalDuration;
	int r = ((Bike *)b)->totalDuration;
	return r-l;
}

int compareStart(const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Trip *)a)->startStationId;
	int r = ((Trip *)b)->startStationId;
	return r-l;
}
int compareEnd(const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Trip *)a)->endStationId;
	int r = ((Trip *)b)->endStationId;
	return r-l;
}
int comparator (const void * a, const void * b){
   /* function that sorts the numbers in descending order*/
	int l = ((Bike *)a)->bikeId;
	int r = ((Bike *)b)->bikeId;
	return r-l;
}
Trip* ReadTripData(){
	FILE *infile;
	char inputname[200],line[50];
	int ch;
    printf("\nEnter filename for input: ");
	while ((ch = getchar()) != '\n' && ch != EOF);
    fgets(line, 50, stdin);
	sscanf(line,"%s",inputname);
	//get name of input file the open to read
	infile = fopen(inputname, "r");
	if(infile == NULL) {
		printf("\nUnable to open input file!\n");
		return NULL;
	}
	else{
		Trip* tripArr=IntializeTripArray(infile);
		return tripArr;
		}
	
}
Trip* IntializeTripArray(FILE *infile){
	//struct Trip_struct tripArr[4000];
	char line[200];
	int numentries,i;
	Trip mytrip;
	fscanf(infile, "%d", &numentries);
	// numentries * the size  of a trip is size of array
	Trip *tripsArray = malloc(numentries * sizeof (Trip)); // allocate memory for 4000x the size of one Trip
	//initialized first element to the number of entries
	mytrip.bikeId=numentries;
	tripsArray[0]=mytrip;
	fgets(line, sizeof(line), infile);
	for (i = 1; i < numentries; i++) {
		//get the line of input
		fgets(line, sizeof(line), infile);
		fscanf(infile, "%s%d%d%d%d%d%d", mytrip.membershipType, &mytrip.startStationId, &mytrip.endStationId, &mytrip.bikeId, &mytrip.duration,&mytrip.startHr,&mytrip.startMin);
		printf("%s %d %d %d %d %d %d\n", mytrip.membershipType, mytrip.startStationId,mytrip.endStationId, mytrip.bikeId, mytrip.duration,mytrip.startHr,mytrip.startMin);
		tripsArray[i]=mytrip;
	}

	

	return tripsArray;
}

int * countHours(Trip* tripsArray){
	
	int hourCount=0;
	int i;
	int hour=0;
	int Arrsize= tripsArray[0].bikeId;
	int *hourArr = malloc (sizeof (int) * 24);
	for(i=0;i<Arrsize;i){
		
		//loop through and create a list where i= hour and array[i] number of trips per hour;
		if (hour ==tripsArray[i+1].startHr){
			hourArr[hour]++;
			i++;
			hour=0;
		}
		else{
			hour++;
			
		}
	}
	
	return hourArr;
}
void countTripsScreen(int * hourArr){
	int i;
	for(i=0;i<24;i++){
		printf("At hour %d the trips were %d\n",i,*hourArr);
		hourArr++;
	}
}
void countTripsFile(int * hourArr,FILE * filename){
	int i;

	for(i=0;i<24;i++){
		fprintf(filename,"At hour %d the trips were %d\n",i,hourArr[i]);
	}
}
int hourMostTrips(int * hourArr){
	int i;
	int max=0;
	
	//i represents which hour, Array[i] is how many trips during that hour
	for(i=0;i<24;i++){
		if(hourArr[i]>max){
			max=i;
			
		}
	}
	printf("The busiest hour was %d\n",max);
	return max;
}

float averageTrips(int * hourArr){
	int i;
	float total=0;
	for(i=0;i<24;i++){
		total+=hourArr[i];
	}
	total= (float)total/24.0;
	printf("The average trips per day is %f\n",total);
	return total;
}
int* durationArray(Trip* tripsArray){
	int i,ArrSize;
	ArrSize= tripsArray[0].bikeId;
	int *durationArray = malloc (sizeof (int) * ArrSize);
	qsort(tripsArray,ArrSize,sizeof(Trip),compare);
	for(i=0;i<ArrSize;i++){
		//create a list where i=0 is the longest trip and i= ArrSize-1 is the shortest trip
		durationArray[i]=tripsArray[i+1].duration;
		//printf("dur arr = %d at %d and trip arr = %d\n",durationArray[i],i,tripsArray[i+1].duration);
	}
	return durationArray;
}
Bike * mostTripsArray(Bike* bikesArray){
	int ArrSize;
	ArrSize= 4000;
	
	qsort(bikesArray,ArrSize,sizeof(Bike),compareTripsdescending);
	printf("The Bikes with the most trips are %d %d %d %d %d %d %d %d %d %d\n",bikesArray[0].bikeId,bikesArray[1].bikeId,bikesArray[2].bikeId,bikesArray[3].bikeId,bikesArray[4].bikeId,bikesArray[5].bikeId,bikesArray[6].bikeId,bikesArray[7].bikeId,bikesArray[8].bikeId,bikesArray[9].bikeId);
	return bikesArray;
}
int fewTrips(Bike* bikesArray){
	int i,ArrSize,j=0;
	ArrSize= 4000;
	int *fewTripsArray=malloc (sizeof (int) * ArrSize);
	qsort(bikesArray,ArrSize,sizeof(Bike),compareTrips);
	
	for(i=0;i<ArrSize;i++){
		//create a list where the bike Id 
		if(bikesArray[i].numTrips<3){
			fewTripsArray[i]=bikesArray[i].bikeId;
			printf("BikeId %d has fewer than 3 trips\n",fewTripsArray[i]);
			j++;
		}
		
	}
	printf("There are a total of %d bikes with fewer than 3 trips\n",j);
	return j;
}
int printFewStartStations(int *fewTripsArray){
	int x,i=0;
	int ArrSize= sizeof(fewTripsArray)/sizeof(fewTripsArray[0]);
	while(i<ArrSize){
		x=fewTripsArray[i];
		i++;
	}
	return 0;
}

int averageDurationTrips(int * durationArray){
	int i;
	float total=0;
	int ArrSize= sizeof(durationArray)/sizeof(durationArray[0]);
	for(i=0;i<ArrSize;i++){
		int x=durationArray[i];
		total+=x;
	}
	total= (float)total/ArrSize;
	float seconds = durationArray[i] / 1000.0;
	float minutes = seconds / 60.0;
	printf("The average duration is %d ms %.2f min\n",durationArray[i],minutes);
	return total;
}
int longestTrip(int * durationArray){
	float seconds = durationArray[0] / 1000.0;
	float minutes = seconds / 60.0;
	printf("The longest trip duration is %d ms %.2f min\n",durationArray[0],minutes);
	return durationArray[0];
}
float totalMinutes(Trip trip){
	float milliseconds = trip.duration;
	float seconds = milliseconds / 1000.0;
	float minutes = seconds / 60.0;
	return minutes;
}
float totalMinutesb(int duration){
	float milliseconds = duration;
	float seconds = milliseconds / 1000.0;
	float minutes = seconds / 60.0;
	return minutes;
}
Bike* IntializeBikeArray(Trip* tripsArray){
	//go through trip array and create a new array for each bike
	int i,ArrSize,y,x=-1;
	ArrSize=tripsArray[0].bikeId;
	
	Bike * firstbikesArray = malloc(ArrSize * sizeof (Bike));
	Bike * secondbikesArray = malloc(4000 * sizeof (Bike));
	for(i=1;i<ArrSize;i++){
		firstbikesArray[i-1].maintenanceFlag='0';
		firstbikesArray[i-1].bikeId=tripsArray[i].bikeId;
		firstbikesArray[i-1].numTrips=1;
		firstbikesArray[i-1].totalDuration=tripsArray[i].duration;
		firstbikesArray[i-1].totalMinutes=totalMinutes(tripsArray[i]);
		firstbikesArray[i-1].endStationId=tripsArray[i].endStationId;
		printf("The bike is initialized as %c %d %d %d %d %.2f\n", firstbikesArray[i-1].maintenanceFlag, firstbikesArray[i-1].bikeId,firstbikesArray[i-1].endStationId, firstbikesArray[i-1].numTrips, firstbikesArray[i-1].totalDuration,firstbikesArray[i-1].totalMinutes);
		
	}

	printf("\n\n");
	qsort(firstbikesArray,ArrSize,sizeof(Bike),comparator);
	
	//j=i while next isn't individual id j++ increment i after done
	int j=0;
    for(i=0;i<ArrSize;i++){
		if(j==ArrSize-1){
          secondbikesArray[i]=firstbikesArray[j];
          
          break;
        }
        if(firstbikesArray[j].bikeId==firstbikesArray[j+1].bikeId){
      
        while(firstbikesArray[j].bikeId==firstbikesArray[j+1].bikeId){
			x++;
			y=j-x;
           
            firstbikesArray[y].totalDuration+=firstbikesArray[j+1].totalDuration;
			float z=firstbikesArray[j+1].totalDuration/60000.0;
            firstbikesArray[y].totalMinutes += (z);
            firstbikesArray[y].endStationId=firstbikesArray[j+1].endStationId;
            firstbikesArray[y].numTrips++;
			if(j==ArrSize-1){
                break;
            }
          j++;
        }
		//j was incremented
        secondbikesArray[i]=firstbikesArray[y];
        j++;
		x=-1;
        }
        else{
        if(j==ArrSize-1){
          secondbikesArray[i]=firstbikesArray[j];
          
          break;
        }
        if(firstbikesArray[j].bikeId!=firstbikesArray[j+1].bikeId){
                secondbikesArray[i]=firstbikesArray[j];
            }
        j++;
        }
	}		
		/*for(i=0;i<3000;i++){
			
			printf("The bike is initialized as %c %d %d %d %d %.2f\n", secondbikesArray[i].maintenanceFlag, secondbikesArray[i].bikeId,secondbikesArray[i].endStationId, secondbikesArray[i].numTrips, secondbikesArray[i].totalDuration,secondbikesArray[i].totalMinutes);
			
		}*/
	
	//sort bikes by bikeId 
	// if bike exists update duration, minutes, num trips,end station
	
	return firstbikesArray;
}
int MaintenanceFlag(Bike* mybikearr){
	int inputNum,i;
	printf("What bikeId would you like to update the flag\n");
	scanf(" %d",&inputNum);
	for(i=0;i<4000;i++){
		if (inputNum==mybikearr[i].bikeId){
			mybikearr[i].maintenanceFlag='1';
		}
		else{
			printf("Could not match that BikeId");
		}
	}
	return 0;
}
Bike* longestDuration(Bike * bikeArray){
	int i;
	Bike *bikearr = malloc (sizeof (Bike) * 10);
	qsort(bikeArray,4000,sizeof(Bike),compareDuration);
	for (i=0;i<10;i++){
		bikearr[i]=bikeArray[i];
		printf("bikearr[%d].duration = %d\n",i,bikearr[i].totalDuration);
	}
	printf("The 10 bikes with longest duration in minutes are %d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",bikearr[0].bikeId,bikearr[1].bikeId,bikearr[2].bikeId,bikearr[3].bikeId,bikearr[4].bikeId,bikearr[5].bikeId,bikearr[6].bikeId,bikearr[7].bikeId,bikearr[8].bikeId,bikearr[9].bikeId);
	return bikearr;
}
int FlagBikes(){
	//find the bikes with the slowest times
	// update maintenanceFlag to 1
	int i,inputNum=0;
	
	Trip* tripArr=ReadTripData();
	Bike* mybikearr=IntializeBikeArray(tripArr);
	Bike* longestbikes;
	printf("Would you like to find the bikes with the slowest times  press 1\n");
	printf("Would you like to update a maintenance flag press 2\n");
	scanf(" %d",&inputNum);
	while(inputNum!=1||inputNum!=2)
	switch(inputNum){
		case 1:
			longestbikes=longestDuration(mybikearr);
			for (i=0;i<10;i++){
				longestbikes[i].maintenanceFlag='1';
			}
			break;
		case 2:
			MaintenanceFlag(mybikearr);
			break;
		default:
			printf("Please choose 1 or 2\n");
			break;
	}
	return 0;
}



Bike* longestDurationStation(Bike * bikearr){
	printf("The 10 bikes with longest duration can be found at stations %d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",bikearr[0].endStationId,bikearr[1].endStationId,bikearr[2].endStationId,bikearr[3].endStationId,bikearr[4].endStationId,bikearr[5].endStationId,bikearr[6].endStationId,bikearr[7].endStationId,bikearr[8].endStationId,bikearr[9].endStationId);
	return bikearr;
}
Bike* AddBike(Bike * bikearr){
	int i=0;
	Bike mybike;
	printf("Please enter maintenance flag, bikeId, endStationId, numTrips, and totalDuration all separated by whitespace\n");
	scanf(" %c%d%d%d%d", mybike.maintenanceFlag,&mybike.bikeId, &mybike.endStationId, &mybike.numTrips, &mybike.totalDuration );
	mybike.totalMinutes=totalMinutesb(mybike.totalDuration);
	while(bikearr[i].bikeId){
		i++;
	}
	bikearr[i+1]=mybike;
	return bikearr;
}
Bike* RemoveBike(Bike * bikearr){
	int i=0;
	int bikeId;
	printf("Give me a bike ID \n");
	scanf(" %d", &bikeId);
	while(bikearr[i].bikeId){
		if (bikearr[i].bikeId== bikeId){
			bikearr[i].bikeId==NULL;
		}
		i++;
	}
	qsort(bikearr,4000,sizeof(Bike),comparator);
	return bikearr;
}
int UpdateInventory(){
	int inputNum;
	Trip* tripArr=ReadTripData();
	Bike* mybikearr=IntializeBikeArray(tripArr);
	printf("Press 1 to add a bike\n");
	printf("Press 2 to remove a bike\n");
	scanf("%d",&inputNum);
	switch(inputNum){
		case 1:
			AddBike(mybikearr);
			break;
		case 2:
			RemoveBike(mybikearr);
			break;
	}
	return 0;
}

int MetricsSubmenu(){
	int inputNum,inputNum2;
	char outputname[50], line[200];
	FILE *outfile;
	PrintMetricsMenu();
	scanf("%d",&inputNum);
	printf("Press 1 to print metrics on screen\n");
	printf("Press 2 to print metrics to a file\n");
	scanf("%d",&inputNum2);
	if (inputNum2==2){
		printf("Please enter the name of the file\n");
		fgets(line, 50, stdin);
		sscanf(line,"%s",outputname);
		//get the name of output file then open to write
		outfile = fopen(outputname, "w");
		if(outfile == NULL) {
			printf("Unable to open output file!\n");
			return -1;
		}
	}
	PrintMetricsSwitch(inputNum);
	return 0;
}

int* fiveEndStations(Trip* tripsArray){
	int i,j,ArrSize,counter;
	ArrSize= tripsArray[0].bikeId;
	int *endArray = malloc (sizeof (int) * ArrSize);
	int *endArray2 = malloc (sizeof (int) * ArrSize);
	qsort(tripsArray,ArrSize,sizeof(Trip),compareEnd);
	j=1;
	for(i=1;i<ArrSize;i++){
		counter=1;
		if (j==ArrSize-1){
            endArray[i-1]=tripsArray[j].endStationId;
            endArray2[i-1]=counter;
			return endArray;
        }
		while(tripsArray[j].endStationId==tripsArray[j+1].endStationId&&i<ArrSize){
			counter++;
			j++;
			if (j==ArrSize-1){
                endArray[i-1]=tripsArray[j].endStationId;
                endArray2[i-1]=counter;
                return endArray;
              }
		}
		if(tripsArray[j].endStationId!=tripsArray[j+1].endStationId){
        endArray[i-1]=tripsArray[j].endStationId;
        endArray2[i-1]=counter;
        j++;
		}
	}
	return endArray;

}

float membershipType(Trip* tripsArray){
	int i,ArrSize,totalcas=0;
	ArrSize= tripsArray[0].bikeId;
	for(i=1;i<ArrSize;i++){
		if(strcmp("Casual",tripsArray[i].membershipType)==0){
			++totalcas;
		}
	}
	float x=(float)totalcas/ArrSize;
	x*=100;
	float y= 100.0-x;
	printf("The total casual riders are %f%% and Registered are %f%%\n",x,y);
	return x;
}
int PrintMetricsSwitch(int inputnum){
	Trip* tripArr=ReadTripData();
	int * hourArr=countHours(tripArr);
	Bike* mybikearr=IntializeBikeArray(tripArr);
	int * durrArr = durationArray(tripArr);
	//int * fewTripsArray=fewTrips(mybikearr);
	Bike * bikearr;
		switch(inputnum){
		case 1:
			bikearr=longestDuration(mybikearr);
			break;
		case 2:
			bikearr=longestDuration(mybikearr);
			longestDurationStation(bikearr);
			break;
		case 3:
			// 10 bikes with the most trips
			mostTripsArray(mybikearr);
			break;
		case 4:
			fewTrips(mybikearr);
			break;
		case 5:
			//count start stations for above bikes
			break;
		case 6:
			averageTrips(hourArr);
			break;
		case 7:
			hourMostTrips(hourArr);
			break;
		case 8:
			countTripsScreen(hourArr);
			break;
		case 9:
			averageDurationTrips(durrArr);
			break;
		case 10:
			longestTrip(durrArr);
			break;
		case 11:
			//5 busiest starting station
			//have functions with logic could get them to work
			break;
		case 12:
			//5 busiest ending stations
			//have functions with logic could get them to work
			break;
		case 13:
			membershipType(tripArr);
			break;
	}
	return 0;
}
int PrintTripData(Trip* tripArr){
	int inputNum,i;
	char outputname[50],line[200];
	FILE *outfile;
	int numTrips=tripArr[0].bikeId;
	Trip mytrip;
	
	printf("Press 1 to print data on screen\n");
	printf("Press 2 to print data to a file\n");
	scanf("%d",&inputNum);
	if(inputNum==1){
		for(i=0;i<numTrips;i++){
			mytrip=tripArr[i];
			printf("membership=%s start station=%d end station=%d \nbikeId=%d duration=%d startHr=%d startMin=%d\n\n", mytrip.membershipType, mytrip.startStationId,mytrip.endStationId, mytrip.bikeId, mytrip.duration,mytrip.startHr,mytrip.startMin);
		
		}
	}
	if (inputNum==2){
		printf("Please enter the name of the file\n");
		fgets(line, 50, stdin);
		sscanf(line,"%s",outputname);
		//get the name of output file then open to write
		outfile = fopen(outputname, "w");
		if(outfile == NULL) {
			printf("\nUnable to open output file!");
			return -1;
		}
		
		for(i=0;i<numTrips;i++){
			mytrip=tripArr[i];
			fprintf(outfile,"membership=%s start station=%d end station=%d \nbikeId=%d duration=%d startHr=%d startMin=%d\n\n", mytrip.membershipType, mytrip.startStationId,mytrip.endStationId, mytrip.bikeId, mytrip.duration,mytrip.startHr,mytrip.startMin);
			
		}
	}
	
	return 0;
}
int PrintBikeInventory(Bike *bikearr){
	int bikeId,inputNum2,i;
	char outputname[50],line[200];
	FILE *outfile;
	printf("Enter a bike Id\n");
	scanf("%d",&bikeId);
	printf("Press 1 to print data on screen\n");
	printf("Press 2 to print data to a file\n");
	scanf("%d",&inputNum2);
	if (inputNum2==1){
		for(i=0;i<4000;i++){
		printf("The bike is initialized as flag=%c id =%d end station=%d trips=%d duration=%d minutes=%.2f\n", bikearr[i].maintenanceFlag, bikearr[i].bikeId,bikearr[i].endStationId, bikearr[i].numTrips, bikearr[i].totalDuration,bikearr[i].totalMinutes);
			if (bikearr[i+1].bikeId==0){
				break;
			}
		}
	}
	if (inputNum2==2){
		printf("Please enter the name of the file\n");
		fgets(line, 50, stdin);
		sscanf(line,"%s",outputname);
		//get the name of output file then open to write
		outfile = fopen(outputname, "w");
		if(outfile == NULL) {
			printf("\nUnable to open output file!");
			return -1;
		}
		for(i=0;i<4000;i++){
		
			fprintf(outfile,"The bike is initialized as flag=%c id =%d end station=%d trips=%d duration=%d minutes=%.2f\n", bikearr[i].maintenanceFlag, bikearr[i].bikeId,bikearr[i].endStationId, bikearr[i].numTrips, bikearr[i].totalDuration,bikearr[i].totalMinutes);
			/*if (bikearr[i+1].bikeId==NULL || bikearr[i+1].bikeId==0){
				break;
			}*/				
		}
	}
	return 0;
}
int PrintCeo(Bike * mybikearr){
	int j,ch;
	char outputname[50],line[50];
	FILE *outfile;
	Bike* bikearr;
	Bike* bikesArray;
	
	printf("Please enter the name of the file\n");
	while ((ch = getchar()) != '\n' && ch != EOF)
	fgets(line, 50, stdin);
	sscanf(line,"%s",outputname);
	//get the name of output file then open to write
	outfile = fopen(outputname, "w");
	if(outfile == NULL) {
		printf("\nUnable to open output file!\n");
		return -1;
	}
	
		bikearr=longestDuration(mybikearr);
		fprintf(outfile,"The 10 bikes with longest duration in minutes are %d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n\n",bikearr[0].bikeId,bikearr[1].bikeId,bikearr[2].bikeId,bikearr[3].bikeId,bikearr[4].bikeId,bikearr[5].bikeId,bikearr[6].bikeId,bikearr[7].bikeId,bikearr[8].bikeId,bikearr[9].bikeId);
		fprintf(outfile,"The 10 bikes with longest duration can be found at stations %d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n\n",bikearr[0].endStationId,bikearr[1].endStationId,bikearr[2].endStationId,bikearr[3].endStationId,bikearr[4].endStationId,bikearr[5].endStationId,bikearr[6].endStationId,bikearr[7].endStationId,bikearr[8].endStationId,bikearr[9].endStationId);
		bikesArray=mostTripsArray(bikearr);
		fprintf(outfile,"The Bikes with the most trips are %d %d %d %d %d %d %d %d %d %d\n\n",bikesArray[0].bikeId,bikesArray[1].bikeId,bikesArray[2].bikeId,bikesArray[3].bikeId,bikesArray[4].bikeId,bikesArray[5].bikeId,bikesArray[6].bikeId,bikesArray[7].bikeId,bikesArray[8].bikeId,bikesArray[9].bikeId);
		j=fewTrips(bikearr);
		fprintf(outfile,"There are a total of %d bikes with fewer than 3 trips\n\n",j);
	
	return 0;
}
int PrintOneBike(Bike * bikearr){
	int bikeId,inputNum2,i;
	char outputname[50],line[200];
	FILE *outfile;
	printf("Press 1 to print data on screen\n");
	printf("Press 2 to print data to a file\n");
	scanf(" %d",&inputNum2);
	printf("What bikeId would you like to print\n");
	scanf(" %d",&bikeId);
	if (inputNum2==1){
	for(i=0;i<4000;i++){
		if (bikeId==bikearr[i].bikeId){
			printf("The bike is initialized as %c %d %d %d %d %.2f\n", bikearr[i].maintenanceFlag, bikearr[i].bikeId,bikearr[i].endStationId, bikearr[i].numTrips, bikearr[i].totalDuration,bikearr[i].totalMinutes);
		}
	}
	}
	if (inputNum2==2){
		printf("Please enter the name of the file\n");
		fgets(line, 50, stdin);
		sscanf(line,"%s",outputname);
		//get the name of output file then open to write
		outfile = fopen(outputname, "w");
		if(outfile == NULL) {
			printf("\nUnable to open output file!");
			return -1;
		}
		for(i=0;i<4000;i++){
		if (bikeId==bikearr[i].bikeId){
			fprintf(outfile,"The bike is initialized as %c %d %d %d %d %.2f\n", bikearr[i].maintenanceFlag, bikearr[i].bikeId,bikearr[i].endStationId, bikearr[i].numTrips, bikearr[i].totalDuration,bikearr[i].totalMinutes);
		}
	}
	}
	return 0;
}

int PrintSubmenu(){
	int inputnum;
	PrintMenu();
	scanf(" %d",&inputnum);
	Trip* tripArr=ReadTripData();
	Bike* mybikearr=IntializeBikeArray(tripArr);
	
	switch(inputnum){
		case 1:
			PrintTripData(tripArr);
			break;
		case 2:
			PrintBikeInventory(mybikearr);
			break;
		case 3:
			PrintCeo(mybikearr);
			break;
		case 4:
			PrintOneBike(mybikearr);
			break;
	}
	return 0;
}

int main(){
	int inputnum,errornum=-1;
	printf("Welcome\n");
	do{
	PrintMainMenu();
	scanf("%d",&inputnum);
	switch(inputnum){
		case 1:
			errornum=ReadTripData();
			break;
		case 2:
			MetricsSubmenu();
			break;
		case 3:
			PrintSubmenu();
			break;
		case 4:
			FlagBikes();
			break;
		case 5:
			UpdateInventory();
			break;
		default:
			errornum= 2;
	}
	//bring back to main menu if there was an error
	}while(errornum==0||errornum==-1);
	
	return 0;
}