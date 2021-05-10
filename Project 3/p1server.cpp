#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>


using namespace std;

int bytesReceived = 0;
//Function that handles the connection between the client and server
void handleConnection(void **servSock) {
    int sock = *((int*)servSock);
    //Creates the client address
    struct sockaddr_in clientAddr;
    int clientSocket;
    char buffer[4096];

    socklen_t clientAddrLen = sizeof(clientAddr);
    //Accepts a connection between the client and sock
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);
    }

    memset(buffer, 0, 4096);
    //read the data between the coonection
    bytesReceived = recv(clientSocket, buffer, 4096, 0);

    int stringLen = sizeof(string(buffer, 0, bytesReceived));
    //Prints out what is received from the client
    cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

    int n = bytesReceived;
    //Reverses the string
    for(int i = 0; i <= bytesReceived / 2; i++) {
        swap(buffer[i], buffer[n]);
        n--;
    }
    //sends the reverse string
    send(clientSocket, buffer, bytesReceived + 1, 0);
    //closes the socket
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
    //Convert the port to a int
    port = atoi(argv[1]);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1) {
        fprintf(stderr, "ERROR cannot create socket\n");
        exit(0);
    }

    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_port = port;
    //Binds the socket to the socket address
    if(bind(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot bind\n");
        exit(0);
    }
    //Determines if a socket can be connected to
    listen(sock, 5);

    //Initializing and joining the threads
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