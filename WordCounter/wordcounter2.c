#define NTHREADS 4

//common includes for both main
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>
#include <pthread.h>


typedef struct infoR info;
struct infoR
{
    char *filename;//the text that this thread must read
    int counter_thread;//the number of the thread
    int size;//size of file
};



//initialise the locks for threads
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

//initialise the total sum for the threads of one file
int sum=0;

//check if the given character is one from the separators of a word
int is_separator(char ch)
{
    if(ch=='\t' || ch=='\n'|| ch=='\0'|| ch==' ')
    {
        return 1;//true

    }
    return 0;//false
}

//counts the words of a file with the help of the threads
void *word_counter(void *structs_args)
{
    //a struct that contains the information for one thread
    info *args;
    args =structs_args;

    char characters[2];

    int fd;
    if ((fd=open(args->filename, O_RDONLY ))==-1)
    {
            //Print to user the corresponding message
            perror("");
            //return to the while to continue the reading of the folder
            exit(-1);

    }


    //the number of the thread
    int limit=args->counter_thread;

    //counter for words that read a thread
    int counter=0;
    int i;
    
    long filesize=lseek(fd,0,SEEK_END);

    lseek(fd,0,SEEK_SET);
    lseek(fd, limit * (filesize/NTHREADS), SEEK_SET );

    //check if is the last thread to read correctly the lasts characters in buffer
    if(limit==NTHREADS-1 && filesize%NTHREADS!=0)
    {

        //define the piece that will read the last thread
        for(int i=limit * (filesize/NTHREADS);i<((limit+1)*(filesize/NTHREADS))+(filesize % NTHREADS)-1;i++)
        {
            read(fd, characters, 2);
            //check if it is in the end of a word
            if(!is_separator(characters[0]) && is_separator(characters[1]))
            {
                
                //increase the counter of the words that read
                counter++;

            }
            lseek(fd,-1,SEEK_CUR );
        }
    }
    else
    {
        //define the piece that will read the last thread
        for(int i=limit * (filesize/NTHREADS);i<((limit+1)*(filesize/NTHREADS));i++)
        {
            read(fd, characters, 2);
            //check if it is in the end of a word
            if(!is_separator(characters[0]) && is_separator(characters[1]))
            {
                //increase the counter of the words that read
                counter++;

            }
            lseek(fd,-1,SEEK_CUR );
        }
    }

    //lock this area to protect it and write in sum only one thread each time
    pthread_mutex_lock(&mymutex);

    //total sum of words that each thread read
    sum=sum+counter;

    //unlock this area
    pthread_mutex_unlock(&mymutex);

    close(fd);
    //exit from the current thread
    pthread_exit (NULL);
    
}



int main(int argc, char *argv[])
{
    //initialize the array of the threads
    pthread_t threads[NTHREADS];
    //initialize the struct of the arguments that every thread must has
    info counter[NTHREADS];
    int fd, characters;

    //for the threads
    for (int i = 0; i < NTHREADS; ++i)
    {
      
        counter[i].filename=argv[1];

        
        //second info is the number of thread
        counter[i].counter_thread=i;
        //create the threads and call the correct function
        pthread_create(&threads[i], NULL, &word_counter, (void *)&(counter[i]));
            
    }
    
    //wait the threads to complete
    for (int i = 0; i < NTHREADS; ++i)
    {
        pthread_join(threads[i],NULL);
    }

    //calculate the words of the file that will write in the file (6->words:)
    int saveddata_size=(sizeof(getpid())+strlen(argv[1])+6+sizeof(sum));
    char buffer_of_saveddata[saveddata_size];
    
    not_file = opendir (buffer_of_filename);

    //if not open is a file and print the correct message
    if( not_file != NULL)
    {
            printf("\tThe %s is not a file\n",de->d_name);
            //return to the while to continue the reading of the folder
            exit(-1);
    }
    //open the out.txt for write and append and create if does not exist and check if opens
    if ((fd=open("out.txt",O_WRONLY | O_CREAT | O_APPEND,0644 ))==-1)
    {
        //Print to user the corresponding message
        perror("");
        //exiting from the program
        exit(1);

    }

    //construct the correct sentence that will write in the file
    sprintf(buffer_of_saveddata,"%d %s Words:%d\n",getpid(),argv[1],sum);

    //write the array of characters in the file
    write(fd, buffer_of_saveddata, strlen(buffer_of_saveddata)); 

    return 0;
}       

