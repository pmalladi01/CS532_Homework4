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

void show()
{
    char statuss[4][20] = {"Waiting", "Running", "Completed", "Completed"};
    struct Node *headptr1 = process_list;
    struct Node *wait_ptr = NULL;
    int runcount = 0;
    printf("jobid\t\tcommand\t\tstatus\n");
    while (headptr1 != NULL)
    {
        if (headptr1->status == RUNNING)
            printf("%d\t\t%s  %s\t\tRUNNING\n", headptr1->jobid, headptr1->arg1, headptr1->arg2);
        else if (headptr1->status == WAITING)
            printf("%d\t\t%s  %s\t\tWAITING\n", headptr1->jobid, headptr1->arg1, headptr1->arg2);
        headptr1 = headptr1->next;
    }
}
    
void showall()
{
    char statuss[4][20] = {"Waiting", "Running", "Completed", "Completed"};
    struct Node *headptr1 = process_list;
    struct Node *wait_ptr = NULL;
    int runcount = 0;
    printf("Job id\tCommand\tStarttime\tEndtime\tStatus\n");
    while (headptr1 != NULL)
    {
        if (headptr1->status == RUNIT)
            printf("%d\t%s %s\t%s\t%s\tSUCCESS\n", headptr1->jobid, headptr1->arg1, headptr1->arg2, headptr1->starttime, headptr1->endtime);
        headptr1 = headptr1->next;
    }
}
    
void append(char **args)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *last = process_list;
    new_node->args1 = args;
    strcpy(new_node->arg1, args[1]);
    strcpy(new_node->arg2, args[2]);
    new_node->status = WAITING;
    new_node->next = NULL;
    new_node->jobid = mxx++;
    if (process_list == NULL)
    {
        process_list = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;
}

void parse(char *line, char **argv)
{

    while (*line != '\0')
    {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';

        *argv++ = line;
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++;
    }
    *argv = '\0';
}


void execute(char **argv, struct Node *ptr)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    { 
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        int fdin, fdout;
        char buff[12];
        snprintf(buff, 12, "%d.err", getpid());
        if ((fdin = open(buff, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1)
        {
            printf("Error opening file stderr.txt for error\n");
        }
        char buff1[12];
        snprintf(buff1, 12, "%d.out", getpid());
        if ((fdout = open(buff1, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1)
        {
            printf("Error opening file stdout.txt for output\n");
            exit(-1);
        }

        dup2(fdin, 3);
        dup2(fdout, 1);
        if (execvp(*(argv + 1), argv) < 0)
        {
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        ptr->prid = pid;
    }
}

void scheduler(int n)
{
    time_t t;
    int status;

    struct Node *headptr = process_list, *headptr1 = process_list;
    struct Node *wait_ptr = NULL;
    int runcount = 0;
    while (headptr1 != NULL)
    {
        if (headptr1->status == RUNIT)
        {

            runcount--;
        }
        if (headptr1->status == RUNNING)
        {
            runcount++;
        }
        if (headptr1->status == RUNNING)
        {
            int pid = headptr1->prid;
            if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
            {
                int www = 1;
            }
            else if (pid == 0)
            {
                time(&t);

                sleep(1);
            }
            else
            {
                if (WIFEXITED(status))
                {
                    headptr1->status = RUNIT;
                    time(&t);
                    strcpy(headptr->endtime, ctime(&t));
                }
                else
                {
                    headptr1->status = RUNIT;
                    time(&t);
                    strcpy(headptr->endtime, ctime(&t));
                }
            }
            if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
                perror("wait() error");
            else if (pid == 0)
            {
                time(&t);

                sleep(1);
            }
            else
            {
                if (WIFEXITED(status))
                {
                    headptr1->status = RUNIT;
                    time(&t);
                    strcpy(headptr->endtime, ctime(&t));
                }
            }
        }
        headptr1 = headptr1->next;
    }

    while (headptr != NULL)
    {

        if (headptr->status == WAITING && runcount < n)
        {
            headptr->status = RUNNING;

            time(&t);
            strcpy(headptr->starttime, ctime(&t));
            execute(headptr->args1, headptr);
        }

        headptr = headptr->next;
    }
  }
}
