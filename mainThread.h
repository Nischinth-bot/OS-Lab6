struct arg_struct{ 
    int tid;
    int numDirs;
    char *dirs[MAXDIRS];
};
char ** globalFileList;
char ** globalDirs;
struct arg_struct  globalArgs[100];
int numFiles;
int globalFileIndex;
int globalThreadCount;
void getAllFiles(char * target, int depth);
int getTID(char * file);
void fileInDir(char * file, char* dir);
void printDirs(char* dir);
void printAllFiles();
void sortFileList();
int comp(char * file1, char* file2);
void swap(char** file1, char** file2);
