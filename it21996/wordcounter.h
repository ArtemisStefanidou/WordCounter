#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#define NTHREADS 4

//common includes for both main
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>


typedef struct infoR info;
struct infoR
{
    char *filename;//the file that thread must read
    int counter_thread;//the number of the thread
};

//check if a character is a separator for words
int is_separator(char );

//calculate the words from a file
void *word_counter(void *);

//check if a character is not in ASCII 
int isnot_ascii(int ,char *);

//to handle the sigint sigterm
void sig_Handler(int ) ;


#endif