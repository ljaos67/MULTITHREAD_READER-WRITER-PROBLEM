/****************************************************************
   PROGRAM:   Assignment 5
   AUTHOR:    Leonart Jaos
   LOGON ID:  Z1911688
   DUE DATE:  04/16/2022

   FUNCTION:  Linux program in C++ which solves the Reader-writer problem using semaphores and pthreads

   INPUT:     Input on command line

   OUTPUT:  The shared string is being printed once read. 

   NOTES:     
****************************************************************/
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <semaphore.h>

using namespace std;

string sharedString = "All work and no play makes Jack a dull boy.";
sem_t rw_sem; //used by both readers and writers
sem_t cs_sem; //used for protecting critical sections of readers
int readerCount = 0;

void *writer(void *param)
{
	//loop until the string is empty:
	while(sharedString != "")
	{
		//print out a message saying that it is writing
		sem_wait(&rw_sem);
		if(sharedString.size() > 0)
		{	
			printf("Writing\n");
			fflush(stdout);
			//writing (chopping the last character of the string)
			sharedString.pop_back();
		}
		sem_post(&rw_sem);
		sleep(1);
		//sleep for 1 second
	}
	return NULL;
}

void *reader(void *param)
{
	//The next two lines are not required in the assignment but are in the code to see which thread is operating
	//int tid = *(int*)param;
	//delete (int*)param;
	
	// enter the critical section
	while(sharedString != "")
	{
		sem_wait(&cs_sem); // mutex
		readerCount++;
		printf("Reader count:%d\n",readerCount);
		fflush(stdout);
		if(readerCount == 1)
			sem_wait(&rw_sem);
		sem_post(&cs_sem);
		// reading is performed
		//next line is included if current thread wants to be viewed
		//cout << tid << " is reading" << endl;
		printf("%s\n", sharedString.c_str());
		fflush(stdout);
		sem_wait(&cs_sem);
		readerCount--;
		printf("Reader count:%d\n",readerCount);
		fflush(stdout);
		if(readerCount == 0)
			sem_post(&rw_sem);
		sem_post(&cs_sem);
		sleep(1);
	}
	return NULL;
	

}
int main(int argc, char* argv[])
{ 
	if(argc < 3 || argc > 3)
	{
		printf("usage: z1911688_project5 arg1 arg2\n");
		return 1;
	}
	// take arguments from command line
	int numReaders = stoi(argv[1]);
	int numWriters = stoi(argv[2]);
	int newReaders = numReaders;
	int newWriters = numWriters;
	bool first = true;
	// check return values, 
	if(sem_init(&rw_sem, 0, 1) < 0 || sem_init(&cs_sem, 0, 1) < 0)
	{
		perror("Sem init error");
		exit(EXIT_FAILURE);
	}
	// create array for threads
	pthread_t readerThreads[numReaders];
	pthread_t writerThreads[numWriters];

	for(int i = 0; i < numReaders; i++)
	{
		// check error
		if(pthread_create(&readerThreads[i], NULL, reader, NULL) != 0)
		{
			if(first)
			{
				newReaders = i;
				first = false;
			}
		}			
	}
	first = true;
	for(int i = 0; i < numWriters; i++)
	{
		// check error	
		if(pthread_create(&writerThreads[i], NULL, writer, NULL) != 0)
		{
			if(first)
			{
				newWriters = i;
				first = false;
			}
		}			
	}
	
	for(int i = 0; i < newWriters; i++)
	{
		// check error

		if(pthread_join(writerThreads[i],NULL) != 0)
		{
			perror("Create failure");
			exit(EXIT_FAILURE);
		}	
	}
	for(int i = 0; i < newReaders; i++)
	{
		// check error
		if(pthread_join(readerThreads[i],NULL) != 0)
		{
			perror("Create failure");
			exit(EXIT_FAILURE);
		}		
	}
	// clear memory of semaphores
	sem_destroy(&rw_sem);
	sem_destroy(&cs_sem);
	
    return 0;
}
