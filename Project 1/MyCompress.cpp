/*
    Takes in an input file of 0s and 1s and writes it to the output file. If there are a row of 16 
    or more 1s or 0s then it will compress it by adding a '+' or '-' symbol then the number of 
    1s or 0s and output it to the file.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

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
    ifstream infile;
    ofstream outfile;

    // Opens the input and output file if the there are enough command line arguments.
    // Else it will print out an error statement and exit the program.
    if(argc > 2) {
        infile.open(argv[1]);
        outfile.open(argv[2]);
    }
    else {
        cout << "ERROR: INPUT SOURCE AND DESTINATION FILES\n";
        return 0;
    }
    
    // Checks if the input file exists.
    // If it doesn't then it will print and error statement and exit the program
    if(!infile.good()) {
        cout << "FILE NOT FOUND \n";
        return 0;
    }

    char c;
    int counter = 0;
    char lastChar = ' ';

    // Loops while there are characters in the file and
    // gets the characters.
    while(!infile.eof()) {
        infile.get(c);

        // Checks if the current character is the same as the last one.
        // If it isn't then it will call the compress function then
        // make counter equal to 0 again.
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