#include<iostream>
#include<semaphore.h>
#include<pthread.h>
using namespace std;

int N=10;
const int bufferSize = 3;
int buffer[bufferSize];//If buffer is smaller then what (DONE)
int buffer_itr=0;
int pc=0,cc=0;
int NUM_PROD=3;//producer less or more than consumer (DONE)
int NUM_CONS=5;//consumer less or more than producer (DONE)
sem_t bufferCheck, sem, p, c;

void* producer(void* arg){
	while(pc<N){
		sem_wait(&p);	//letting one producer in at a time (synchronization of multiple producers)
		if (pc < N) {
			sem_wait(&bufferCheck);		//consuming slot of the buffer	
			//critical section start
			buffer[buffer_itr]=pc+1;
			buffer_itr++;
			pc++;
			//critical section end
			sem_post(&sem);		//keeping the record of items produced
		}
		sem_post(&p);	//turn of that producer ended now
	}
	pthread_exit(NULL);
}

void* consumer(void* arg){
	while(cc<N){
		sem_wait(&c);	//letting one consumer in at a time	(synchronization of multiple consumers)
		if (cc < N) {
			sem_wait(&sem);		//checking if theres anything produced to consume or not
			//critical section star
			buffer_itr--;
			cout<<buffer[buffer_itr]<<endl;
			cc++;
			//critical section end
			sem_post(&bufferCheck);		//freeing slot of the buffer
		}
		sem_post(&c);	//turn of that consumer ended now
	}
	pthread_exit(NULL);
}

int main(){
	//counting semaphores
	sem_init(&bufferCheck,0,bufferSize);	//for checking buffer size
	sem_init(&sem,0,0);		//for keeping record of items
	
	//binary semaphores
	sem_init(&p,0,1);	//for synchronization of multiple producers
	sem_init(&c,0,1);	//for synchronization of multiple consumers;
	
	pthread_t pid,cid;

	for (int i = 0; i<NUM_PROD; ++i) {
		pthread_create(&pid,NULL,producer,NULL);
	}

	for (int i = 0; i<NUM_CONS; ++i) {
		pthread_create(&cid,NULL,consumer,NULL);
	}
	
	pthread_exit(NULL);
}