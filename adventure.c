/*
CS 344-400 Spring 2015
Program 2: Adventure!
Becky Solomon
*/

#include <stdio.h> //for file i/o
#include <stdlib.h>
#include <time.h> //for random number generator
#include <unistd.h> //for process id
#include <sys/stat.h> //for mkdir
#include <string.h> //for strcpy, strchr

const int NUM_ROOMS = 7;
const int NUM_ROOM_NAMES = 10;

//swaps anArray element at index A with element at index B
void swapStringElements(char *anArray[], int A, int B){
	char *temp;
	temp = anArray[A];
	anArray[A] = anArray[B];
	anArray[B] = temp;
	return;
}

//swaps anArray element at index A with element at index B
void swapIntElements(int anArray[], int A, int B){
	int temp;
	temp = anArray[A];
	anArray[A] = anArray[B];
	anArray[B] = temp;
	return;
}

//chooses numNames unique random elements from the array names[]
char* randName(char *names[], int numNames){
	int randNum = rand() % numNames;
	
	//swap names[randNum] with names[numNames - 1]
	swapStringElements(names, randNum, numNames - 1);
	
	return names[numNames - 1];
}

//create a new directory called solomreb.rooms.<process id>
createDirectory(int pid){
	char dirName[50];
	sprintf(dirName, "solomreb.rooms.%d", pid);
	if (mkdir(dirName, 0775) == -1){ 
		perror("Error while creating directory");
		exit(1);
	}
	chdir(dirName);
}

struct room
{
	char *name;
	char *type;
	int connections[6];
	int numConnections;
};
struct room rooms[7]; //an array of 7 rooms 

//create a new file for a room
void createRoomFile(struct room rooms[], struct room r, int roomNum){
	FILE *fpr;
	int i;
	char roomName[2];
	sprintf(roomName, "%d", roomNum);
	fpr = fopen(roomName, "w");
	if (fpr == NULL){
		perror("Error while opening file");
		exit(1);
	}
	fprintf(fpr, "ROOM NAME: %s\n", r.name);
	for (i=0; i<r.numConnections; i++){
		fprintf(fpr, "CONNECTION %d: ", i+1);
		if (rooms[r.connections[i]].name == NULL)
			continue;
		fprintf(fpr, "%s\n", rooms[r.connections[i]].name);
	}
	fprintf(fpr, "ROOM TYPE: %s\n", r.type);
	return;
}

void readRoomFile(int roomNum, struct room* r){
	FILE *fpr;
	int i;
	char roomName[2];
	char buffer[50];
	sprintf(roomName, "%d", roomNum);
	printf("Opening file %s\n", roomName);
	fpr = fopen(roomName, "r");
		if (fpr == NULL){
		perror("Error while opening file");
		exit(1);
	}
	if (fgets (buffer , 50 , fpr) == NULL){
		perror("fgets error");
		exit(1);
	}

	printf("buffer= [%s]\n", buffer);
	strcpy(r->name, buffer + 11); //skip "ROOM_NAME: " (11 chars)
	char* newline;
	newline = strchr(r->name, '\n');
	*newline = '\0'; //replace newline with null char
	

}


int main(void){
	srand(time(NULL)); //initialize random number generator

//generate cave in memory
	char *roomNames[] = {"Scarlet", "PEACOCK", "plum", "MUSTARD", "white", "PEACH", "Lavender", "rOSE", "ButleR", "Clue"};
	//randomly pick 7 of the 10 rooms
	int i, j, randNum;
	for (i=0; i<NUM_ROOMS; i++){
		rooms[i].name = randName(roomNames, NUM_ROOM_NAMES - i);
		rooms[i].type = "MID_ROOM";
	}
		
	//pick at random one room to be the start
	randNum = rand() % NUM_ROOMS;
	rooms[randNum].type = "START_ROOM";
	//swap chosen start_room with last element in array rooms[]
	struct room temp;
	temp = rooms[randNum];
	rooms[randNum] = rooms[NUM_ROOMS - 1];
	rooms[NUM_ROOMS - 1] = temp;
	
	//pick at random a different room to be the end
	randNum = rand() %  (NUM_ROOMS - 1);
	rooms[randNum].type = "END_ROOM";
	
	//the 5 remaining rooms stay mid_rooms
		
	//for each room,
	for (i=0; i<7; i++){
		int unusedRoomNumbers[7];
		int numUnusedRooms = 7;
		//fill unusedRoomNumbers with 0 through 6 excluding i
		for (j=0; j<NUM_ROOMS; j++){
			if (j < i){
				unusedRoomNumbers[j] = j;
			}
			if (j > i){
				unusedRoomNumbers[j] = j + 1;
			}
		}
		
		//numConnections = random number between 3 and 6 inclusive
		int numConnections = (rand() % 4) + 3;
		rooms[i].numConnections = numConnections;
		
		//randomly generate numConnections connections chosen from unusedRoomNumbers[]
		for (j=0; j<numConnections; j++){
			randNum = rand() % numUnusedRooms;
			rooms[i].connections[j] = unusedRoomNumbers[randNum];
			swapIntElements(unusedRoomNumbers, randNum, numUnusedRooms - 1);
			numUnusedRooms--;	
		}
	}
		
	int k;
	int done = 0;
	for (i=0; i<NUM_ROOMS; i++){
	//for each connection, make sure it goes both ways
		for (j=0; j<rooms[i].numConnections; j++){
			int neighbor= rooms[i].connections[j];
			//check if one of neighbor's connections is i
			for (k=0; k< rooms[neighbor].numConnections;k++){
				if (rooms[neighbor].connections[k] == i){
					done = 1;				
				}
			}
			//if neighbor does not connect to i, add the connection
			if (done == 0){
				//set one of neighbor's connections to i
				rooms[neighbor].connections[rooms[neighbor].numConnections] = i;
				rooms[neighbor].numConnections++;
			}
		}
	}
//write out rooms to files
	
	//create directory solomreb.rooms.<processid>
	int pid = getpid();	
	createDirectory(pid);
	
	//create new files for each room
	for (i=0; i<NUM_ROOMS; i++){
		createRoomFile(rooms, rooms[i], i+1);
	}
	//read in rooms from files
	for (i=0; i<NUM_ROOMS; i++){
		struct room r;
		readRoomFile(i+1, &r);
	}
	
	//play the game
	printf("Welcome to the Becky's Cave Adventure!\n");
	printf("****************************************\n");

	int stepCount = 0;
	
	//start at START_ROOM
	
		
}

