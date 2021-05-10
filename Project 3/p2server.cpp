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

    //Accepts the client socket that is connecting.
    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);

    }

    //Clears the buffer
    memset(buffer, 0, 10000);

    //Receives the string and saves it into buffer.
    bytesReceived = recv(clientSocket, buffer, 10000, 0);

    int stringLen = sizeof(string(buffer, 0, bytesReceived));

    //Prints out the received string from the client.
    cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

    //Creates a pipe and an id for forking.
    int pipefds[2];
    pid_t pid;

    //Checks to see if the pipe can be created.
    if(pipe(pipefds) == -1) {
        perror("ERROR Pipe cannot be created");
        exit(0);
    }

    //Creates an char array pointer to hold the command.
    char* arg1[2];
    arg1[0] = buffer;
    arg1[1] = NULL;

    //Forks the process
    pid = fork();

    fflush(stdout);

    //Exits the program if the program has trouble forking.
    if(pid < 0) {
        printf("ERROR fork\n");
        exit(0);
    }
    //Checks to see if it is the child process
    else if(pid == 0) {
        //Redirects the output into the client.
        dup2(clientSocket, STDOUT_FILENO);
        close(pipefds[0]);
        //Executes the system command
        execvp(buffer, arg1);
    }
    //The parent process waits
    else {
        wait(NULL);
    }

    //Closes the client socket after it is done.
    close(clientSocket);
}

int main(int argc, char* argv[]) {
    int sock, clientSocket, port;
    struct sockaddr_in hint, clientAddr;
    int numOfThreads = 20;
    pthread_t threads[numOfThreads];

    //Checks to see if there are enough arguments provided on the command line
    if(argc != 2) {
        fprintf(stderr, "ERROR please enter a port #\n");
        exit(0);
    }

    //Saves port number
    port = atoi(argv[1]);

    //Creates a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //Checks to see if the socket can be created.
    if(sock == -1) {
        fprintf(stderr, "ERROR cannot create socket\n");
        exit(0);
    }

    //Edits the server socket address and provides it with a random ip address
    //and the port saved.
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_port = port;

    //Binds the server address to the socket.
    if(bind(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot bind\n");
        exit(0);
    }

    listen(sock, 5);

    //Continuously creates threads which handle the client connections.
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