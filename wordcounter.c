#include "wordcounter.h"

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
        for(int i=limit*(strlen(args->buf)/NTHREADS);i<((limit+1)*(strlen(args->buf)/NTHREADS))+strlen(args->buf)%NTHREADS;i++)
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

int isnot_ascii(int filesize,char *buffer)
{
    int i;
    for(i=0;i<filesize;i++)
    {
        if((int)buffer[i]>127 || (int)buffer[i]<0 ){
            printf("The file is not in ASCII\n");
            return 1;//true
        }
            
    }
    return 0;//false
}