#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main() {
    pid_t pid;
    pid = fork();

    if(pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0) {
        string fileInput;
        cout << "Enter Input file";
        cin >> fileInput;
        string fileOutput;
        cout << "Enter output file";
        cin >> fileOutput;

        execl("./MyCompress", "./MyCompress", fileInput, fileOutput, NULL);
    }
    else {
        wait(NULL);
        printf("Child Complete\n");
    }


    return 0;
}