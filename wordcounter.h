#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h> 
#include <dirent.h>

#define NTHREADS 4

typedef struct infoR info;
struct infoR
{
    char *buf;//the text that this thread must read
    int counter_thread;//the number of the thread
    char next_char;
};

//check if a character is a separator for words
int is_separator(char );

//calculate the words from a file
void *word_counter(void *);

//check if a character is not in ASCII 
int isnot_ascii(int ,char *);

#endif