/*
    Takes in input and executes the terminal command including arguments if it is valid.
    The result is then printed out.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {
    char c[50];
    pid_t pid;

    // Checks if the input is 'Exit'
    // If it is then it will stop the program
    // If it isn't then it will continue getting input
    while(strcmp(c, "exit")) {
        cout << "MoreShell>";

        cin.getline(c, 50);

        // Seperates the input by spaces to get both the command
        // and the arguments
        char* pch = strtok(c, " ");
        

        char* arg[50];

        arg[0] = c;

        int x = 1;
        // Continues to splits the input line by spaces to get all the arguments
        while(pch != NULL) {
            pch = strtok(NULL, " ");
            arg[x] = pch;
            x++;
        }

        arg[x] = NULL;

        pid = fork();

        fflush(stdout);

        if(pid < 0) {
            cout << "ERROR";
        }
        // Child process executes the command with it's arguments.
        else if(pid == 0) {
            execvp(c, arg);
        }
        else {
            wait(NULL);
        }
    }
    return 0;
}