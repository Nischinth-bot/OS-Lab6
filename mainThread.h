struct arg_struct{ 
    int tid;
    int numDirs;
    char *dirs[MAXDIRS];
};
char ** globalFileList;
char ** globalDirs;
struct arg_struct  globalArgs[100];
int globalFileIndex;
int globalThreadCount;
void getAllFiles(char * target, int depth);
int getTID(char * file);
void fileInDir(char * file, char* dir);
void printDirs(char* dir);
void printAllFiles();

