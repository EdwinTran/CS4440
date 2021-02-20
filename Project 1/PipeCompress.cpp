#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void compress(ofstream& outfile,char lastChar, int counter) {
    if(counter >= 16) {
        if(lastChar == '1') {
            string compressedVer = "+" + to_string(counter) + "+";
            outfile << compressedVer;
        }
        else {
            string compressedVer = "-" + to_string(counter) + "-";
            outfile << compressedVer;
        }
    }
    else {
        for(int i = 0; i < counter; i++) {
            outfile << lastChar;
        }
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int p[2];

    if(pipe(p) == -1) {
        printf("Failed to create the pipe. \n");
    }

    pid = fork();

    if(pid < 0) {
        printf("Error \n");
    }
    else if(pid == 0){
        printf("Child Process: STARTED\n");
        
        ofstream fileOutput;
        fileOutput.open(argv[2]);

        close(p[1]);

        char c;
        char lastChar = ' ';
        int counter = 0;
        while(read(p[0], &c, 1) > 0) {
            if(c != lastChar) {
                compress(fileOutput, lastChar, counter);
                counter = 0;
            }

            counter++;
            lastChar = c;
        }
        

        close(p[0]);
        fileOutput.close();
        printf("Child process: DONE \n");
    }
    else {
        printf("Parent process: STARTED\n");
        string str = "";
        ifstream fileInput;

        fileInput.open(argv[1]);
        if (!fileInput.good())
        {
            cout << "ERROR";
            return 0;
        }
        
        close(p[0]);

        char c;
        while (!fileInput.eof()) {
            fileInput.get(c);
            write(p[1], &c, 1);
        }

        close(p[1]);
        fileInput.close();
        wait(NULL);
        printf("Parent process: DONE \n");
    }

    return 0;
}