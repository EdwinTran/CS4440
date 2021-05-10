#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int sock, port;
    struct hostent *host;
    struct sockaddr_in hint;
    //Check to see if there are enough arguments
    if(argc < 3) {
        fprintf(stderr, "ERROR not enough arguments\n");
        exit(0);
    }

    port = atoi(argv[2]);
    //Sockets() used to make the socket. Which will be used for communication
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //Program exits because socket cannot open
    if(sock == -1) {
        fprintf(stderr, "ERROR cannot open socket\n");
        exit(0);
    }
    //Returns pointer to host information
    host = gethostbyname(argv[1]);
    //Check to see if the host exist
    if(host == NULL) {
        fprintf(stderr, "ERROR host does not exist\n");
        exit(0);
    }

    hint.sin_family = AF_INET;
    memcpy(&hint.sin_addr.s_addr, host->h_addr, host->h_length);
    hint.sin_port = port;
    //Create and checks to see if the socket is connected with the connect function
    if(connect(sock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        fprintf(stderr, "ERROR cannot connect\n");
        exit(0);
    }
    
    char buf[4096];
    string userInput;
    //user input
    cout << "> ";
    getline(cin, userInput);
    //Sending user input to server
    int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
    //lets user know that they cannot send their input to the server
    if(sendRes == -1) {
        cout << "Could not send to server";
        exit(0);
    }

    memset(buf, 0, 10000);
    //Receive information from sock
    int bytesReceived = recv(sock, buf, 10000, 0);
    //Lets the user know that an error 
    if(bytesReceived == -1) {
        cout << "There was an error getting response";
    }
    else {
        cout << "SERVER> " << string(buf, bytesReceived) << "\n";
    }
    //closes the sock
    close(sock);
    return 0;
}