#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "fileHandle.h"
#include "mainThread.h"

/**
 * Recursive function that traverses all subdirectories 
 * of the given target and generates a comprehensive file list.
 * The search is performed up to a depth of MAXDEPTH.
 * @param: target - The target directory.
 * @param: depth - the depth of the current iteration.
 * @return: list of all files in all subdirectories
 */
void getAllFiles(char* target, int depth)
{
    if(depth  > MAXDEPTH) return;
    int fileCount = getFileCount(target); 
    int dirCount = getDirectoryCount(target);
    int i;
    if(fileCount > 0){
        char ** fileList = (char **) malloc(sizeof(char*)* fileCount); 
        getFileList(target,fileList,fileCount);
        for(i = 0; i < fileCount; i ++){
            globalFileList[globalFileIndex++] = fileList[i]; //collect the files into the global variable
        }
    }
    if(dirCount > 0){
        char ** dirList = (char **) malloc(sizeof(char *) * dirCount);
        getDirectoryList(target,dirList,dirCount);
        for(i = 0; i < dirCount; i ++){
            getAllFiles(dirList[i], depth + 1);
        }
    }
}

/**
 * Helper method to print the global file list.
 */
void printAllFiles()
{
    int i;
    for(i = 0 ; i < globalFileIndex; i ++){
        char * file  = globalFileList[i];
        int tid = getTID(file);
        printf("%d %s\n", tid, globalFileList[i]);
    }
}

/**
 * Helper method to print directories belonging to a thread.
 */
void printDirs(struct arg_struct arg){
    int i;
    for(i = 0; i < arg.numDirs; i ++) printf("%s\n", arg.dirs[i]);
}
/**
 * Recursive function that traces the TID of a given file.
 * @param: file - a pointer to a file.
 * @param: nThreads - number of active threads
 * @return: TID of the thred that is monitoring the file upon discovery, else -1.
 */
int getTID(char * file)
{
    int i;
    for(i = 0; i < globalThreadCount; i ++){
        struct arg_struct arg = globalArgs[i];
        int j;
        for(j = 0; j < arg.numDirs; j ++){
            if(fileInDir(file,arg.dirs[j])) return arg.tid;
        }
    }
    return -1;

}
/**
 * Recursive function checks if a file is present in a directory and all its subdirectories.
 * @param: file - a pointer to a file.
 * @param: dir - a pointer to a directory.
 * @return: 1 upon success, 0 upon failure.
 */
int fileInDir(char * file, char* dir)
{   
    int fileCount = getFileCount(dir);
    int dirCount = getDirectoryCount(dir);
    if(fileCount > 0){
        char ** fileList = malloc(sizeof(char*) * fileCount);
        getFileList(dir,fileList,fileCount);
        int j;
        for(j = 0; j < fileCount; j++){
            if(strcmp(fileList[j],file) == 0) return 1;
        }
    } 
    if(dirCount > 0){
        char ** dirList = malloc(sizeof(char*) * dirCount);
        getDirectoryList(dir,dirList,dirCount);
        int i;
        for(i = 0; i < dirCount; i ++){
            return fileInDir(file,dirList[i]);
        }
    }
    return 0;
}

