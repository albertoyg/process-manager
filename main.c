#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linkedlist.h"

// bg, bglist, 

void killDaZombies(struct LinkedList* programList){
    int state;
    pid_t pid;
    
    while((pid = waitpid(-1, &state, WNOHANG))>0){
        if(findNodeByPID(programList, pid) == NULL){
            // child process is running
        }
        else {
            // child process is finished
            deleteNodeByPID(programList, pid);
            printf("Terminated Zombie Child [%d].\n", pid);
        }
    }
 
}

void BGlist(struct LinkedList* programList){
    // get number of processes currently in list
    int Length;
    Length = listLength(programList);
    if (Length != 0){
        printList(programList);
        printf("Total background jobs: %d\n", Length);
    } else {
        printf("No running programs.\n");
    }

}

void BGkill(struct LinkedList* programList,char * code){
    int pid = atoi(code);
    if(findNodeByPID(programList, pid) != NULL){
        deleteNodeByPID(programList, pid);
        kill(pid, SIGTERM);
        printf("Process [%d] has been killed\n", pid);
        usleep(100);
    }
    else {
        printf("Process [%d] does not exist\n", pid);
    }
}

void BGstop(struct LinkedList* programList, char * code){
    // pauses to sleeping state
    int pid = atoi(code);
    if(findNodeByPID(programList, pid) != NULL){
        kill(pid, SIGSTOP);
        printf("Process [%d] has been stopped\n", pid);
        usleep(100);
    }
    else {
        printf("Process [%d] does not exist\n", pid);
    }
}

void BGstart(struct LinkedList* programList, char * code){
    // continues process if stopped 
    int pid = atoi(code);
    if(findNodeByPID(programList, pid) != NULL){
        kill(pid, SIGCONT);
        printf("Process [%d] is resumming\n", pid);
        usleep(100);
    }
    else {
        printf("Process [%d] does not exist\n", pid);
    }
}

void BG(char **input, struct LinkedList* programList){
    char programName[70];
    // check if name is present
    if (input[0] != NULL){
        // if it is, programName = name of program

        strncpy(programName, input[0], 49);
    } else {
        // error if no name provided 
        printf("Error, please provide a function name\n");
        return;
    }

        
    pid_t pid;
    pid = fork();

    // check if fork is valid:
    if (pid < 0){
        printf("fork failed\n");
        exit(EXIT_FAILURE);
    } 

    // check if fork is child provess
     else if (pid == 0) {


        int exeStatus = 0;
        exeStatus = execvp(programName, input);

        if (exeStatus < 0){
            printf("epic failure\n");
            exit(EXIT_FAILURE);
        } 
        


    } 

    // check if fork is parent process
     else if (pid > 0) {
       //printf("parent process\n");

        // wait for childprocess
        usleep(100);

        // create path
        char path[100]="/home/user/CSC360/myassignment/";

        // concat file name
        strcat(path,programName);

        // add to linkedlist
        addNode(programList, path, pid);

        // response
        printf("added [%d] to programlist\n", pid);
    }
    return;
}

void BGstats(int * pid, char * filePath){
    // open file at given path:
    FILE *file = fopen(filePath, "r");
    if (file == NULL){
        printf("Error: Failed opening file at path %s\n", filePath);
        exit(EXIT_FAILURE);
    }

    // format the parser
    const char* parser = "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %f %f %*d %*d %*d %*d %*d %*d %*u %*u %ld";
    // init variables for stats
    double ticks = (double) sysconf(_SC_CLK_TCK);
    char comm[500]; 
    float utime;
    float stime;
    long int rss;
    char state;

    // scan for stats
    int num_fields_read = fscanf(file, parser, comm, &state, &utime, &stime, &rss);
    // error checking
    if (num_fields_read != 5) {
        printf("Error: failed to read process stats from %s\n", filePath);
        exit(EXIT_FAILURE);
    }

    // normalize the times
    float ut;
    float st;

    ut = utime/ticks;
    st =  stime/ticks;

    // output results 
    printf("\nProcess stats for process at path: [%s]\n", filePath);
    printf("------------------------------------------\n");
    printf("comm: %s\n", comm);
    printf("state: %c\n", state);
    printf("utime: %f, stime: %f\n", ut, st);
    printf("rss: %ld\n", rss);
    fclose(file);

}

