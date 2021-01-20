#include "wordcounter.h"

pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
int sum=0;

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
    
    
    if(limit==NTHREADS-1 && strlen(args->buf)%NTHREADS!=0)
    {

        for(int i=limit*(strlen(args->buf)/NTHREADS);i<((limit+1)*(strlen(args->buf)/NTHREADS))+(strlen(args->buf)%NTHREADS)-1;i++)
        {
           
            if(!is_separator(args->buf[i]) && is_separator(args->buf[i+1]))
            {
                
                counter++;
                
            }
        }
    }else
    {
        for(int i=limit*(strlen(args->buf)/NTHREADS);i<(limit+1)*(strlen(args->buf)/NTHREADS);i++)
        {
            if(!is_separator(args->buf[i]) && is_separator(args->buf[i+1]))
            {
                
                counter++;
                
            }
        }
    }

    pthread_mutex_lock(&mymutex);
    sum=sum+counter;
    pthread_mutex_unlock(&mymutex);

   

    pthread_exit (NULL);
    
}



int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    info counter[NTHREADS];
    int fd, characters;
    
    if ((fd=open(argv[1], O_RDONLY ))==-1)
    {
        perror("open");
        exit(1);
    }

    printf("%s\n",argv[1]);

    long filesize=lseek(fd, 0, SEEK_END);

    char buffer[filesize];
                
    lseek(fd, 0, SEEK_SET);
    characters=read(fd, buffer, sizeof(buffer));

    printf("%d characters were read from thread main\n", characters); 

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
    printf ("The words of the %s is: %d\n",argv[1],sum);

    return 0;
}       
