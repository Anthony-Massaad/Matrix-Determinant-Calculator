#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/shm.h>
#include <time.h>
#include <sys/time.h>

#include "DET.h"

#define MICRO_SEC 1000000


int getLargestIntInRow(int index, int M[3][3]){
	int largest = 0;
	for (int i = 0; i < 3; i++){
		if (M[index][i] > largest){
			largest = M[index][i];
		}
	}	
	return largest;
}

void computationFinished(int* comp, int* R, int largestRowInt[3], struct timeval end, int D[3], struct timeval start){
	if (*comp == 0){
		gettimeofday(&end, NULL);
		*R = D[0] + D[1] + D[2];
		int largest = largestRowInt[0];
		for (int i = 1; i < 3; i++){
			if (largestRowInt[i] > largest){
				largest = largestRowInt[i];
			}
		}
		printf("Value of R is %d\n", *R);
		printf("Largest int is %d\n", largest);
		printf("Elapsed Time: %ld micro secnds\n", ((end.tv_sec * MICRO_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC + start.tv_usec)));
	}

}

int main(int argc, char **argv){

	int M[3][3] = {{20,20,50}, {10,6,70}, {40,3,2}};
	int index = 0; 

	void *shared_memory = (void *)(0);
	struct shared_use_st *shared_stuff; 
	struct timeval start, end;
	int shmid; 
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	
	if (shmid == -1){
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1){
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);	
	}
	
	shared_stuff = (struct shared_use_st *)shared_memory;
	shared_stuff->comp = 3; 
	shared_stuff->R = 0; 
	pid_t pid;
	gettimeofday(&start, NULL);
	pid = fork(); 

	switch(pid){
		case -1: 
			perror("fork failed");
			exit(1);
		case 0: // parent 1 child
			if (shmid == -1){
				fprintf(stderr, "shmget failed\n");
				exit(EXIT_FAILURE);
			}
			shared_memory = shmat(shmid, (void *)0, 0);
			
			if (shared_memory == (void *)-1){
				fprintf(stderr, "shmat failed\n");
				exit(EXIT_FAILURE);	
			}

			shared_stuff = (struct shared_use_st *)shared_memory;

			printf("Child Process: Working with element %d of D\n", index);
						
			shared_stuff->largestRowInt[index] = getLargestIntInRow(index, M);
			shared_stuff->D[index] = M[0][0] * (M[1][1]*M[2][2] - M[1][2]*M[2][1]);
			shared_stuff->comp--; 
			computationFinished(&shared_stuff->comp, &shared_stuff->R, shared_stuff->largestRowInt, end, shared_stuff->D, start);

			break;
		default: // parent 1 forking to the next pid

			index++;
			pid = fork();

			switch(pid){
				case -1:
					perror("fork failed");
					exit(EXIT_FAILURE);
				case 0: // parent 2 child

					if (shmid == -1){
						fprintf(stderr, "shmget failed\n");
						exit(EXIT_FAILURE);
					}	
					shared_memory = shmat(shmid, (void *)0, 0);
					
					if (shared_memory == (void *)-1){
						fprintf(stderr, "shmat failed\n");
						exit(EXIT_FAILURE);	
					}


					shared_stuff = (struct shared_use_st *)shared_memory;

					printf("Child Process: Working with element %d of D\n", index);
						
					shared_stuff->largestRowInt[index] = getLargestIntInRow(index, M);
					shared_stuff->D[index] = M[0][1] * (M[1][2]*M[2][0] - M[1][0]*M[2][2]);
					shared_stuff->comp--; 
					computationFinished(&shared_stuff->comp, &shared_stuff->R, shared_stuff->largestRowInt, end, shared_stuff->D, start);					
					break;
				
				default: // parent 2 forking to parent 3
					index++;
					pid = fork();
					switch(pid){
						case -1:
							perror("fork failed");
							exit(EXIT_FAILURE);
						case 0: // parent 2 child

							if (shmid == -1){
								fprintf(stderr, "shmget failed\n");
								exit(EXIT_FAILURE);
							}	
							shared_memory = shmat(shmid, (void *)0, 0);
							
							if (shared_memory == (void *)-1){
								fprintf(stderr, "shmat failed\n");
								exit(EXIT_FAILURE);	
							}


							shared_stuff = (struct shared_use_st *)shared_memory;

							printf("Child Process: Working with element %d of D\n", index);
								
							shared_stuff->largestRowInt[index] = getLargestIntInRow(index, M);
							shared_stuff->D[index] = M[0][2] * (M[1][0]*M[2][1] - M[1][1]*M[2][0]);
							shared_stuff->comp--; 
							computationFinished(&shared_stuff->comp, &shared_stuff->R, shared_stuff->largestRowInt, end, shared_stuff->D, start);					
							break;

						default:
							break;
					}
					break;	
			
			}
			break;
		}

	return 0;
}