void BGstatus(int * pid, char * filePath){
    FILE *file = fopen(filePath, "r");
    if (file == NULL){
        printf("Error: failed opening file at path %s\n", filePath);
        exit(EXIT_FAILURE);
    }

    char line[5000]; 
    while(fgets(line, sizeof(line), file)) {
        int volContext = 0;
        int nonVolContext = 0;
        if (sscanf(line, "voluntary_ctxt_switches: %d", &volContext) == 1){
            printf("Number of voluntary context switches: %d\n", volContext);
        }

        if (sscanf(line, "nonvoluntary_ctxt_switches: %d", &nonVolContext) == 1){
            printf("Number of involuntary context switches: %d\n", nonVolContext);
        }
    }

    fclose(file);

}


void BGstat(struct LinkedList* programList, char * code){
    // first check if the process exists
    int pid = atoi(code);
    if(findNodeByPID(programList, pid) == NULL){
        printf("Error: Process [%d] does not exist.\n", pid);
        return;
    }
    // if we are here, it does! 
    char filePath1[100];
    char filePath2[100];
    sprintf(filePath1, "/proc/%d/stat", pid);
    sprintf(filePath2, "/proc/%d/status", pid);
    BGstats(&pid, filePath1);
    BGstatus(&pid, filePath2);
    printf("------------------------------------------\n");

}
 
int main(){
    // init user 
    char input[50];

    // init list of running programs 
    struct LinkedList* programList = initList();

    // begin Pman program loop
    while (true) {
        // remove all zombie children
        killDaZombies(programList); //<- not sure why

        // print pman prompt 
        printf("Pman: > ");

        // get user input and store in 'input'
        fgets(input, 50, stdin);

        // removes white space from input string and stores in decodedInput 
        char * decodedInput = strtok(input, " \n");

        // skip if decodedinput is empty 
        if(decodedInput == NULL){
            continue;
        }

        // delcare an array of pointers 
        char * inputList[50];

        // init index
        int index = 0;

        // decodedinput is entered into inputList at index 
        inputList[index] = decodedInput;

        // inc index
        index++;

        // this loop will take every word in the inputted line and store
        while(decodedInput != NULL){
            decodedInput = strtok(NULL, " \n");
            inputList[index]=decodedInput;
            index++;
        }

        // check if we are given bg as a command. If so, run bg 
        if (strcmp("bg",inputList[0]) == 0){
            BG(inputList + 1, programList);
        } 

        // check if user wants to see list of processes
        else if (strcmp("bglist",inputList[0]) == 0){
            BGlist(programList);
        }

        // check if user wants to pause a process
        else if (strcmp("bgstop",inputList[0]) == 0){
            BGstop(programList, inputList[1]);
        }

        // check if user wants to resume a process
        else if (strcmp("bgstart",inputList[0]) == 0){
            BGstart(programList, inputList[1]);
        }

        // check if user wants to kill a process
        else if (strcmp("bgkill",inputList[0]) == 0){
            BGkill(programList, inputList[1]);
        }

        else if (strcmp("pstat",inputList[0]) == 0){
            BGstat(programList, inputList[1]);
        }

        // check if we want to shut program, if we do, shut it. 
        else if (strcmp("q", inputList[0]) == 0 || 
                 strcmp("Q", inputList[0]) == 0 || 
                 strcmp("quit", inputList[0]) == 0 || 
                 strcmp("end", inputList[0]) == 0 || 
                 strcmp("exit", inputList[0]) == 0) {
            printf("Terminating... Thanks for using PMan! \n");
            exit(0);
        }
        else {
            printf("%s: command not found\n", inputList[0]);
        }

    }

    return 0;
}