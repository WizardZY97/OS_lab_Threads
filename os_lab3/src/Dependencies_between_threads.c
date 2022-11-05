#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void *funtion_thread (void *arg){
	int i = *((int *) arg);
  printf("Start executing the thread %d.\n", i);
  
  /* Bla, bla... */
  if (i == 1) sleep(10);
  
  printf("Terminate the thread %d.\n", i);
  
  return (void*)0;
}

int main (int argc, char **argv)
{

	if (argc != 1){
    fprintf(stderr, "Wrong usage : %s\n", argv[0]) ;
    exit (-1) ;
  }
  
  int i = 0;
  int index[10] = {0,1,2,3,4,5,6,7,8,9};
  int nb_threads = 10;
  pthread_t *tids;
  tids = malloc (nb_threads*sizeof(pthread_t)) ;
  
  /* Thread T0 */
  if(pthread_create (&tids[index[0]], NULL, funtion_thread, &index[0]) != 0){
      fprintf(stderr,"Failed to create thread number %d\n",index[0]);
  }
  
  pthread_join(tids[index[0]], NULL);
  
  i++;
  
  /*************/
  
  /* Thread T1 & T2 */
	if(pthread_create (&tids[index[1]], NULL, funtion_thread, &index[1]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[1]);
	}
	
  if(pthread_create (&tids[index[2]], NULL, funtion_thread, &index[2]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[2]);
	}
	
  pthread_join(tids[2], NULL);
  
  if(pthread_create (&tids[index[3]], NULL, funtion_thread, &index[3]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[3]);
	}
	
  if(pthread_create (&tids[index[4]], NULL, funtion_thread, &index[4]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[4]);
	}
	
	if(pthread_create (&tids[index[5]], NULL, funtion_thread, &index[5]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[5]);
	}
	
  pthread_join(tids[5], NULL);
  
  if(pthread_create (&tids[index[8]], NULL, funtion_thread, &index[8]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[8]);
	}
  
  pthread_join(tids[8], NULL);
  pthread_join(tids[4], NULL);
  
  if(pthread_create (&tids[index[7]], NULL, funtion_thread, &index[7]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[7]);
	}
  
  pthread_join(tids[1], NULL);
  pthread_join(tids[3], NULL);
  pthread_join(tids[7], NULL);
  
  if(pthread_create (&tids[index[6]], NULL, funtion_thread, &index[6]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[6]);
	}
  
  pthread_join(tids[6], NULL);
  /******************/
  
  /* Thread T9 */
  if(pthread_create (&tids[index[9]], NULL, funtion_thread, &index[9]) != 0){
			fprintf(stderr,"Failed to create thread number %d\n",index[9]);
	}
  
  pthread_join(tids[9], NULL);
  /*************/
  
  
  free (tids) ;
  
  return EXIT_SUCCESS;
}
