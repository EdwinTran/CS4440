#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main() {
    string fileInput;
    cout << "Please type what input file you want to open. \n";
    cin >> fileInput;
    ifstream infile(fileInput);

    if(!infile.good()) {
        cout << "FILE NOT FOUND \n";
        return 0;
    }

    string fileOutput;
    ofstream outfile;
    cout << "Please type what output file you want to open. \n";
    cin >> fileOutput;
    outfile.open(fileOutput);

    char c;
    int counter = 0;
    char lastChar = ' ';

    while(!infile.eof()) {
        infile.get(c);

        if(c != lastChar) {
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
            counter = 0;
        }

        counter++;
        lastChar = c;
    }

    infile.close();
    outfile.close();
    return 1;
}