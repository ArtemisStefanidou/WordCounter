#include "wordcounter.h"



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
        pid = fork();
        char *argv[];
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}

        argv[0]=de->d_name;

        if (pid == 0)
		{
           execvp("wordcounter.c",argv)
        }else{
            wait(NULL);
        }
    }

   

    closedir(pDir);  
    return 0;
}

