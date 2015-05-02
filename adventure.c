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
	int i, j, randNum;
	for (i=0; i<7; i++){
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
		for (j=0; j<7; j++){
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
	for (i=0; i<7; i++){
	//for each connection, does each neighbor connect to rooms[i]?
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
	for (i=0; i<7; i++){
		printf("%s: %s, connections: ", rooms[i].name, rooms[i].type);
		for (j=0; j<rooms[i].numConnections; j++){
			printf("%d, ", rooms[i].connections[j]);
		}
		printf("\n");		
	}
	
	//read in rooms from files
	//play the game
	
	

			
			
}

