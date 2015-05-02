/*
CS 344-400 Spring 2015
Program 2: Adventure!
Becky Solomon
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //for process id
#include <sys/stat.h> //for mkdir

const int NUM_ROOMS = 7;
const int NUM_ROOM_NAMES = 10;

//swaps anArray element at index A with element at index B
void swapElements(char *anArray[], int A, int B){
	char *temp;
	temp = anArray[A];
	anArray[A] = anArray[B];
	anArray[B] = temp;
	return;
}

//chooses numNames random elements from the array names[]
char* randName(char *names[], int numNames){
	int randNum = rand() % numNames;
	
	//swap names[randNum] with names[numNames - 1]
	swapElements(names, randNum, numNames - 1);
	
	return names[numNames - 1];
}

struct room
{
	char *name;
	char *type;
	int connections[6];
	int numConnections;
};
struct room rooms[7]; //an array of 7 rooms 

int main(void){
	srand(time(NULL)); //initialize random number generator

	//create directory solomreb.rooms.<processid>
	int pid = getpid();	
	char dirName[50];
	sprintf(dirName, "solomreb.rooms.%d", pid);
	if (mkdir(dirName, 0775) == -1){ 
		perror(dirName);
		exit(1);
	}
	
	//generate cave in memory
	char *roomNames[] = {"Scarlet", "PEACOCK", "plum", "MUSTARD", "white", "PEACH", "Lavender", "rOSE", "ButleR", "Clue"};
	//randomly pick 7 of the 10 rooms
	int i, randNum;
	for (i=0; i<7; i++){
		rooms[i].name = randName(roomNames, NUM_ROOM_NAMES - i);
		rooms[i].type = "MID_ROOM";
	}
		
	//pick at random one room to be the start
	randNum = rand() % 7;
	rooms[randNum].type = "START_ROOM";
	//swap chosen start_room with last element in array rooms[]
	struct room temp;
	temp = rooms[randNum];
	rooms[randNum] = rooms[6];
	rooms[6] = temp;
	
	//pick at random a different room to be the end
	randNum = rand() % 6;
	rooms[randNum].type = "END_ROOM";
	
	//the 5 remaining rooms stay mid_rooms
		
	for (i=0; i<7; i++){
		printf("%s: %s", rooms[i].name, rooms[i].type);
		printf("\n");		
	}
		//for each room,
			//numConnect = random number between 3 and 6
			//randomly generate numConnect connections chosen from roomNames[]
		
	
	//write out rooms to files
	//read in rooms from files
	//play the game
	
	

			
			
}

