/*
    Takes in a file of 0s and 1s which compresses it. The process is divided between the parent
    and the child process. The parent process will read from the file and write it into a pipe
    while the child process will read from the pipe, compress it, and write it into the file.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Compresses the string and outputs the string to the output file.
void compress(ofstream& outfile,char lastChar, int counter) {
    // Checks whether counter is greater than 16
    // If greater than 16 then it will print out the compressed version
    // else if it isn't then it will print out the respective number 'counter' number of times.
    if(counter >= 16) {
        // Checks if it is a row of 1s or 0s.
        // Will print out '+' on either side of the number 'counter' if 1s.
        // Else it will print out '-' on either side along with 'counter in the middle.
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
    
    // Creates a child process from the parent
    pid = fork();

    if(pid < 0) {
        printf("Error \n");
    }
    // The child process reads from the pipe and compresses the file.
    // It is the written to the output file.
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
    // The parent process reads from the file and writes it into the pipe
    // for the child to compress and write into the output file.
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