#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <math.h>

#include "fileHandle.h"

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
   for (i = 0; i < dirCt; i++) printf("dirs[%d] = %s\n", i, dirs[i]); 

   return 0;
}


