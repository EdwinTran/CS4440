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
        else if(pid == 0) {
            execvp(c, arg);
        }
        else {
            wait(NULL);
        }
    }
    return 0;
}