#include "wordcounter.h"

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
    DIR *pDir;
    DIR *not_file;
    struct dirent *de;  
    char* filename;
    size_t filename_l;
    pid_t pid;

    if(argc!=2)
    {
        printf("Υou have to give also a directory\n");
        return 0;
    }

    pDir = opendir (argv[1]);
    if (pDir == NULL) 
    {
        perror("opendir");
	    exit(1);
    }

    int fd, characters;

    while ((de = readdir(pDir)) != NULL) 
    {
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
        
        char *argv[2];

        pid = fork();
        
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
        char *args[]={"./wordcounter",buffer_of_filename,NULL}; 

        
        if (pid == 0)
		{
            printf("OLE\n");
           execv(args[0],args);
        }else{
            wait(NULL);
        }
    }

   
    close(fd);
    closedir(pDir);  
    return 0;
}

