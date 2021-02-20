#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    pid_t pid;
    pid = fork();

    if(pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0) {
        execl("/media/sf_CS4440/Project 1/MyCompress", "MyCompress", argv[1], argv[2], NULL);
    }
    else {
        wait(NULL);
        printf("Child Complete\n");
    }


    return 0;
}