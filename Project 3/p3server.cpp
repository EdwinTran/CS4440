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

//Creates a struct that holds an array of 128 characters.
struct dataBlock {
    char data[128];
};

void handleConnection(void **servSock) {
    struct stat sb;
    int sock = *((int*)servSock);
    struct sockaddr_in clientAddr;
    int clientSocket;
    char buffer[4096];

    //Checks to see if the file size can be received from the file.
    //If it can it is saved as the struct stat.
    if(fstat(fd, &sb) == -1) {
        perror("CANNOT GET FILE SIZE\n");
    }

    //Accepts the client socket that is connecting.
    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if(clientSocket == -1) {
        perror(NULL);
    }

    //Continuously run until the client types 'E'.
    while(true) {
        int userCylin = 0, userSec = 0;
        char choice;
        string updatedStr;
        char retBuf[129];

        //Receives the string from the client and saves as buffer.
        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        //Creates a string which holds the contents of buffer.
        //Also creates a stringstream which will be used to split the contents.
        string bufStr(buffer);
        istringstream split(bufStr);

        //Splits the contents by white spaces to get the choice cylinder and sector
        //and saves them into variables.
        split >> choice >> userCylin >> userSec;

        //Gets the remaining string from the buffer to be used for writing if
        //writing is chose as an option.
        getline(split, updatedStr);

        int stringLen = sizeof(string(buffer, 0, bytesReceived));
        int n = bytesReceived;

        //Prints out that the server received the string.
        cout << "Received: " << string(buffer, 0, bytesReceived) << "\n";

        if(toupper(choice) == 'I') {
            //Clears the buffer variable
            memset(buffer, 0, 4096);
            //Creates a string that will hold the number of cylinders
            //and the number of sectors and copys it to retBuf.
            string s = to_string(cylinders) + " " + to_string(sectors);
            strcpy(retBuf, s.c_str());
        }
        else if(toupper(choice) == 'R') {
            //Checks to see if the number of cylinders and sectors provided by the 
            //client is valid.
            if((userCylin > 0 && userCylin <= cylinders) && (userSec > 0 && userSec <= sectors)) {
                //Creates a virtual space and saves it as the struct datablock.
                //Can only read
                struct dataBlock *p = (dataBlock *)mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

                //Checks to see if the mapping failed
                if(p == MAP_FAILED) {
                    fprintf(stderr, "ERROR MAPPING FAILED\n");
                    std::exit(0);
                }

                //Copys the contents at the specified cylinder and sectors and saves it
                // as retBuf
                memcpy(retBuf + 1, p[(userCylin - 1) * sectors + (userSec - 1)].data, 128);
                //Saves a 1 if at index 0 to signify that it worked.
                retBuf[0] = '1';
            }
            else {
                //Saves a 0 at index 0 of retbuf to signify that it did not work.
                retBuf[0] = '0';
            }
        }
        else if(toupper(choice) == 'W') {
            //Checks to see if the number of cylinders and sectors provided by the 
            //client is valid.
            if((userCylin > 0 && userCylin <= cylinders) && (userSec > 0 && userSec <= sectors)) {
                //Creates a virtual space and saves it as the struct datablock.
                //Can be read and written to
                struct dataBlock *p = (dataBlock *)mmap(0, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

                //Checks to see if the mapping failed
                if(p == MAP_FAILED) {
                    fprintf(stderr, "ERROR MAPPING FAILED\n");
                    std::exit(0);
                }

                //Copys the string that the user provided and virtual space which is saved into the file.
                memcpy(p[(userCylin - 1) * sectors + (userSec - 1)].data, updatedStr.c_str(), updatedStr.length());
                //Saves a 1 if at index 0 to signify that it worked.
                retBuf[0] = '1';
            }
            else {
                //Saves a 0 at index 0 of retbuf to signify that it did not work.
                retBuf[0] = '0';
            }
        }
        //Closes the socket connection if choice is the character 'E' and
        //ends the loop
        else if(toupper(choice) == 'E') {
            close(clientSocket);
            break;
        }

        //sends the results from whatever choice that the user choice to the client.
        send(clientSocket, retBuf, 129, 0);
        //Clears retBuf and buffer.
        memset(retBuf, 0, 129);
        memset(buffer, 0, 4096);
    }
}

int main(int argc, char* argv[]) {
    int sock, port;
    struct sockaddr_in hint, clientAddr;
    int numOfThreads = 20;
    pthread_t threads[numOfThreads];

    //Checks to see if there are enough arguments
    if(argc != 5) {
        fprintf(stderr, "ERROR please enter a port #, # of cylinder, # of sectors per cylinder, file name\n");
        std::exit(0);
    }

    //Saves the command line arguments into variables.
    port = atoi(argv[1]);
    cylinders = atoi(argv[2]);
    sectors = atoi(argv[3]);
    fileName = argv[4];

    //Opens a file to be read and written in
    fd = open(fileName, O_RDWR);

    //Creates a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //Checks to see if the socket can be created.
    if(sock == -1) {
        fprintf(stderr, "ERROR cannot create socket\n");
        std::exit(0);
    }

    //Creates a sockaddr and makes it hold any ip address and 
    //holds a port # specified by the user
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_port = port;

    //Checks to see if the socket and the socket address can be binded.
    if(bind(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot bind\n");
        std::exit(0);
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