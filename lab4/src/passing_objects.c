#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define LOOPS 20

#define BUF_SIZE 4

int count = 0, in = 0, out = 0;
int buffer[BUF_SIZE];

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t lock_sem, full_sem, empty_sem;

void pushing(int val)
{
    /* Mutex (Busy waiting) */
    /*
		pthread_mutex_lock(&buffer_mutex);
    // NOTE: pay attention to where to put the call to printf to be
    //   sure it can be helpful for debugging
    while (count == BUF_SIZE) {
			// Busy waiting...
    	pthread_mutex_unlock(&buffer_mutex);
    	printf("pushing value %d...\n",val);
    	pthread_mutex_lock(&buffer_mutex);
    }
		count++;
    buffer[in] = val;
    in = (in + 1) % BUF_SIZE;
    printf("pushed value %d\n",val);
    pthread_mutex_unlock(&buffer_mutex);
    */
    
    /* Semaphore */
    sem_wait(&full_sem);
    sem_wait(&lock_sem);
    buffer[in] = val;
    in = (in + 1) % BUF_SIZE;
    printf("pushed value %d\n",val);
		sem_post(&lock_sem);
    sem_post(&empty_sem);
}

int fetching(void)
{
    int val=0;

    /* Mutex (Busy waiting) */
    /*
		pthread_mutex_lock(&buffer_mutex);
    // NOTE: pay attention to where to put the call to printf to be
    //   sure it can be helpful for debugging 
		while (count == 0) {
			// Busy waiting...
			pthread_mutex_unlock(&buffer_mutex);
    	printf("fectching value...\n");
    	pthread_mutex_lock(&buffer_mutex);
		}
		// count = (count - 1) % BUF_SIZE; // mod a negative number won't work
		count--;
    val = buffer[out];
    out = (out + 1) % BUF_SIZE;
    printf("\t feched value %d\n",val);
    pthread_mutex_unlock(&buffer_mutex);
    */
    
    /* Semaphore */
    sem_wait(&empty_sem);
		sem_wait(&lock_sem);
    val = buffer[out];
    out = (out + 1) % BUF_SIZE;
    printf("\t feched value %d\n",val);
		sem_post(&lock_sem);
    sem_post(&full_sem);
    
    return val;
}

void* thread_generating(void* arg)
{
    int i = 0;
    int new_value = 0;
    
    for(i=0; i<LOOPS; i++){
        new_value = rand()%10;
        pushing(new_value);
    }

    return NULL;
}


void* thread_using(void* arg)
{
    int i = 0;
    int value = 0;
    
    for(i=0; i<LOOPS; i++){
        value = fetching();
        //printf("Get the value: %d\n", value);
    }

    return NULL;
}



int main(void)
{
    pthread_t tids[2];
    int i=0;
    
    struct timespec tt;
    clock_gettime(CLOCK_MONOTONIC, &tt);
    /* seed for the random number generator */
    srand(tt.tv_sec);
    
    sem_init(&lock_sem, 0, 1);
    sem_init(&empty_sem, 0, 0);
    sem_init(&full_sem, 0, BUF_SIZE);
    
    if(pthread_create (&tids[0], NULL, thread_using, NULL) != 0){
        fprintf(stderr,"Failed to create the using thread\n");
        return EXIT_FAILURE;
    }
    
    if(pthread_create (&tids[1], NULL, thread_generating, NULL) != 0){
        fprintf(stderr,"Failed to create the generating thread\n");
        return EXIT_FAILURE;
    }
    
    /* Wait until every thread ended */ 
    for (i = 0; i < 2; i++){
        pthread_join (tids[i], NULL) ;
    }  
    
    return EXIT_SUCCESS;
}
