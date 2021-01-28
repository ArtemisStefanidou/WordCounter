#include "wordcounter.h"
#include <pthread.h>

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

    //a array to save two by two the characters that read by the read function
    char characters[2];

    int fd;
    if ((fd=open(args->filename, O_RDONLY ))==-1)
    {
        //Print to user the corresponding message
        perror("");
        //return to the while to continue the reading of the folder
        exit(1);

    }

    //the number of the thread
    int limit=args->counter_thread;

    //counter for words that read a thread
    int counter=0;
    int i;
    
    //calculate the size of the file to do right the sharing for threads
    long filesize=lseek(fd,0,SEEK_END);

    //set again the pointer of file in the beggining of the file
    lseek(fd,0,SEEK_SET);

    //give the correct offset of file descriptor for each thread
    lseek(fd, limit * (filesize/NTHREADS), SEEK_SET );

    //check if is the last thread to read correctly the lasts characters in buffer
    if(limit==NTHREADS-1 && filesize%NTHREADS!=0)
    {

        //define the piece that will read the last thread
        for(int i=limit * (filesize/NTHREADS);i<((limit+1)*(filesize/NTHREADS))+(filesize % NTHREADS)-1;i++)
        {

            //read two by two characters to do the comparison and find if something is word
            read(fd, characters, 2);

            //check if it is in the end of a word
            if(!is_separator(characters[0]) && is_separator(characters[1]))
            {
                
                //increase the counter of the words that read
                counter++;

            }

            /*
            Set the pointer of the file one character back to compare all 
            the right possible combinations to see if something is a word.
            file-> abcd  what I want to compare each time-> ab bc cd ....
            */
            lseek(fd,-1,SEEK_CUR );
        }
    }
    else
    {
        //define the piece that will read the last thread
        for(int i=limit * (filesize/NTHREADS);i<((limit+1)*(filesize/NTHREADS));i++)
        {
            
            //read two by two characters to do the comparison and find if something is word
            read(fd, characters, 2);

            //check if it is in the end of a word
            if(!is_separator(characters[0]) && is_separator(characters[1]))
            {
                //increase the counter of the words that read
                counter++;

            }

            /*
            Set the pointer of the file one character back to compare all 
            the right possible combinations to see if something is a word.
            file-> abcd  what I want to compare each time-> ab bc cd ....
            */
            lseek(fd,-1,SEEK_CUR );
        }
    }

    //lock this area to protect it and write in sum only one thread each time
    pthread_mutex_lock(&mymutex);

    //total sum of words that each thread read
    sum=sum+counter;

    //unlock this area
    pthread_mutex_unlock(&mymutex);

    //close correct the folder because when i recall this function i open it again
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
      
        //first info is the filename of the file that thread must open
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

    //calculate the words of the file that will write in the file (6->words:) + , , +\n=9
    int saveddata_size=(sizeof(getpid())+strlen(argv[1])+9+sizeof(sum));
    char buffer_of_saveddata[saveddata_size];
    
    //open the out.txt for write and append and create if does not exist and check if opens
    if ((fd=open("output.txt",O_WRONLY | O_CREAT | O_APPEND,0644 ))==-1)
    {
        //Print to user the corresponding message
        perror("");
        //exiting from the program
        exit(1);

    }

    //construct the correct sentence that will write in the file
    sprintf(buffer_of_saveddata,"%d, %s, Words:%d\n",getpid(),argv[1],sum);

    //write the array of characters in the file
    write(fd, buffer_of_saveddata, strlen(buffer_of_saveddata)); 

    return 0;
}       

