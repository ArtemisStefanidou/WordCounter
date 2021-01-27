#include "wordcounter.h"
#include <signal.h> 
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>


//Display a message to the user and will continue to run 
void sig_Handler(int sig_num) 
{ 
    printf("Ignoring signal: %d", sig_num); 
}

//Check if a file-char array contains a not ASCII character
int isnot_ascii(int filesize,char *buffer)
{
    int i;
    for(i=0;i<filesize;i++)
    {
        //ASCII has from 0 to 127 
        if((int)buffer[i]>127 || (int)buffer[i]<0 ){
            printf("\tThe file is not in ASCII\n");
            return 1;//true
        }
            
    }
    return 0;//false
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    info counter[NTHREADS];
    DIR *pDir;
    DIR *not_file;
    struct dirent *de;  
    char* filename;
    size_t filename_l;
    pid_t pid;
    int count_for_fork=0;

    //Check if user gives correct arguments from terminal
    if(argc>2)
    {
        printf("\tOptionally you have to give one directory.If not given will be listed in the current directory\n");
        exit(1);
    }

    if(argc == 1)
    {
        //Get current directory if user don't give a second argument from terminal
        argv[1]=".";

    }

    //open the directory given from user
    pDir = opendir (argv[1]);

    //Check if the directory opens 
    if (pDir == NULL) 
    {
        //Print to user the corresponding message
        perror("");

        //exit from the program
        exit(1);

    }
        
    int fd, characters;
    
    //read one per one the contents of the folder
    while ((de = readdir(pDir)) != NULL) 
    {
        //get the name of the file that read
        filename=de->d_name; 

        //calculate the size of the path for the file that read
        int filename_size=strlen(argv[1])+1+strlen(filename);

        //create a char array for file path with the correct size
        char buffer_of_filename[filename_size];
                
        //construct the correct path for the file and save it in the char array 
        sprintf(buffer_of_filename,"%s/%s",argv[1],filename);
            
        //open the content of the array us directory 
        not_file = opendir (buffer_of_filename);

        //if not open is a file and print the correct message
        if( not_file != NULL)
        {
            printf("\tThe %s is not a file\n",de->d_name);
            //return to the while to continue the reading of the folder
            continue;
        }

        //open for read only the file and check if opens
        if ((fd=open(buffer_of_filename, O_RDONLY ))==-1)
        {
            //Print to user the corresponding message
            perror("");
            //return to the while to continue the reading of the folder
            continue;

        }

       
        //find the size of the file
        long filesize=lseek(fd, 0, SEEK_END);
                
        //check if file is empty and print the correct message  
        if( filesize ==0 )
        {
            printf("\tThe file is empty.\n");
            //return to the while to continue the reading of the folder
            continue;
        }

            
        //a char array for saving the character that read from the file
        char buffer[filesize];
                
        //set again the pointer of the file in the beginning to start the reading
        lseek(fd, 0, SEEK_SET);

        //read the file and save the characters int the char array
        characters=read(fd, buffer, filesize);

        //check if is in ASCII
        if(isnot_ascii(filesize,buffer))
        {
            //return to the while to continue the reading of the folder
            continue;
        }
        
        

        count_for_fork++;
        //create child process
        pid = fork();
        
        //check if created child process
		if (pid == -1)
		{
            //Print to user the corresponding message
			perror("");
            //exiting from the program
			exit(1);

		}
        char fd_buffer[sizeof(fd)];

        sprintf(fd_buffer,"%d",fd);
        //creates the correct arguments array to call exec
        char *args[]={"./wordcounter2",buffer_of_filename,NULL}; 

        //for the parent
        if (pid == 0)
		{
            //Ignore SIGTERM and SIGINT signals
            signal(SIGINT, sig_Handler);
			signal(SIGTERM,sig_Handler);

           
            
           
        }else{
            
            //close the file
            close(fd);

            //go to the program that is the first element of the array
            execv(args[0],args);
        }
    }
    printf("%d",count_for_fork);
    for(int i=0;i<count_for_fork;i++)
    {
        //Wait for childern 
        wait(NULL); 
    }
    //close correctly the folder
    closedir(pDir);  

    //print a message to user to understand where is the output tha written by the program
    printf("\t.....Only files in Ascii is in the file out.txt.....\n");

    exit(0);
}

