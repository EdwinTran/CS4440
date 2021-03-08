/*
    Splits a file into N sections and uses multiple forks to
    read, compress, and write into an output file.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>

using namespace std;

int N = 10;

// Compresses the string and outputs the string to the output file.
string compress(ofstream& outfile,char lastChar, int counter) {

    // Checks whether counter is greater than 16
    // If greater than 16 then it will print out the compressed version
    // else if it isn't then it will print out the respective number 'counter' number of times.
    if(counter >= 16) {

        // Checks if it is a row of 1s or 0s.
        // Will print out '+' on either side of the number 'counter' if 1s.
        // Else it will print out '-' on either side along with 'counter in the middle.
        if(lastChar == '1') {
            string compressedVer = "+" + to_string(counter) + "+";
            return compressedVer;
        }
        else {
            string compressedVer = "-" + to_string(counter) + "-";
            return compressedVer;
        }
    }
    else {
        string s = "";
        for(int i = 0; i < counter; i++) {
            s += lastChar;
        }
        return s;
    }
}

int main(int argc, char *argv[]) {
    pid_t pids[N];
    ifstream infile;
    ofstream outfile;

    // Checks if there is an input and output file
    if(argc > 2) {
        infile.open(argv[1]);
        outfile.open(argv[2]);
    }
    else {
        
        return 0;
    }

    if(!infile.good()) {
       
        return 0;
    }

    // Splits the file into equal chunks and
    // puts them into a vector.
    vector<string> temp;
    string chunk = "";

    while(getline(infile, chunk)) {
        temp.push_back(chunk);
    }

    string str = "";
    vector<string> finalString;
    int push = temp.size()/N;
    int count = 0;
    for(int i = 0; i < temp.size(); i++) {
        chunk += temp[i];

        count++;
        if(count >= push) {
            temp.erase(temp.begin(), temp.begin() + i);
            finalString.push_back(chunk);
            str = "";
            count = 0;
            i = 0;
        }
    }

    finalString[N - 1] = finalString[N - 1].append(str);
    
    // Creates N forks and for each fork the child will
    // compress the file and output to the file.
    for(int i = 0; i < N; i++) {
    	pids[i] = fork();

        if(pids[i] == 0) {
            char c;
            char lastChar = ' ';
            int counter = 0;

            // Checks if the char is the same as the last char
            // for the current child's string
            for(int k = 0; k < finalString[i].length(); k++) {
                c = finalString[i].at(k);

                if(c != lastChar) {
                    outfile << compress(outfile, lastChar, counter);
                    counter = 0;
                }

                counter++;
                lastChar = c;
            }

            outfile << "\n";
            return 0;
        }
    }

    outfile.close();

    int status;
    pid_t pid;

    int n = N;
    while (n > 0) {
        pid = wait(&status);
        n--;
    }

    return 0;
}