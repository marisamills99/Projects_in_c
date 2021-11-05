#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef struct _songnode
{   
  char title[35];
  char artist[35];
  char album[35];
  double duration;
  struct _songnode *next;
} SongNode;
void PrintAddSongMenu(){
	printf("Press 1 to add to end\n");
	printf("Press 2 to add to beginning\n");
	printf("Press 3 to add at a specific position\n");

}
void PrintPlaylistMenu(){
	printf("Press 1 to print playlist\n");
	printf("Press 2 to print duration\n");
	printf("Press 3 to search a song\n");
	printf("Press 4 to move a song up\n");
	printf("Press 5 to move a song down\n");
	printf("Press 6 to remove a song\n");
	printf("Press 7 to go to music library\n");
	printf("Press any other number to quit\n");
}
void PrintLibraryMenu(){
	printf("Press 1 to view all songs\n");
	printf("Press 2 to search by title\n");
	printf("Press 3 to add a song to a playlist\n");
	printf("Press any other number to return to playlist menu\n");

}
void printSongInfo(SongNode *song){
	printf("Title: %s Artist: %s \nAlbum: %s Duration: %.2lf \n\n",song->title,song->artist, song->album, song->duration);
}
SongNode *createSongNode(char *title, char *artist, char *album, double duration){
	
	SongNode *newNode= ( SongNode*) malloc(sizeof(SongNode));
	strcpy(newNode->title,title);
	
	strcpy(newNode->artist,artist);
	
	strcpy(newNode->album,album);
	
	newNode->duration=duration;
	return newNode;
	
}
/*SongNode* MusicLibrary( FILE * infile){
	char *line;
	char * title, * artist,* album;
	double duration;
	int numentries,i,postion;
	fscanf(infile, "%s", &line);
	SongNode* head, *newNode;
	if (line != EOF){
		line =title;
		fscanf(infile, "%s", &artist);
		fscanf(infile, "%s", &album);
		fscanf(infile, "%d", &duration);
		head= createSongNode(title,artist,album,duration);
		printf("Title: %s Artist: %s Album: %s Duration: %lf ",head->title,head->artist, head->album, head->duration);
	}
	
	while(!feof(infile)){
		fscanf(infile, "%s", &title);
		fscanf(infile, "%s", &artist);
		fscanf(infile, "%s", &album);
		fscanf(infile, "%d", &duration);
		newNode= createSongNode(title,artist,album,duration);
		printf("Title: %s Artist: %s Album: %s Duration: %lf ",newNode->title,newNode->artist, newNode->album, newNode->duration);
		//insert node
	}
	return head;
}*/
int insertSong(SongNode **head, SongNode *newNode, int position){
	int i=0;
	SongNode *temp = *head;
	char* title=newNode->title;
	char* album=newNode->album;
	char* artist=newNode->artist;
	double duration=newNode->duration;
	SongNode *toadd=createSongNode(title,artist,album,duration);
	if (temp==NULL){
		temp = toadd;
		return 0;
	}
	if (i==0&&temp->next==NULL){
			
			temp->next=toadd;
			return 0;
	}
	while(i<position){
		if (temp->next==NULL){
			printf("Adding to end of list\n");
			temp->next=toadd;
			return 0;
		}
		temp = temp->next;
		i++;
	}
	if (temp->next==NULL){
			printf("Adding to end of list\n");
			temp->next=toadd;
			return 0;
	}
	else{
	toadd->next= temp->next;
	temp->next=toadd;
	}
	return 0;
	
}
void printList(SongNode *head){
	printf("The list is \n\n");
	// temp is head of list
	SongNode *temp = head;
	//don't print dummy node
	temp = temp->next;

	while (temp != NULL){
		printSongInfo(temp);
		temp = temp->next;
	}
	printf("\n");
	free(temp);
}
double computeDuration(SongNode *head){
	double total;
	// temp is head of list
	SongNode *temp = head;
	//don't add dummy node
	temp=temp->next;
	
	while (temp != NULL){
		total+=temp->duration;
		temp = temp->next;
	}
	free(temp);
	return total;
}

SongNode *createEmptyList(){
	SongNode *head = (SongNode*) malloc(sizeof(SongNode));
	return head;
}

int appendSong(SongNode **head, SongNode *newNode){
	SongNode* temp=(SongNode*)malloc(sizeof(SongNode));
	
	//start pointing at head;
	temp = *head;
	printf("appending song %s\n", newNode->title);
	
	//printf("temp duration= %d \n", temp->duration);
	if (temp==NULL){
		printf("head song is %s\n", newNode->title);
		temp = newNode;
		temp->next=NULL;
		return 0;
	}

	int x=strcmp(temp->title,newNode->title);
	while (temp->next != NULL && x<0){
		x=strcmp(temp->next->title,newNode->title);
		if (x>0){
			break;
		}
		temp = temp->next;
		x=strcmp(temp->title,newNode->title);
		
	}
	if(temp->next== NULL){
		printf("next song is %s\n", newNode->title);
		temp->next = newNode;
	}

	else{
		newNode->next = temp->next;
		temp->next = newNode;
	}
	return 0;
	//free(temp);
}

