/*
* CS 344-400 Spring 2015
* Program 2: Cave Adventure
* Becky Solomon
*/

#include <stdio.h> //for file i/o
#include <stdlib.h>
#include <time.h> //for random number generator
#include <unistd.h> //for process id
#include <sys/stat.h> //for mkdir
#include <string.h> //for strcpy, strchr


struct room
{
	char name[20];
	char type[20];
	int connections[6];
	int numConnections;
};
struct room rooms[7]; //an array of 7 rooms 

const int NUM_ROOMS = 7;
const int NUM_ROOM_NAMES = 10;
int cur_room, end_room;


//swaps anArray element at index A with element at index B
void swapStringElements(char *anArray[], int A, int B);

//swaps anArray element at index A with element at index B
void swapIntElements(int anArray[], int A, int B);

//chooses numNames unique random elements from the array names[]
char* randName(char *names[], int numNames);

//creates a new directory called solomreb.rooms.<process id>
void createDirectory(int pid);

//creates a new file with filename roomNum for a struct room, 
//including room name, room type, and room connections
void createRoomFile(struct room r, int roomNum);

//given name, return index of that room in array rooms[]
int findRoom(char* name);

//reads room name and room type from file named roomNum,
//stores member data in r
void readRoomNameType(int roomNum, struct room* r);

//reads room connections from file named roomNum,
//stores member data in r
void readRoomConnections(int roomNum, struct room* r);


