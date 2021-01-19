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

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    info counter[NTHREADS];

    filename=de->d_name; 
    int filename_size=strlen(argv[1])+1+strlen(filename);
    char buffer_of_filename[filename_size];
            
    sprintf(buffer_of_filename,"%s/%s",argv[1],filename);
        

    not_file = opendir (buffer_of_filename);

    if( not_file != NULL)
    {
        printf("The %s is not a file\n",de->d_name);
        continue;
    }

    if ((fd=open(buffer_of_filename, O_RDONLY ))==-1)
    {
        perror("open");
        continue;

    }

    printf("%s/%s\n",argv[1],filename);
    printf("%s\n",filename);

    long filesize=lseek(fd, 0, SEEK_END);
            
            
    if( filesize ==0 )
    {
        printf("The file is empty.\n");
        continue;
    }

            
    char buffer[filesize];
            
    lseek(fd, 0, SEEK_SET);
    characters=read(fd, buffer, sizeof(buffer));

    //check if is in ASCII
    if(isnot_ascii(filesize,buffer))
    {
        continue;
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

