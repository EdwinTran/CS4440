#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    int sock, port;
    struct hostent *host;
    struct sockaddr_in hint;
    string commands;

    //Checks to see if there are enough arguments.
    if(argc < 3) {
        fprintf(stderr, "ERROR not enough arguments\n");
        exit(0);
    }

    //Saves the port number provided from command line into a variable.
    port = atoi(argv[2]);

    //Creates a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //Checks to see if the socket can be opened.
    if(sock == -1) {
        fprintf(stderr, "ERROR cannot open socket\n");
        exit(0);
    }

    //Gets the host by the name.
    host = gethostbyname(argv[1]);
    if(host == NULL) {
        fprintf(stderr, "ERROR host does not exist\n");
        exit(0);
    }

    //Creates a socket address for the server.
    hint.sin_family = AF_INET;
    memcpy(&hint.sin_addr.s_addr, host->h_addr, host->h_length);
    hint.sin_port = port;

    //Connects to the server using the socket address.
    if(connect(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot connect\n");
        exit(0);
    }

    //Creates a string of the commands and prints it.
    commands = "\n[I]nfo\t-Gets Disk Geometry\n";
    commands += "[R]ead\t-Read from disk 'R [Cylinders] [Sectors]\n";
    commands += "[W]rite\t-Writes to disk 'W [Cylinders] [Sectors] [string]\n";
    commands += "[E]xit\t -Exits the program\n\n";

    cout << commands;
    
    char buf[4096];
    string userInput;

    //Continuously loops until the string "E" is entered
    while(userInput != "E") {
        cout << "> ";
        getline(cin, userInput);

        //Sends user input to server.
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        //Checks to see if the server input can sent to the server.
        if(sendRes == -1) {
            cout << "Could not send to server";
            exit(0);
        }

        //Clears the buffer.
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        //Checks to see if there was any response from server.
        //If not prints out the response.
        if(bytesReceived == -1) {
            cout << "There was an error getting response";
        }
        else {
            cout << "SERVER> " << string(buf, bytesReceived) << "\n";
        }
    }
    cout << "DISCONNECTED FROM SERVER\n\n";
    
    close(sock);
    return 0;
}