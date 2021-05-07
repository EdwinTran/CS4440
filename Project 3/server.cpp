#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>


using namespace std;

int bytesReceived = 0;

void reverseString(char *buffer) {
    int n = bytesReceived;
    for(int i = 0; i <= bytesReceived / 2; i++) {
        swap(buffer[i], buffer[n]);
        n--;
    }
}

int main() {
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cout << "Can't create socket";
        return 0;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*) &hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    sockaddr_in client;
    socklen_t clientLen = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*) &client, &clientLen);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    close(listening);

    char buf[4096];

    while(true) {
        pthread_t thread;

        memset(buf, 0, 4096);

        bytesReceived = recv(clientSocket, buf, 4096, 0);

        int stringLen = sizeof(string(buf, 0, bytesReceived));

        cout << "Received: " << string(buf, 0, bytesReceived) << "\n";

        if(bytesReceived == -1) {
            cout << "ERROR in receiving" << endl;
            break;
        }

        if(bytesReceived == 0) {
            cout << "Client disconnected " << endl;
            break;
        }
        
        pthread_create(&thread, NULL, (void *(*)(void *))reverseString, &buf);

        pthread_join(thread, NULL);

        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    close(clientSocket);
    return 0;
}