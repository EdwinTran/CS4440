#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fstream>


using namespace std;

int bytesReceived = 0;
int MAX_SIZE = 128;
int cylinders, sectors, fd;
char* fileName;

struct dataBlock {
    char data[128];
};

void handleConnection(void **servSock) {
    struct stat sb;
    int sock = *((int*)servSock);
    struct sockaddr_in clientAddr;
    int clientSocket;
    char buffer[4096];

    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);
    }

    while(true) {
        memset(buffer, 0, 4096);

        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        int stringLen = sizeof(string(buffer, 0, bytesReceived));

        cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

        int n = bytesReceived;

        if(toupper(buffer[0]) == 'I') {
            memset(buffer, 0, 4096);
            string s = to_string(cylinders) + " " + to_string(sectors);
            strcpy(buffer, s.c_str());
        }
        else if(toupper(buffer[0] == 'R')) {
            memset(buffer, 0, 4096);
            char *p = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, , 0);
        }

        send(clientSocket, buffer, 4096, 0);
    }
    close(clientSocket);
}

int main(int argc, char* argv[]) {
    int sock, port;
    struct sockaddr_in hint, clientAddr;
    int numOfThreads = 20;
    pthread_t threads[numOfThreads];

    if(argc != 5) {
        fprintf(stderr, "ERROR please enter a port #, # of cylinder, # of sectors per cylinder, file name\n");
        exit(0);
    }

    port = atoi(argv[1]);
    cylinders = atoi(argv[2]);
    sectors = atoi(argv[3]);
    fileName = argv[4];

    fd = open(fileName);

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