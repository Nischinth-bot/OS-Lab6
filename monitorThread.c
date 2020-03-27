#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileHandle.h"
#include "config.h"
#include "fileHandle.h"
#include "mainThread.h"
#include "monitorThread.h"

char ** threadFiles[MAXTHREADS]; //array of char** to hold the file list of each thread.
int listIndices[MAXTHREADS]; //structure that holds the current number of files in each thread.
pthread_mutex_t mymutex =  PTHREAD_MUTEX_INITIALIZER;

/**
 * Function executed by each thread.
 * Gets an original file list, and then loops indefinitely while 
 * checking for changes in its file list.
 * @param: arg 
 */
void* threadFunction(void * arg)
{
    int tid = ((struct arg_struct *) arg) -> tid;
    char ** fileList = malloc(sizeof(char*) * MAXFILES);
    threadFiles[tid] = malloc(sizeof(char*) * MAXFILES/globalThreadCount);
    getThreadFiles((struct arg_struct *) arg, fileList);    
    while(1){
        pthread_mutex_lock(&mymutex);
        char ** temp = malloc(sizeof(char*) * MAXFILES); //assign a temp array of strings
        int currentSize = listIndices[tid]; //size of the file list right now.
        listIndices[tid] = 0;
        getThreadFiles(arg,temp);           // get the files into temp
        if(currentSize != listIndices[tid]) printf("Something changed... in %d\n", tid);
        //printFiles(threadFiles[tid], listIndices[tid]); 
        pthread_mutex_unlock(&mymutex);
    }
    pthread_exit(NULL);
}

/**
 * Wrapper function that calls another getThreadFiles function on every directory
 * assgined to the thread.
 */
void getThreadFiles(struct arg_struct * arg, char ** fileList)
{

    int dirCt = arg->numDirs;
    int tid = arg->tid;
    if(dirCt > 0){
        int i;
        for(i = 0; i < dirCt; i ++){
            getThreadFiles2(tid,arg->dirs[i],fileList);
        }
    }
}

/**
 * The actual getThreadFiles function that generates a fileList 
 * for each thread.
 */
void getThreadFiles2(int tid, char* dir, char ** fileList)
{
    int fileCount = getFileCount(dir); 
    if(listIndices[tid] + fileCount < MAXFILES/globalThreadCount) {
        if(fileCount > 0){ 
            getFileList(dir, fileList, fileCount);
            copyFileList(threadFiles[tid], fileList, fileCount, tid);    
        }
    }
    int dirCt = getDirectoryCount(dir);
    char ** dirs = malloc(sizeof(char*) * dirCt);
    getDirectoryList(dir, dirs, dirCt);
    if(dirCt > 0){
        int i;
        for(i = 0; i < dirCt; i ++){
            getThreadFiles2(tid, dirs[i],fileList);
        }
    }
}

/**
 * Copy contents of fileList2 into fileList1
 */
void copyFileList(char ** fileList1, char ** fileList2, int numFiles, int tid)
{
    int i;  
    for(i = 0; i < numFiles; i ++){
        fileList1[listIndices[tid]++] = fileList2[i];
    }
}



void printFiles(char ** fileList, int numFiles)
{
    int i;
    for(i = 0; i < numFiles; i ++){
        printf("%s\n", fileList[i]);
    }
}

