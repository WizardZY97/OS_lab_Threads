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
sem_t buffer_sem;

void pushing(int val)
{
    /* Mutex */
    
		pthread_mutex_lock(&buffer_mutex);
    // NOTE: pay attention to where to put the call to printf to be
    //   sure it can be helpful for debugging
    while (count == BUF_SIZE) {
    		pthread_mutex_unlock(&buffer_mutex);
    		printf("pushing value %d\n",val);
    		pthread_mutex_lock(&buffer_mutex);
    }
		count++;
    buffer[count] = val;
    printf("pushing value %d\n",val);
    pthread_mutex_unlock(&buffer_mutex);
    
    
    /* Semaphore */
    /*
    sem_wait(&buffer_sem);
    count = (count + 1) % BUF_SIZE; // avoid using "while" which would cause busy waiting.
    buffer[count] = val;
    printf("pushing value %d\n",val);
    sem_post(&buffer_sem);
    */
}

int fetching(void)
{
    int val=0;

    /* Mutex */
    
		pthread_mutex_lock(&buffer_mutex);
    val = buffer[count];
    // count = (count - 1) % BUF_SIZE; // mod a negative number won't work
    // NOTE: pay attention to where to put the call to printf to be
    //   sure it can be helpful for debugging 
    printf("\t feched value %d\n",val);
    pthread_mutex_unlock(&buffer_mutex);
    
    
    /* Semaphore */
    /*
    sem_wait(&buffer_sem);
    val = buffer[count];
    count = (count - 1) % BUF_SIZE;
    printf("\t feched value %d\n",val);
    sem_post(&buffer_sem);
    */
    
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
    
    sem_init(&buffer_sem, 0, 1);
    
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
