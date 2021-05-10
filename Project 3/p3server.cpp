#include <sys/mman.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

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

    if(fstat(fd, &sb) == -1) {
        perror("CANNOT GET FILE SIZE\n");
    }

    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);
    }

    while(true) {
        //RESET THE INPUTED CYLINDERS AND SECTORS TO 0
        int userCylin = 0, userSec = 0;
        char choice;
        string updatedStr;
        char retBuf[129];

        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        string bufStr(buffer);
        istringstream split(bufStr);

        split >> choice >> userCylin >> userSec;
        getline(split, updatedStr);

        int stringLen = sizeof(string(buffer, 0, bytesReceived));
        int n = bytesReceived;

        cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

        if(toupper(choice) == 'I') {
            memset(buffer, 0, 4096);
            string s = to_string(cylinders) + " " + to_string(sectors);
            strcpy(retBuf, s.c_str());
        }
        else if(toupper(choice) == 'R') {
            if((userCylin > 0 && userCylin <= cylinders) && (userSec > 0 && userSec <= sectors)) {
                //GET CYLINDERS AND SECTORS FROM READING FROM CLIENT
                struct dataBlock *p = (dataBlock *)mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

                if(p == MAP_FAILED) {
                    fprintf(stderr, "ERROR MAPPING FAILED\n");
                    std::exit(0);
                }

                memcpy(retBuf + 1, p[(userCylin - 1) * sectors + (userSec - 1)].data, 128);
                retBuf[0] = '1';
            }
            else {
                retBuf[0] = '0';
            }
        }
        else if(toupper(choice) == 'W') {
            if((userCylin > 0 && userCylin <= cylinders) && (userSec > 0 && userSec <= sectors)) {
                struct dataBlock *p = (dataBlock *)mmap(0, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

                if(p == MAP_FAILED) {
                    fprintf(stderr, "ERROR MAPPING FAILED\n");
                    std::exit(0);
                }


                memcpy(p[(userCylin - 1) * sectors + (userSec - 1)].data, updatedStr.c_str(), updatedStr.length());
                retBuf[0] = '1';
            }
            else {
                retBuf[0] = '0';
            }
        }
        else if(toupper(choice) == 'E') {
            close(clientSocket);
            break;
        }

        send(clientSocket, retBuf, 129, 0);
        memset(retBuf, 0, 129);
        memset(buffer, 0, 4096);
    }
}

int main(int argc, char* argv[]) {
    int sock, port;
    struct sockaddr_in hint, clientAddr;
    int numOfThreads = 20;
    pthread_t threads[numOfThreads];

    if(argc != 5) {
        fprintf(stderr, "ERROR please enter a port #, # of cylinder, # of sectors per cylinder, file name\n");
        std::exit(0);
    }

    port = atoi(argv[1]);
    cylinders = atoi(argv[2]);
    sectors = atoi(argv[3]);
    fileName = argv[4];

    fd = open(fileName, O_RDWR);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1) {
        fprintf(stderr, "ERROR cannot create socket\n");
        std::exit(0);
    }

    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_port = port;

    if(bind(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot bind\n");
        std::exit(0);
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