SongNode* ReadSongData(FILE *infile){
	char inputname[200],line[50];
	int ch;
    
	
	//get name of input file the open to read
	infile = fopen(inputname, "r");
	if(infile == NULL) {
		printf("\nUnable to open input file!\n");
		return NULL;
	}
	else{
		//SongNode* playlist=IntializeSongArray(infile);
		//return playlist;
		}
		
	
}
int getNodePosition(SongNode *head, char *search){
	int counter=0;
	SongNode *temp = head;
	//don't add dummy node
	temp=temp->next;
	//capitalize search
	char * current;
	while (temp != NULL){
		current=temp->title;
		if(strcmp(current,search) ==0){
			printf("The position is %d\n",counter);
			return counter;
		}
		else{
		temp=temp->next;
		counter++;
		}
	}
	printf("Node Not Found\n");
	return -1;
}
SongNode *searchByTitle(SongNode *head, char *search){
	int x,i=0;
	SongNode *temp = head;
	char * current;
	//don't add dummy node
	temp=temp->next;
	//capitalize search
	while (search[i] != '\0') {
    	if(search[i] == ' '){
			search[i] = ' ';
		}
		else if (search[i] >= 'a' && search[i] <= 'z') {
        	search[i] = search[i] - 32;
    	}
      	i++;
		
	}
	while (temp != NULL){
		current=temp->title;
		x=strcmp(search,current);
		//printf("search title is %s and curr title is %s difference is %d\n", search,current,x);	
		if( x==0){
			printf("Found song\n");
			return temp;
		}
		else{
		temp=temp->next;
		}
	}
	printf("Node Not Found\n");
	return NULL;
}
SongNode *RemoveSong(SongNode **head, SongNode *remNode){
	char * search, *current;
	int x=0;
	SongNode* prev, *temp= *head;
	search= remNode->title;
	while (temp->next != NULL){
		current=temp->title;
		x=strcmp(search,current);
		if( x==0){
			
			if (prev == NULL){
				*head = temp->next;   // Changed head 
				//printf("temp title is %s and next title is %s\n", temp->title,temp->next->title);
				return temp; 
			}
			
			prev->next=temp->next;
			return temp;
			
		}
		else{
			prev= temp;
			temp=temp->next;
		}
	}
}
SongNode *removeSong(SongNode *head, SongNode *remNode){
	char * search, *current;
	int x=0;
	SongNode* prev=NULL, *temp= head;
	search= remNode->title;
	while (temp->next != NULL){
		current=temp->title;
		x=strcmp(search,current);
		if( x==0){
			if (prev == NULL){
				head = temp->next;   // Changed head 
				//printf("temp title is %s and next title is %s\n", temp->title,temp->next->title);
				return temp; 
			}
			
			prev->next=temp->next;
			return temp;
			
		}
		else{
			prev= temp;
			temp=temp->next;
		}
	}
}
int deleteSong(SongNode *head, SongNode *delNode){
	delNode=removeSong(head,delNode);
	free(delNode->artist);
	free(delNode->title);
	free(delNode->album);
	//free(delNode->duration);
	free(delNode);
}
int moveUp(SongNode** head,SongNode* curr){
	SongNode* temp= *head;
	int x=getNodePosition(temp,curr->title);
	if(x==-1){
		// Node doesn't exist 
		return -1;
	}
	else{
		--x;
		curr=removeSong(temp,curr);
		insertSong(head,curr,x);
	}
	return 0;
	
}
int moveDown(SongNode** head,SongNode* curr){
	SongNode* temp= *head;
	int x=getNodePosition(temp,curr->title);
	if(x==-1){
		// Node doesn't exist 
		return -1;
	}
	else{
		++x;
		curr=removeSong(temp,curr);
		insertSong(head,curr,x);
	}
	
}

