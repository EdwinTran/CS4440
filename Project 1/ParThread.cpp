#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <string>
#include <unistd.h>
#include <list>
#include <iterator>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

string binarystring = "";
vector<int>finallist;

struct boundary
{
    int start;
    int end;
};


string compress(int counter, char lastChar) {
    
    if(counter >= 16) {
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
       string tempstring = "";
        for(int i = 0; i < counter; i++) {
            tempstring.push_back(lastChar);
        }
        return tempstring;
    }
    return "it closed";
}

void * reader(void *arg){

    struct boundary tempbounds = *((struct boundary *)arg);
    int startingp = tempbounds.start;
    int endingp = tempbounds.end;
    int counter = 0;
    char lastChar = ' ';
    string mainstring = "";
    string finalstring = "";
    vector<string> vstring;
    

    for(int u = startingp; u<endingp; u++){
        mainstring.push_back(binarystring.at(u));
  }
    
    for(int r = 0; r<mainstring.length(); r++){
        if(mainstring.at(r) != lastChar) {
            cout<<compress(lastChar, counter)<<"\n";
            counter = 0;

        }

        counter++;
        lastChar = mainstring.at(r);
    }


    char * usr = (char*)malloc(100 * sizeof(mainstring));
    strcpy(usr, mainstring.c_str());

    return usr;
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
        
        return 0;
    }

    if(!infile.good()) {
       
        return 0;
    }

    char c = ' ';
    int n = 4;
    int saver = 0;
    vector<int> binaryindex;

    while(!infile.eof()) {
        infile.get(c);
        binarystring.push_back(c);
    }
    int splitter = int(binarystring.length()/n);
    int finalnum = binarystring.length() - splitter * (n - 1);
    for(int k = 0 ; k<n; k++){
        if(k == 0){
            binaryindex.push_back(k * splitter);
            binaryindex.push_back((k + 1) * splitter);
        }
        else if(k != n - 1){
            binaryindex.push_back((k * splitter) + 1);
            binaryindex.push_back((k + 1) * splitter);
        }
        else{
            binaryindex.push_back((k * splitter) + 1);
            binaryindex.push_back(finalnum + (k * splitter));
        }
    }

    pthread_t thread_id[n];
    int savecounter = 0;
    vector<struct boundary> boundlist;
    void *result;
    for(int v = 0; v<n; v++){
        struct boundary boundaries;
        savecounter = v * 2;
        boundaries.start = binaryindex[savecounter];
        boundaries.end = binaryindex[savecounter + 1];
        boundlist.push_back(boundaries);
    }

    for(int d = 0; d<boundlist.size();d++){
        pthread_create(&thread_id[d], NULL, reader, &boundlist[d]);
    }

    char * temp = NULL;

    for(int r = 0; r<n; r++){
        pthread_join(thread_id[r], (void**)&temp);
    }
    return 0;
}