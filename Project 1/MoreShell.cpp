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
        cout << "MoreShell>";

        cin.getline(c, 50);

        char* pch = strtok(c, " ");
        

        char* arg[50];

        arg[0] = c;

        int x = 1;
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
        else if(pid == 0) {
            execvp(c, arg);
        }
        else {
            wait(NULL);
        }
    }
    return 0;
}