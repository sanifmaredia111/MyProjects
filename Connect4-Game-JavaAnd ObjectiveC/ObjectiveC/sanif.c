#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int a[6][7];
int turn=0;
int game_over=0;
int rows=6,cols=7;

pthread_t t_refree,t_player1,t_player2;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_consumer=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_producer=PTHREAD_COND_INITIALIZER;

void *refree(void*);

void *player_one(void*);
void *player_two(void*);

int main(int argc,char **argv)
{
  int i,j;
  for(i=0;i<rows;i++)
    {
      for(j=0;j<cols;j++)          //filling the board
        {
          a[i][j]=0;
        }
    }

  pthread_create( &t_player1, NULL, player_one, NULL);
  pthread_create( &t_player2, NULL, player_two, NULL);

  pthread_create(&t_refree,NULL,refree,NULL);

  //pthread_cond_init(&cond_consumer,NULL);
  //thread_cond_init(&cond_producer,NULL);
  pthread_join(t_player1,NULL);
  pthread_join(t_player2,NULL);
  pthread_join(t_refree,NULL);

}

void *refree(void *ptr)   
{
   int i,j,m,count2;
  while(1)
    {
      pthread_mutex_lock(&counter_mutex);

      while(turn==1)
	pthread_cond_wait(&cond_producer,&counter_mutex);

      turn=1;
     
      count2=0;
      for ( i= 0; i < rows; ++i)     //check for horizontal
	{
	  int count = 0;

	  for (j = 1; j < cols; ++j)
	    {
	      if (a[i][j] != 0 && a[i][j] == a[i][j-1])
		++count;
	      else
		count = 1;

	      if (count >= 4)
		{
		  game_over=1;
		  printf(" Player %d Won! Match:Horizontal\n", a[i][j]);
		  pthread_cond_signal(&cond_producer);
		  pthread_mutex_unlock(&counter_mutex);
		  return(NULL);
		}
	    }
	}  

      for (m=0; m < cols; ++m)    //board check whether it is empty or not
	{
	  if(a[0][m]!=0)
	    {
	      ++count2;
	    }
	}

      if (count2 >= 7)
	{
	  game_over=1;
	  printf("Board is full, Game result: Tie \n");
	  pthread_cond_signal(&cond_producer);
	  pthread_mutex_unlock(&counter_mutex);
	  return(NULL);
	}
      pthread_cond_signal(&cond_producer);
      pthread_mutex_unlock(&counter_mutex);  
    }
}
  void *player_one(void *ptr)
  {
    int b,i,j;
    time_t t;
    while(1)
      {
	pthread_mutex_lock(&counter_mutex);
	while(turn==0)
	  pthread_cond_wait(&cond_producer,&counter_mutex);
	if(game_over==1)
	  {
	    pthread_cond_signal(&cond_producer);
	    pthread_mutex_unlock(&counter_mutex);
	    return(NULL);
	  }
	sleep(1);
	srand(time(&t));

	b=rand()%7;
	for(i=5;i>=0;i--)
	  {
	    if(a[i][b]==0)
	      {
		a[i][b]=1;
                printf("board display after player one chance");
                printf("\n");
		break;
	      }
	  }
	//int j;
	for(i=0;i<rows;i++)
	  {
	    printf("|");
	    for(j=0;j<cols;j++)
	      {
		printf("%d \t",a[i][j]);
	      }
	    printf("\n");
	  }
	turn=0;
	pthread_cond_signal(&cond_producer);
	pthread_mutex_unlock(&counter_mutex); 
      }

  }
  void *player_two(void *ptr)
  {
    int b,i,j;
    time_t t;
    while(1)
      {
	pthread_mutex_lock(&counter_mutex);
	while(turn==0)
	  pthread_cond_wait(&cond_producer,&counter_mutex);
	if(game_over==1)
	  {
	    pthread_cond_signal(&cond_producer);
	    pthread_mutex_unlock(&counter_mutex);
	    return(NULL);
	  }
	sleep(1);
	srand(time(&t));
	b=rand()%7;
	for(i=5;i>=0;i--)
	  {
	    if(a[i][b]==0)
	      {
		a[i][b]=2;
                break;
	      }
	  }
	//	int j;
	printf("Board Display after player 2 chance");
        printf("\n");
  
	for(i=0;i<rows;i++)
	  {
	    printf("|");
	    for(j=0;j<cols;j++)
	      {
		printf("%d \t",a[i][j]);
	      }
	    printf("\n");
	  }
  
	turn=0;
	pthread_cond_signal(&cond_producer);
	pthread_mutex_unlock(&counter_mutex); 
      }
  }            

