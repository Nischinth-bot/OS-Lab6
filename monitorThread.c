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
void recompileMasterList();
void updateThreadList(int tid, char ** fileList);
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
    getThreadFiles(1, (struct arg_struct *) arg, fileList);    
    while(1){
        pthread_mutex_lock(&mymutex);
        char ** temp = malloc(sizeof(char*) * MAXFILES); //assign a temp array of strings
        int currentSize = listIndices[tid]; //size of the file list right now.
        listIndices[tid] = 0;
        getThreadFiles(0,arg,temp);           // get the files into temp, but don't copy them into threadFiles variable.
        if(currentSize != listIndices[tid]) 
        {
            updateThreadList(tid, temp);
            recompileMasterList();
        }
        //printFiles(threadFiles[tid], listIndices[tid]); 
        pthread_mutex_unlock(&mymutex);
    }
    pthread_exit(NULL);
}

/**
 * Wrapper function that calls another getThreadFiles function on every directory
 * assgined to the thread.
 */
void getThreadFiles(int signal, struct arg_struct * arg, char ** fileList)
{

    int dirCt = arg->numDirs;
    int tid = arg->tid;
    if(dirCt > 0){
        int i;
        for(i = 0; i < dirCt; i ++){
            getThreadFiles2(signal, tid,arg->dirs[i],fileList);
        }
    }
}

/**
 * The actual getThreadFiles function that generates a fileList 
 * for each thread.
 */
void getThreadFiles2(int tid, int signal, char* dir, char ** fileList)
{
    int fileCount = getFileCount(dir); 
    if(listIndices[tid] + fileCount < MAXFILES/globalThreadCount) {
        if(fileCount > 0){ 
            getFileList(dir, fileList, fileCount);
            if(signal == 1) copyFileList(threadFiles[tid], fileList, fileCount, tid);    
        }
    }
    int dirCt = getDirectoryCount(dir);
    char ** dirs = malloc(sizeof(char*) * dirCt);
    getDirectoryList(dir, dirs, dirCt);
    if(dirCt > 0){
        int i;
        for(i = 0; i < dirCt; i ++){
            getThreadFiles2(tid, signal, dirs[i],fileList);
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

void updateThreadList(int tid, char ** fileList)
{
    int i;
    char ** ptr = threadFiles[tid];
    for(i = 0; i < listIndices[tid]; i ++){
        ptr[i] = fileList[i];    
    }
}


void recompileMasterList()
{
    int i,j;
    //globalFileList = malloc(sizeof(char*) * MAXFILES);
    for(i = 0; i < globalThreadCount; i ++){
        char ** threadFile = malloc(sizeof(char *) * MAXFILES/globalThreadCount);
        threadFile = threadFiles[i];
        for(j = 0; j < listIndices[i]; j ++){
            globalFileList[i] = threadFile[j];
        }
    }
}
