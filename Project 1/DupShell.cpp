#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main(){
    char c[50];

    
    while(strcmp(c,"exit")){
        cout << "DupShell>";
        cin.getline(c,50);


        char *p = strtok(c, " ");
        char *arg[50];
        arg[0] = c;
        
        char *beforePipe[50];

        int x =1;
        while(p != NULL){
            p = strtok(NULL, " |");
            arg[x] = p;
            x++;
        }
        arg[x]= NULL;

        
        for(int y=0; y<x-2; y++){
            beforePipe[y] = arg[y];
        }
        beforePipe[x-1] = NULL;

        
    
    
    int pipefds[2];
    pid_t pid;

    if(pipe(pipefds) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

        pid = fork();

        fflush(stdout);

        if(pid < 0){
            cout << "ERROR" << endl;
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            dup2(pipefds[1], STDOUT_FILENO);
            close(pipefds[0]);
            execvp(c,beforePipe);
            exit(EXIT_SUCCESS);
        }
        else{
       
            dup2(pipefds[0], STDIN_FILENO);
            close(pipefds[1]);
            execlp(arg[x-2], arg[x-2], NULL);
            exit(EXIT_SUCCESS);
        }

    }


    return 0;
}