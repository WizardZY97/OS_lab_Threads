#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define NB_SONG 3

typedef struct cheering{
	int nb_songs;
	char* song_lyrics;
}cheering_t;

void *supporter (void *structure){
  char      *str = NULL ;
  int       i ;
  int       pid ;
	int				NB_SONGS ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  
	cheering_t *curr_cheering = (cheering_t *) structure;
	NB_SONGS = curr_cheering->nb_songs;
	str = curr_cheering->song_lyrics;

  for (i = 0; i < NB_SONGS; i++){
      printf ("Process %d Thread %x : %s \n", pid, (unsigned int) tid, str) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){

  int team1 ;
  int team2 ;
  int repeat1 ;
  int repeat2 ;
  int i ;
  int nb_threads = 0 ;
  pthread_t *tids ;
  char *lyrics1 = NULL, *lyrics2 = NULL;

  if (argc != 5){
    fprintf(stderr, "usage : %s team1 song_repeats_1 team2 song_repeats_2\n", argv[0]) ;
    exit (-1) ;
  }

  team1 = atoi (argv[1]) ;
  repeat1 = atoi (argv[2]) ;
  team2  = atoi (argv[3]) ;
  repeat2 = atoi (argv[4]) ;
  nb_threads = team1 + team2; 
  tids = malloc (nb_threads*sizeof(pthread_t)) ;
	
	cheering_t *cheering_French = (struct cheering *)malloc(sizeof(struct cheering));
	cheering_t *cheering_English = (struct cheering *)malloc(sizeof(struct cheering));
	
	lyrics1 = "Allons enfants de la patrie";
	cheering_French->song_lyrics = (char *)malloc(strlen(lyrics1) + 1);
	strcpy(cheering_French->song_lyrics, lyrics1);
	cheering_French->nb_songs = repeat1;
	
  lyrics2 = "Swing low, sweet chariot";
	cheering_English->song_lyrics = (char *)malloc(strlen(lyrics2) + 1);
	strcpy(cheering_English->song_lyrics, lyrics2);
	cheering_English->nb_songs = repeat2;
	
  /* Create the threads for team1 */
  for (i = 0 ; i < team1; i++){
    pthread_create (&tids[i], NULL, supporter, cheering_French) ;
  }
  /* Create the other threads (ie. team2) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, supporter, cheering_English) ;
  }  
	
  /* Wait until every thread ended */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free(cheering_French->song_lyrics);
	free(cheering_French);
	free(cheering_English->song_lyrics);
	free(cheering_English);
	
  free (tids) ;
  return EXIT_SUCCESS;
}
