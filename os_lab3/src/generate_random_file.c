#define _GNU_SOURCE 

#include <fcntl.h>
#include <pthread.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <unistd.h>

#include <string.h>

#define NB_THREADS 8

int sub_val[NB_THREADS];

typedef struct struct_vector{
		int *data;
		int size;
		int index;
}struct_vector_t;

int sum(int *vect, int size) 
{
		int res = 0;
		size--;
		while (size >= 0) {
			res += vect[size];
			size--;
		}	
		
		return res;
}

void* function_thread(void *arg) 
{
		struct_vector_t *struct_vect = (struct_vector_t *) arg;
		
		int *res = (int *)malloc(sizeof(int)); 
		res[0] = sum(struct_vect->data, struct_vect->size);
		sub_val[struct_vect->index] = res[0];
		
		return (void *) res;
}

int main(int argc, char **argv)
{
    int fd;
    int *array;

    int size = 10*1024*1024;
    int final_size=0;
    int i=0;

    char filename[256];

		pthread_t *tids = (pthread_t *)malloc(NB_THREADS*sizeof(pthread_t));

    if (argc != 2){
        fprintf(stderr, "usage : %s nb_MB\n", argv[0]) ;
        exit (-1) ;
    }

    size = atoi (argv[1]) ;
    final_size = size * 1024 *1024;

		if (final_size % NB_THREADS != 0) {
				fprintf(stderr, "Wrong size.\n");
				exit (-1);
		}

    memset(filename, 0, 256);
    
    sprintf(filename, "/tmp/random_int_%dMB.data",size);
    
    srand(time(NULL));

    if((fd = open(filename, O_CREAT|O_RDWR, S_IRWXU))==-1){
        perror("file opening");
    }

    if(fallocate(fd, 0, 0, final_size*sizeof(int)) == -1){
        perror("fallocate");
    }

    
    if((array = (int*)mmap(NULL, final_size*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        perror("mmap");
    }
    
    for(i=0; i < final_size; i++){
        array[i] = (double) 10 * rand() / RAND_MAX;
    }
		
		/* The sequential version */
		
		printf("The sum of the elements of the a vector: %d.\n", sum(array, final_size));
		
		/**************************/
		
		/* Multi-threads */
		
		int sub_vector_size = final_size / NB_THREADS;
		struct_vector_t *sub_vectors = (struct_vector_t *)malloc(NB_THREADS*sizeof(struct_vector_t));
		for (i = 0; i < NB_THREADS; i++) 
		{
			sub_vectors[i].data = (int *)malloc(sub_vector_size*sizeof(int));
			sub_vectors[i].size = sub_vector_size;
			sub_vectors[i].index = i;
			for (int j = 0; j < sub_vector_size; j++) 
			{
				sub_vectors[i].data[j] = array[i * sub_vector_size + j];
			}
		}
		
		for (i = 0; i < NB_THREADS; i++) 
		{
			pthread_create(&tids[i], NULL, function_thread, &sub_vectors[i]);
		}
		
		int sum_val = 0;
		void *retval[NB_THREADS];
		for (i = 0; i < NB_THREADS; i++) 
		{
			pthread_join(tids[i], &retval[i]);
			int *val = (int *) retval[i];
			sum_val += val[0];
		}
		
		printf("The sum of the elements of the a vector: %d.\n", sum_val);
		
		/*****************/
		
		/* Shared variable */
		
		sum_val = 0;
		for (i = 0; i < NB_THREADS; i++) 
		{
			sum_val += sub_val[i];
		}
		
		printf("The sum of the elements of the a vector: %d.\n", sum_val);
		
		/*******************/
		
		for (i = 0; i < NB_THREADS; i++) 
		{
			free(retval[i]);
			free(sub_vectors[i].data);
		}
		free(sub_vectors);
		free(tids);
		
    munmap(array, final_size*sizeof(int));
    close(fd);
    
    return 0;
}
