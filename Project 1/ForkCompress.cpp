/*
    Creates a child process from the parent process which
    calls the "MyCompress" file and compresses a input and
    output file.
*/

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {

    // Forks a process from the parent
    pid_t pid;
    pid = fork();

    // The child process executes the program "MyCompress"
    // while the parent waits for the program to be done executing.
    if(pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0) {
        execl("MyCompress", "MyCompress", argv[1], argv[2], NULL);
    }
    else {
        wait(NULL);
        printf("Child Complete\n");
    }


    return 0;
}