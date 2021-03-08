#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
    Takes in input and executes the terminal command if it is valid.
    The result is then printed out.
*/

using namespace std;

int main() {
    char c[50];
    pid_t pid;

    // Checks if the input is 'Exit'
    // If it is then it will stop the program
    // If it isn't then it will continue getting input
    while(strcmp(c, "exit")) {
        cout << "MiniShell>";

        cin.getline(c, 50);

        char* arg[2];
        arg[0] = c;
        arg[1] = NULL;

        pid = fork();

        fflush(stdout);
        if(pid < 0) {
            cout << "ERROR";
        }
        // The child process executes the command.
        else if(pid == 0) {
            execvp(c, arg);
        }
        else {
            wait(NULL);
        }
    }
    return 0;
}