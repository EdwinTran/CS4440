/*
    Takes in a compressed file version of 0s and 1s and prints it out. Decompresses the file
    if there are compressed parts in there.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

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
    
    if(!infile.good()) {
        cout << "FILE NOT FOUND \n";
        return 0;
    }

    char c;

    // Gets each character till the end of file.
    // Will print out the character as is unless
    // the character is a '+' or '-'.
    while(!infile.eof()) {
        infile.get(c);

        if(c == '+') {
            infile.get(c);
            int num = 0;

            // Gets the number in between the '+'
            // and prints out the char that number of times.
            while (c != '+') {
                num = (num * 10) + (c - '0');
                infile.get(c);
            }

            for (int i = 0 ; i < num; i++) {
                outfile << '1';
            }
        }
        else if(c == '-') {
            infile.get(c);
            int num = 0;

            // Gets the number in between the '-'
            // and prints out the char that number of times.
            while(c != '-') {
                num = (num * 10) + (c - '0');
                infile.get(c);
            }
            
            for(int i = 0; i < num; i++) {
                outfile << '0';
            }
        }
        else {
            outfile << c;
        }
    }

    outfile << " ";

    infile.close();
    outfile.close();
    return 1;
}