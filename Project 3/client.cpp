#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        return 0;
    }

    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(sock, (sockaddr*) &hint, sizeof(hint));
    if(connectRes == -1) {
        return 0;
    }

    char buf[4096];
    string userInput;

    do{
        cout << "> ";
        getline(cin, userInput);

        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if(sendRes == -1) {
            cout << "Could not send to server";
            continue;
        }

        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        if(bytesReceived == -1) {
            cout << "There was an error getting response";
        }
        else {
            cout << "SERVER> " << string(buf, bytesReceived) << "\n";
        }
    } while(true);

    close(sock);
    return 0;
}