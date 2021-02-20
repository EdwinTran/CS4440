#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

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
    string fileInput;
    ifstream infile;
    string fileOutput;
    ofstream outfile;

    if(argc > 2) {
        infile.open(argv[1]);
        outfile.open(argv[2]);
    }
    else {
        cout << "ERROR: INPUT SOURCE AND DESTINATION FILES\n";
        return 0;
    }

    if(!infile.good()) {
        cout << "FILE NOT FOUND \n";
        return 0;
    }

    char c;
    int counter = 0;
    char lastChar = ' ';

    while(!infile.eof()) {
        infile.get(c);

        if(c != lastChar) {
            compress(outfile, lastChar, counter);
            counter = 0;
        }

        counter++;
        lastChar = c;
    }

    infile.close();
    outfile.close();
    return 1;
}