SongNode* SongMenuSwitch(int inputnum, SongNode* playlistHead, SongNode* libraryHead){
	int index=-1, errornum;
	double duration;
	char line[36];
	char *title,*artist,*album;
	getchar();
	printf("Please give me a title\n");
	fgets(line,sizeof(line),stdin);
	sscanf(line,"%[^\n]",line);
	SongNode* nodeToAdd= searchByTitle(libraryHead,line);
	if (nodeToAdd==NULL){
		printf(" returning to menu\n");
		return playlistHead;
	}
	switch(inputnum){
	case 1:
		//add to end
		insertSong(&playlistHead, nodeToAdd, 10000);
		break;
	case 2:
		//add to beginning
		insertSong(&playlistHead,nodeToAdd,0);
		break;
	case 3:
		//add a song to position
		while(index<0){ 
		printf("what index would you like to insert at\n");
		scanf(" %d",&index);
		}
		insertSong(&playlistHead, nodeToAdd, index);
		break;	
	default:
		errornum= -1;
		return NULL;
	}
	removeSong(libraryHead,nodeToAdd);
	//printList(libraryHead);
	printList(playlistHead);
	return playlistHead;
	
	
}
SongNode* LibrarySwitch(int inputnum,SongNode* playlistHead, SongNode* libraryHead){
	int input,errornum=0;
	char *search;
	SongNode* curr;
	while(errornum!= -1){
		switch(inputnum){
		case 1:
			//view all songs;
			printList(libraryHead);
			PrintLibraryMenu();
			scanf(" %d",&inputnum);
			break;
		case 2:
			//search by title;
			printf("which song to search for\n");
			scanf(" %[^\n]",search);
			searchByTitle( libraryHead, search);
			PrintLibraryMenu();
			scanf(" %d",&inputnum);
			break;
		case 3:
			//add a song to playlist;
			PrintAddSongMenu();
			scanf(" %d",&input);
			playlistHead=SongMenuSwitch(input,playlistHead,libraryHead);
			//REMOVE FROM LIBRARY
			PrintLibraryMenu();
			scanf(" %d",&inputnum);
			break;	
		default:
			errornum= -1;
		}
	}
	return playlistHead;
}

int PlaylistSwitch(int inputnum, SongNode* head){
	int errornum=0, input;
	double total;
	char *search;
	SongNode* playlistHead= createEmptyList();
	SongNode* curr;
	char line[36];
	while(errornum!= -1){
	switch(inputnum){
		case 1:
			//print playlist;
			printList(playlistHead);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		case 2:
			//show duration;
			total =computeDuration(playlistHead);
			printf("The total playing time in playlist is %.2lf\n",total);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		case 3:
			//search title;
			getchar();
			printf("which song to search for\n");
			fgets(line,sizeof(line),stdin);
			sscanf(line,"%[^\n]",line);
			searchByTitle(playlistHead,line);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;	
		case 4:
			//move song up;
			getchar();
			printf("which song would you like to move up\n");
			fgets(line,sizeof(line),stdin);
			sscanf(line,"%[^\n]",line);
			curr=searchByTitle(playlistHead,line);
			moveUp(&playlistHead,curr);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		case 5:
			//move song down;
			getchar();
			printf("which song would you like to move down\n");
			fgets(line,sizeof(line),stdin);
			sscanf(line,"%[^\n]",line);
			curr=searchByTitle(playlistHead,line);
			moveDown(&playlistHead,curr);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		case 6:
			//remove a song;
			getchar();
			printf("which song would you like to remove\n");
			fgets(line,sizeof(line),stdin);
			sscanf(line,"%[^\n]",line);
			curr=searchByTitle(playlistHead,line);
			RemoveSong(&playlistHead,curr);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		case 7:
			//go to music library;
			PrintLibraryMenu();
			scanf(" %d",&input);
			LibrarySwitch(input,playlistHead,head);
			PrintPlaylistMenu();
			scanf(" %d",&inputnum);
			break;
		default:
			errornum= -1;
		}
	}
	return errornum;
}
void deleteList(SongNode** head){
	SongNode* temp = *head;
	SongNode* delNode;
	while (temp != NULL){
		//set current node = to next
		delNode = temp->next;
		//free current node
		free(delNode->artist);
		free(delNode->title);
		free(delNode->album);
		//free(delNode->duration);
		free(delNode);
		//make the new temp node the current node
		temp = delNode;
	}
	free(temp);
}
int main(int argc, char *argv[]){
	int inputnum,errornum=-1;
	FILE * infile;
	char *inputname;
	char title[35], artist[35], album[35],line[35];
	double duration;
	SongNode* head;
	
	printf("Welcome\n");
	if (argc!=2){
		printf("You need to give me a input file\n");
		return -1;
	}
	inputname = argv[1];
	
	infile = fopen(inputname, "r");
	if(infile == NULL) {
		printf("\nUnable to open input file!\n");
		return -1;
	}
	head = createEmptyList();
	while(fgets(line, sizeof(line), infile)!=NULL){
		//fgets(line, sizeof(line), infile);
		sscanf(line, "%[^\n]", title);
		fgets(line, sizeof(line), infile);
		sscanf(line, "%[^\n]",artist);
		fgets(line, sizeof(line), infile);
		sscanf(line, "%[^\n]", album);
		fgets(line, sizeof(line), infile);
		sscanf(line, "%lf", &duration);
		
		SongNode* newNode= createSongNode(title,artist,album,duration);
		appendSong(&head, newNode);
		
	}
	printList(head);
	PrintPlaylistMenu();
	scanf(" %d",&inputnum);
	errornum =PlaylistSwitch(inputnum,head);
	//deleteList(&head);
	return 0;
}

