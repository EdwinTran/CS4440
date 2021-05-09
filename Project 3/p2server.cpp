#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sys/wait.h>


using namespace std;

int bytesReceived = 0;

void handleConnection(void **servSock) {
    int sock = *((int*)servSock);
    struct sockaddr_in clientAddr;
    int clientSocket;
    char buffer[10000];

    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);

    }

    memset(buffer, 0, 10000);

    bytesReceived = recv(clientSocket, buffer, 10000, 0);

    int stringLen = sizeof(string(buffer, 0, bytesReceived));

    cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

    int pipefds[2];
    pid_t pid;

    if(pipe(pipefds) == -1) {
        perror("ERROR Pipe cannot be created");
        exit(0);
    }

    char* arg1[2];
    arg1[0] = buffer;
    arg1[1] = NULL;

    pid = fork();

    fflush(stdout);

    if(pid < 0) {
        printf("ERROR fork\n");
        exit(0);
    }
    else if(pid == 0) {
        dup2(clientSocket, STDOUT_FILENO);
        close(pipefds[0]);
        execvp(buffer, arg1);
    }
    else {
        wait(NULL);
    }
    close(clientSocket);
}

int main(int argc, char* argv[]) {
    int sock, clientSocket, port;
    struct sockaddr_in hint, clientAddr;
    int numOfThreads = 20;
    pthread_t threads[numOfThreads];

    if(argc != 2) {
        fprintf(stderr, "ERROR please enter a port #\n");
        exit(0);
    }

    port = atoi(argv[1]);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1) {
        fprintf(stderr, "ERROR cannot create socket\n");
        exit(0);
    }

    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_port = port;

    if(bind(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot bind\n");
        exit(0);
    }

    listen(sock, 5);


    while(true) {
        
        for(int i = 0; i < numOfThreads; i++) {
            pthread_create(&threads[i], NULL, (void *(*)(void *))handleConnection, &sock);
        }

        for(int i = 0; i < numOfThreads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    return 0;
}