int main(void){
	srand(time(NULL)); //initialize random number generator

//generate cave in memory
	char *roomNames[] = {"Scarlet", "PEACOCK", "plum", "MUSTARD", "white", "PEACH", "Lavender", "rOSE", "ButleR", "Clue"};
		int path[7];
	int next_room, i, j, k, done, randNum, numConnections, numUnusedRooms, neighbor, pid, stepCount;
	char destination[50];
	char *newline;
	struct room temp;

	//randomly pick 7 of the 10 rooms. initialize names and default to MID_ROOM room type
	for (i=0; i<NUM_ROOMS; i++){
		strcpy(rooms[i].name, randName(roomNames, NUM_ROOM_NAMES - i));
		strcpy(rooms[i].type, "MID_ROOM");
	}
		
	//pick at random one room to be the START_ROOM
	randNum = rand() % NUM_ROOMS;
	strcpy(rooms[randNum].type,"START_ROOM");
	//swap chosen start_room with last element in array rooms[]
	temp = rooms[randNum];
	rooms[randNum] = rooms[NUM_ROOMS - 1];
	rooms[NUM_ROOMS - 1] = temp;
	
	//pick at random a different room to be the END_ROOM
	randNum = rand() %  (NUM_ROOMS - 1);
	strcpy(rooms[randNum].type, "END_ROOM");
	
		
	//for each room,
	for (i=0; i<7; i++){
		int unusedRoomNumbers[7];
		numUnusedRooms = 0;
		//fill unusedRoomNumbers with 0 through 6 excluding i
		for (j=0; j < NUM_ROOMS; j++){
            if (j != i) {
                unusedRoomNumbers[numUnusedRooms] = j;
                numUnusedRooms++;
            }
        }
		
		//numConnections = random number between 3 and 6 inclusive
		numConnections = (rand() % 4) + 3;
		rooms[i].numConnections = numConnections;
		
		//randomly generate numConnections connections chosen from unusedRoomNumbers[]
		for (j=0; j<numConnections; j++){
			randNum = rand() % numUnusedRooms;
			rooms[i].connections[j] = unusedRoomNumbers[randNum];
			swapIntElements(unusedRoomNumbers, randNum, numUnusedRooms - 1);
			numUnusedRooms--;	
		}
	}
	
	//for each connection, make sure it goes both ways	
	done = 0;
	for (i=0; i<NUM_ROOMS; i++){
		for (j=0; j<rooms[i].numConnections; j++){
			neighbor = rooms[i].connections[j];
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
	pid = getpid();	
	createDirectory(pid);
	
	//create new files for each room
	for (i=0; i<NUM_ROOMS; i++){
		createRoomFile(rooms[i], i+1);
	}
	
//read in rooms from files
	for (i=0; i<NUM_ROOMS; i++){
		readRoomNameType(i+1, &rooms[i]);
		if (strcmp(rooms[i].type, "START_ROOM")== 0){
			cur_room = i; 
		}
		if (strcmp(rooms[i].type, "END_ROOM")== 0){
			end_room = i; 
		}
		readRoomConnections(i+1,&rooms[i]);
	}
	
//play the game
	printf("\n****************************************\n");
	printf("Welcome to the Becky's Cave Adventure!\n");
	printf("****************************************\n\n");

	stepCount = 0;
	for(;;){
		printf("CURRENT LOCATION: %s\n", rooms[cur_room].name);
		printf("POSSIBLE CONNECTIONS: ");
		for (i=0; i<rooms[cur_room].numConnections; i++){
			printf("%s",rooms[rooms[cur_room].connections[i]].name);
			if (i == rooms[cur_room].numConnections - 1){
				printf(".\n");
			}
			else{
				printf(", ");
			}
		}
		
		//prompt for next room
		printf("WHERE TO? >");
		fflush(stdout);
		if(fgets(destination, 50, stdin) == NULL){
			printf("\n");
			exit (0);
		}
		newline = strchr(destination, '\n');
		*newline = '\0'; //strip newline from string
		printf("\n");
		
		//check for valid room name
		next_room = findRoom(destination);
		
		//check for valid connection
		for (i=0; i<7; i++){
			if(strcmp(rooms[rooms[cur_room].connections[i]].name, destination)){
				next_room == -1;
			}
		}
		
		if(next_room == -1){
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}
		else{
			cur_room = next_room;
			path[stepCount] = cur_room;
			stepCount++;
			if (cur_room == end_room){
				printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
				printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepCount);
				
				for(i=0; i<stepCount; i++){
					printf("%s\n", rooms[path[i]].name);
					
				}	
				exit(0);			
			}
		}	
	}	
}

void swapStringElements(char *anArray[], int A, int B){
	char *temp;
	temp = anArray[A];
	anArray[A] = anArray[B];
	anArray[B] = temp;
	return;
}

void swapIntElements(int anArray[], int A, int B){
	int temp;
	temp = anArray[A];
	anArray[A] = anArray[B];
	anArray[B] = temp;
	return;
}

char* randName(char *names[], int numNames){
	int randNum = rand() % numNames;
	
	//swap names[randNum] with names[numNames - 1]
	swapStringElements(names, randNum, numNames - 1);
	
	return names[numNames - 1];
}

void createDirectory(int pid){
	char dirName[50];
	sprintf(dirName, "solomreb.rooms.%d", pid);
	if (mkdir(dirName, 0775) == -1){ 
		perror("Error while creating directory");
		exit(1);
	}
	chdir(dirName);
	return;
}

void createRoomFile(struct room r, int roomNum){
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
	fclose(fpr);
	return;
}


//given name, return index of that room in array rooms[]
int findRoom(char* name){
	int i;
	for (i=0; i<7; i++){
		if (strcmp(name, rooms[i].name) == 0){
			return i;		
		}
	}
	return -1;	
}

void readRoomNameType(int roomNum, struct room* r){
	FILE *fpr;
	int i;
	char roomName[2];
	char buffer[50];
	char* newline;
	int connection=0;
	
	sprintf(roomName, "%d", roomNum);
	fpr = fopen(roomName, "r");
		if (fpr == NULL){
		perror("Error while opening file");
		exit(1);
		}
	
	while (fgets(buffer , 50 , fpr) != NULL){
		//printf("buffer: %s\n", buffer);
		if (strncmp(buffer,"ROOM NAME",9) == 0){
			//read room name
			strcpy(r->name, buffer + 11); //skip "ROOM_NAME: " (11 chars)
			newline = strchr(r->name, '\n');
			*newline = '\0'; //replace newline with null char
		}
		else if (strncmp(buffer,"ROOM TYPE",9) == 0){
			//read room type
			strcpy(r->type, buffer + 11); //skip "ROOM_TYPE: " (11 chars)
			newline = strchr(r->type, '\n');
			*newline = '\0'; //replace newline with null char
		}

	}
	fclose(fpr);

}

void readRoomConnections(int roomNum, struct room* r){
	FILE *fpr;
	int i;
	char roomName[2];
	char buffer[50];
	char* newline;
	int connection=0;
	
	sprintf(roomName, "%d", roomNum);
	fpr = fopen(roomName, "r");
		if (fpr == NULL){
		perror("Error while opening file");
		exit(1);
		}
	
	while (fgets(buffer , 50 , fpr) != NULL){
		if (strncmp(buffer,"CONNECTION",9) == 0){
			//read connections
			char connectionName[50];
			strcpy(connectionName, buffer + 14); //skip "CONNECTION #: " (14 chars)
			newline = strchr(connectionName, '\n');
			*newline = '\0'; //replace newline with null char
			r->connections[connection] = findRoom(connectionName);
			connection++;
		}
		
	}
	fclose(fpr);
}
