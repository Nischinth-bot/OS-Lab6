//These values are used by the modify program and the monitor
//program.  You aren't required to use them, but the can allow
//you to declare statically sized arrays instead of having to
//determine them dynamically

//maximum number of files across all of the subdirectories
#define MAXFILES 30
#define MAXTHREADS 100
//maximum number of subdirectories of the target directory
#define MAXDIRS 30

//maximum depth of any file
//depth 1: target/dir1/file
//depth 2: target/dir1/dir1/file
#define MAXDEPTH 5

//maximum length of a path
#define MAXPATH 200
