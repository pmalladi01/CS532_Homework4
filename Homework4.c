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
