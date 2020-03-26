void* threadFunction(void* arg);
void getThreadFiles(struct arg_struct* arg, char ** fileList);
void getThreadFiles2(int tid, char* dir, char ** fileList);
void copyFileList(char ** fileList1, char ** fileList2, int numFiles, int tid);
void printFiles(char ** fileList, int numFiles);
