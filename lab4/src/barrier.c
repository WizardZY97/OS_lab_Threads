#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NB_THREADS 16

int count = 0;
pthread_cond_t not_all_calls = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void barrier(void)
{
    pthread_mutex_lock(&mutex);
    count++;
    while(count < NB_THREADS) {
    	pthread_cond_wait(&not_all_calls, &mutex);
    }
    pthread_cond_signal(&not_all_calls);
    pthread_mutex_unlock(&mutex);
}



void* thread_routine(void* arg)
{
    unsigned long id = (unsigned long) arg;

    printf("thread %lu at the barrier\n", id);

    barrier();
    
    printf("\t thread %lu after the barrier\n", id);

    return NULL;
}


int main(void)
{
    pthread_t tids[NB_THREADS];
    unsigned long i=0;

    for(i=0; i<NB_THREADS; i++){
        if(pthread_create (&tids[i], NULL, thread_routine, (void*) i) != 0){
            fprintf(stderr,"Failed to create thread %lu\n", i);
            return EXIT_FAILURE;
        }
    }
    
    /* Wait until every thread ended */ 
    for (i = 0; i < NB_THREADS; i++){
        pthread_join (tids[i], NULL) ;
    }  
    
    return EXIT_SUCCESS;
}
