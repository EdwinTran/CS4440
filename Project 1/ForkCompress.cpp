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
        execl("/media/sf_CS4440/Project 1/MyCompress", "./MyCompress", "test.txt", "compressed.txt", NULL);
    }
    else {
        wait(NULL);
        printf("Child Complete\n");
    }


    return 0;
}