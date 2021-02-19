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

    while(!infile.eof()) {
        infile.get(c);

        if(c == '+') {
            infile.get(c);
            int num = 0;

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