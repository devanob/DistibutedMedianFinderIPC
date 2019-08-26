#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//child number
#define NUMCHILD 5

//global declarations
#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500

//defining reads
#define READ 0
#define WRITE 1

//Making pipes
int pipeParentToChild [NUMCHILD] [2];
int pipeChildToParent [NUMCHILD] [2];
int pipesID[2];

//messages to read and write
long read(int pipe[], int* recieve);
long write(int pipe[], int message);

//File structure
char fileName[] = "input_i.txt";
void readFile(char filename[], int numList[]);


int main(){
	setPipe();
	if(fork() == 0){
		childProcess();
	}
	else if (fork() == 0) {
		childProcess();
	}
	else if (fork() == 0) {
		childProcess();
	}
	else if (fork() == 0) {
		childProcess();
	}
	else if (fork() == 0) {
		childProcess();
	}
	else if (fork() > 0){
		parentProcess();
	}
	exit(0);
}

void childProcess() {
	int iD; //the child Id number
	int command; //the command read from the pipe from the parent
	int numCurrentEntries = 5;
	int numList[5];
	//List Of Numbers From File
	int pivot = 0;
	int change = 0;
	read(pipesID, &iD); //Read ID From Pipe One Synchonize
	 
	fileName[6] = '0' + (char)iD; //Change File To iD File
	readFile(fileName, numList);
	printf("%s:  %i %i %i %i %i \n", fileName, numList[0], numList[1], numList[2], numList[3], numList[4]);
	writeMssg(pipeChildToParent[iD], READY);
	readMssg(pipeChildToParent[iD], &command);
	if (command == REQUEST) {
		if (numList == 0) {
			writeMssg(pipeChildToParent[iD], -1);
		}
	}
	else if (command == PIVOT) {
		if (numList == 1)
			write(pipeChildToParent[iD], 0);
	}
	else if (command == LARGE) {
		
		for (int i = 0; i < numList; i++) {
			if (numList[i] < pivot) {
				change = decrease_element(numList, numList, i);
				i--;
			}

		}
	}
	else if (command == SMALL) {
		for (int i = 0; i < numList; i++) {
			if (numList[i] > pivot) {
				change = decrease_element(numList, numList, i);
				i--;
			}

		}
	} 
}
int decrease_element(int*from, int size, int index) {
	int i;
	for (i = index; i < size - 1; i++) from[i] = from[i + 1];
	if (size > 0)
		return size - 1;
	else {
		return 0;
	}

}
void readFile(char inputFile[], int listNumbers[]) {
	FILE* fileNum;
	fileNum = fopen(inputFile, "r");
	for (int i = 0; i < 5; i++) {
		if (fscanf(fileNum, "%d", &listNumbers[i]) != 1) {
			printf("Error Reading File");
			exit(0);
		}
	}
	fclose(fileNum);
}

void parentProcess() {
	assignId();
	int command;
	for (int i = 0; i < NUMCHILD; i++) {
		readMssg(pipeChildToParent[i], &command);
		//readMssg(childParentpipe[i],&mssg);
		if (command == READY) {
			printf("Child %i sends  ready\n", i);
		}
		else {
			exit(0);
		}

	}
	printf("Parent  ready\n");
	int mPivot = 0;
	int kMedian = 25 / 2;
	int pivot = 0;
	while (1) {
		pivot = requestFromParent();
		mPivot = broadCast(pivot);
		if (mPivot == kMedian) {
			printf("Median Found %i\n", pivot);

			exit(0);
		}
		else if (mPivot > kMedian) {
			for (int i = 0; i < 5; i++)
				writeMssg(pipeChildToParent[i], SMALL);
		}
		else if (mPivot < kMedian) {
			for (int i = 0; i < 5; i++)
				writeMssg(pipeChildToParent[i], LARGE);
			kMedian -= mPivot;

		}
	}
}

long read(int pipe[], int* recieve) {
	close(pipe[WRITE]);
	return(read(pipe[READ], recieve, sizeof(int)));
}

long write(int pipe[], int message) {
	close(pipe[READ]);
	return (write(pipe[WRITE], &message, sizeof(int)));
}

void assignId() {
	for (int i = 0; i < NUMCHILD; i++) {
		write(pipesID, i); //Write The ID to Pipe 1
	}

}

int broadCast(int pivot) {
	int m[5];
	for (int i = 0; i < 5; i++) {
		write(pipeParentToChild[i], PIVOT);
		write(pipeParentToChild[i], pivot);
	}
	//
	printf("Parent Broadcast %i To All Children", pivot);
	//
	for (int i = 0; i < 5; i++) {
		read(pipeChildToParent[i], &m[i]);
		printf("\nChild %i receives pivot and replies %i\n", i, m[i]);
	}
	int sum = m[0] + m[1] + m[2] + m[3] + m[4];
	printf("\nParent m = %i+%i+%i+%i+%i=%i\n", m[0], m[1], m[2], m[3], m[4], sum);
	return (sum);
}