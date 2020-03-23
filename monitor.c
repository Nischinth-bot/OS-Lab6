#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <math.h>
#include "config.h"
#include "fileHandle.h"
#include "mainThread.h"




static void printUsage();
static void parseArgs(int argc, char * argv[], char ** target, int * threads);


/*
 * printUsage
 * prints the usage information and exits
 */
void printUsage()
{
    printf("usage: monitor -d <directory> -t <#threads>\n");
    printf("-d <directory> - directory to monitor\n");
    printf("-t <#threads> - number of threads doing the monitoring\n");
    exit(1);
}

/*
 * parseArgs
 * Parses the command line arguments setting (*target) to the target
 * directory and (*threads) to the number of threads to be created.
 * If invalid command line arguments were entered then printUsage
 * is called to print usage information and exit.
 *
 * argc - count of command line arguments
 * argv - array of command line arguments
 * target - point to a char * to be set to the target
 * threads - point to an int to be set to the number of threads
 */
void parseArgs(int argc, char * argv[], char ** target, int * threads)
{
    int i;
    if (argc != 5) printUsage();
    for (i = 1; i < argc; i+=2)
    {
        if (strcmp("-d", argv[i]) == 0)
        {
            (*target) = strdup(argv[i+1]);
        } else if (strcmp("-t", argv[i]) == 0)
        {
            (*threads) = atoi(argv[i + 1]);
        } else
            printUsage();
    }
}
/*
 * main
 * The main is very incomplete.
 * It needs to:
 * 1) figure out how to divide up the directories among the threads
 * 2) create the threads which will repeat forever monitor their directories
 * 3) repeat forever updating the display (main thread)
 */
int main (int argc, char *argv[])
{
    char * target;
    int i, nThreads;
    char ** dirs;
    int dirCt; 


    parseArgs(argc, argv, &target, &nThreads);
    dirCt = getDirectoryCount(target);
    if (nThreads > dirCt) nThreads = dirCt;   
    dirs = (char **) malloc(sizeof(char *) * dirCt);
    getDirectoryList(target, dirs, dirCt);
    /**Set up for threading**/ 
    //  globalArgs = malloc(sizeof(struct arg_struct) * nThreads);
    pthread_t threads[nThreads]; //Array of threads.
    globalThreadCount = nThreads; //make available the number of threads globally for iteration.
    int startIndex = 0;    
    int endIndex = 0;
    int eachThread = dirCt/nThreads; //Number of directories per thread.
    int dirsRemaining = dirCt % nThreads; 


    /*************************************************************/


    /**Code for dividing up the directories between threads**/
    for (i = 0; i < nThreads; i ++){
        endIndex = startIndex + eachThread - 1;
        if(i == nThreads - 1 && dirsRemaining > 0) {
            eachThread += dirsRemaining; //Accomodate remaining threads, if they exist 
            endIndex += dirsRemaining; //Change the index for iteration
        }
        char * threadDirs[MAXDIRS]; //Array of char pointers passed to each thread. 
        int j = 0;
        for(j = startIndex; j <= endIndex; j ++){
            threadDirs[j - startIndex] = dirs[j];
        }
        globalArgs[i].tid = i;
        globalArgs[i].numDirs = eachThread;
        for(j = 0; j < eachThread; j ++) globalArgs[i].dirs[j] = threadDirs[j]; //Set the directories in the argument.
        startIndex = endIndex + 1;
    }
    /*************************************************************/ 

    globalFileList= (char **) malloc(sizeof(char *) * MAXFILES);
    getAllFiles(target,0); 
    printAllFiles();
  
    
  //    int tid = getTID(globalFileList[10]);
  //  printf("%d\n", tid);
    return 0;
}


