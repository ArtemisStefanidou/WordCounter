#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h> /* Needed for Solaris */

#define NTHREADS 2//change number to 100,1000,10000,100000 (after that shows seg fault) 


pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
int sum=0;

typedef struct infoR info;
struct infoR
{
    char *buf;//the text that this thread must read
    int counter_thread;//the number of the thread
};

int is_separator(char ch);

void *word_counter(void *structs_args);



int main(int argc,char **argv)
{
    pthread_t threads[NTHREADS];
    info counter[NTHREADS];

    

    int fd, characters;

    if ((fd=open("artemis.txt", O_RDONLY ))==-1)
	{
		perror("open");
		exit(1);

	}

    long filesize=lseek(fd, 0, SEEK_END);
    
    
    if( filesize ==0 )
    {
        printf("The file is empty but continued normally");
        
    }

    
    char buffer[filesize];
    
    lseek(fd, 0, SEEK_SET);
	characters=read(fd, buffer, sizeof(buffer));

    //check if is in ASCII
    int i;
    for(i=0;i<filesize;i++)
    {
        if((int)buffer[i]>127 || (int)buffer[i]<0 ){
            printf("This file is not in ASCII\n");
            exit(1);
        }
    }

	printf("%d characters were read \n", characters);
	close(fd);

    
    for (int i = 0; i < NTHREADS; ++i)
    {
        
        counter[i].buf=buffer;
        counter[i].counter_thread=i;
        pthread_create(&threads[i], NULL, &word_counter, (void *)&(counter[i]));
       
    }
    
    for (int i = 0; i < NTHREADS; ++i)
    {
        pthread_join(threads[i],NULL);
    }
    printf ("The words of the file is: %d\n",sum);
  

    return 0;
}

int is_separator(char ch)
{
    if(ch=='\t' || ch=='\n'|| ch=='\0'|| ch==' ')
    {
        return 1;//true

    }
    return 0;//false
}

void *word_counter(void *structs_args)
{
    info *args;
    args =structs_args;
    int limit=args->counter_thread;
    int counter=0;
    int i;
    
    for(int i=limit*(strlen(args->buf)/NTHREADS);i<(limit+1)*(strlen(args->buf)/NTHREADS);i++)
    {
        if(!is_separator(args->buf[i]) && is_separator(args->buf[i+1]))
        {
            counter++;

        }
    }
    pthread_mutex_lock(&mymutex);
    sum=sum+counter;
    pthread_mutex_unlock(&mymutex);

   

    pthread_exit (NULL);
    
}