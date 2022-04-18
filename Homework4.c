#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
struct Node
{
    int jobid;
    int status;
    struct Node *next;
    int prid;
    char starttime[100];
    char arg1[100];
    char arg2[100];
    char endtime[100];
    char **args1;
};

static struct Node *process_list = NULL;
#define WAITING 0
#define RUNNING 1
#define COMPLETED 2
#define RUNIT 3
int mxx = 1;

void main(int *argc, int **args)
{
    process_list = NULL;
    char line[1024];
    char *argv[100];
    int n = atoi(args[1]);
    while (1)
    {
        scheduler(n);             
        printf("Enter Command> "); 
        gets(line);               
        printf("\n");
        parse(line, argv);              
        if (strcmp(argv[0], "exit") == 0) 
            exit(0);
        if (strcmp(argv[0], "submit") == 0)
            append(argv);
        scheduler(n); 
        if (strcmp(argv[0], "showjobs") == 0)
            show();
        if (strcmp(argv[0], "submithistory") == 0)
            showall();
    }
