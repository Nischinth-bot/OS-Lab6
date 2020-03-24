#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "fileHandle.h"
#include "mainThread.h"
int signal = 0;
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
            numFiles ++;
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

    printf("TID\t\tSize\t\tFile\n"); 
    for(i = 0 ; i < globalFileIndex; i ++){
        char * file1  = globalFileList[i];
        int tid1 = getTID(file1);
        int size1 = getFileSize(file1);
        printf("[%d]\t\t%d\t\t%s\n", tid1, size1, file1);
    }
    printf("\n\n");
}

/**
 * Helper method to print directories belonging to a thread.
 */
void printDirs(char * dir){
    int count = getDirectoryCount(dir);
    if(count == 0) return;
    char ** dirs = malloc(sizeof(char*) * count);
    getDirectoryList(dir,dirs,count);
    int i;
    for(i = 0; i < count; i ++) { printf("%s\n", dirs[i]);}
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
            fileInDir(file,arg.dirs[j]);
            if(signal == 1){
                signal = 0;
                return arg.tid;
            }
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
void fileInDir(char * file, char* dir)
{
    int fileCount = getFileCount(dir);
    int dirCount = getDirectoryCount(dir);
    if(fileCount > 0){
        char ** fileList = malloc(sizeof(char*) * fileCount);
        getFileList(dir,fileList,fileCount);
        int j;
        for(j = 0; j < fileCount; j++){
            if(strcmp(fileList[j],file) == 0){
                signal = 1;
                return;
            }
        }
    } 
    if(dirCount > 0){
        char ** dirList = malloc(sizeof(char*) * dirCount);
        getDirectoryList(dir,dirList,dirCount);
        int i;
        for(i = 0; i < dirCount; i ++){
            fileInDir(file,dirList[i]);
        }
    }
    return;
}

/**
 * Comparison function for inbuilt sort.
 * @param: file1 
 * @param: file2 
 * @return: 1 if size of file1 > file2, 0 otherwise
 */
int comp(char* file1, char* file2)
{ 
    int size1 = getFileSize(file1);
    int size2 = getFileSize(file2);
    if(size1 < size2) return 1;
    if(size1 > size2) return -1;    
    return 0;
}

/**
 * Swap function for bubble sort. 
 */
void swap(char** file1, char** file2)
{
    char* temp = *file1;
    *file1 = *file2;
    *file2 = temp;
}

/**
 * Calls the qsort function in stdlib.
 */
void sortFileList()
{
    int finished = 0; 
    while(!finished){
        int i;
        int didSwap = 0;
        for(i = 0; i < numFiles - 1; i ++){              
            char ** file1 = &globalFileList[i];
            char ** file2 = &globalFileList[i + 1];
            if(comp(*file1,*file2) == 1) {
                swap(file1,file2);
                didSwap = 1;           
            } 
            
        finished = !didSwap;
        }
    }
}
