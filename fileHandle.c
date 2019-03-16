#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include "config.h"
#include "fileHandle.h"

static void buildPath(char * dest, char * dir, char * file, int size);

/*
 * getDirectoryCount
 * Returns a count of the number of directories that are immediate
 * subdirectories of target. Does not include the . and .. directories
 * in the count.
 * 
 * target - must be the path (absolute or relative) to a directory
 */
int getDirectoryCount(char * target)
{
   int dirCt = 0;
   struct dirent *de;  // Pointer to directory entry 
   DIR * subDr;
  
   // opendir() returns a pointer of DIR type.  
   DIR *dr = opendir(target); 
  
   assert(dr != NULL);
  
   char buff[MAXPATH];
   while ((de = readdir(dr)) != NULL) 
   {
      if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
      {
         buildPath(buff, target, de->d_name, MAXPATH);
         subDr = opendir(buff);
         if (subDr == NULL) continue;
         dirCt++;
         closedir(subDr);
      }
   }
   closedir(dr);     
   return dirCt; 
}

/*
 * getDirectoryList
 * Returns a list of the irectories that are immediate
 * subdirectories of target. Does not include the . and .. directories
 * in the count. 
 * 
 * target - must be the path (absolute or relative) to a directory
 * dirs - array of pointers to be initialized to the names of the list
 * dirCnt - size of the dirs array
 */
void getDirectoryList(char * target, char ** dirs, int dirCnt)
{
   struct dirent *de;  // Pointer for directory entry 
   DIR * subDr;
  
   // opendir() returns a pointer of DIR type.  
   DIR *dr = opendir(target); 
   assert(dr != NULL);
  
   int i = 0;
   char buff[MAXPATH];
   while ((de = readdir(dr)) != NULL) 
   {
      if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
      {
         buildPath(buff, target, de->d_name, MAXPATH);
         subDr = opendir(buff);
         if (subDr == NULL) continue;
         assert(i < dirCnt);
         dirs[i] = strdup(buff);
         i++;
         closedir(subDr);
      }
   }
   closedir(dr);     
}

/*
 * getFileCount
 * Returns a count of the number of files (that are not directories) 
 * that are in the target directory.
 * 
 * target - must be the path (absolute or relative) to a directory
 */
int getFileCount(char * target)
{
   int fileCt = 0;
   struct dirent *de;  // Pointer for directory entry 
   DIR * subDr;
  
   // opendir() returns a pointer of DIR type.  
   DIR *dr = opendir(target); 
  
   assert(dr != NULL);
  
   char buff[MAXPATH];
   while ((de = readdir(dr)) != NULL) 
   {
      if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
      {
         buildPath(buff, target, de->d_name, MAXPATH);
         //try to open it as a directory
         //if that fails, it is a file
         subDr = opendir(buff);
         if (subDr == NULL) 
         {
            fileCt++;
         } else
            closedir(subDr);
      }
   }
   closedir(dr);     

   return fileCt; 
}

/*
 * getFileList
 * Returns a list of the files (that are not directories) 
 * that are in the target directory.
 * 
 * target - must be the path (absolute or relative) to a directory
 * files - array of pointers to hold the file names
 * fileCnt - size of the files array
 */
void getFileList(char * target, char ** files, int fileCnt)
{
   struct dirent *de;  // Pointer for directory entry 
   DIR * subDr;
  
   // opendir() returns a pointer of DIR type.  
   DIR *dr = opendir(target); 
   assert(dr != NULL);
  
   int i = 0;
   char buff[MAXPATH];
   while ((de = readdir(dr)) != NULL) 
   {
      if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
      {
         buildPath(buff, target, de->d_name, MAXPATH);
         subDr = opendir(buff);
         if (subDr == NULL) 
         {
            assert(i < fileCnt);
            files[i] = strdup(buff);
            i++;
         } else
            closedir(subDr);
      }
   }
   closedir(dr);     
}

/*
 * getFileSize
 * Returns the size of a file in bytes
 *
 * file - path (relative or absolute) to the file
 */
int getFileSize(char * file)
{
   struct stat st;
   stat(file, &st);
   return st.st_size;
}

/*
 * buildPath
 * Build a path by concatenating "dir", "/", and "file
 * Path is stored in the character array dest
 *
 * dest - array of characters to hold the path
 * size - size of the dest array
 * dir - directory portion of path
 * file - file portion of path
 */
void buildPath(char * dest, char * dir, char * file, int size) {
   int len = strlen(dir);
   strncpy(dest, dir, size);
   strncpy(&dest[len], "/", size - len);
   strncpy(&dest[len + 1], file, size - len - 1);
}
