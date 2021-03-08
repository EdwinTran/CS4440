
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
#include <chrono>

using namespace std;
using namespace std::chrono;

auto start = high_resolution_clock::now();
//Global string is created which will allow access to all functions.
string binarystring = "";
//A struct is used in order to pass 2 integers into the thread argument;
struct boundary
{
    int start;
    int end;
};
//This function will create the chunk of the file and compress it. The compressed string will then be returned.
void * compressor(void *arg){

    struct boundary tempbounds = *((struct boundary *)arg);
    int startingp = tempbounds.start;
    int endingp = tempbounds.end;
    int counter = 0;
    char lastChar = ' ';
    string mainstring = "";
    string finalstring = "";
    vector<string> vstring;
    
    //this section gets the chunk from the string based on start and end parameters provided by the struct
    for(int u = startingp; u<endingp; u++){
        mainstring.push_back(binarystring.at(u));
    }
    //this section compresses the files
    for(int r = 0; r<mainstring.length(); r++){
        if(mainstring.at(r) != lastChar) {
            if(counter >= 16) {
        if(lastChar == '1') {
            string compressedVer = "+" + to_string(counter) + "+";
            finalstring.append(compressedVer);
        }
        else {
            string compressedVer = "-" + to_string(counter) + "-";
            finalstring.append(compressedVer);
        }
    }
   else {
       string tempstring = "";
        for(int i = 0; i < counter; i++) {
            tempstring.push_back(lastChar);
        }
        finalstring.append(tempstring);
    }
            counter = 0;

        }

        counter++;
        lastChar = mainstring.at(r);
    }

    //this will convert a string into a char in order to return it back to the main for thread_join.
    char * usr = (char*)malloc(5000 * sizeof(finalstring));
    strcpy(usr, finalstring.c_str());

    return usr;
}

int main(int argc, char *argv[]) {
    string fileInput;
    ifstream infile;
    string fileOutput;
    ofstream outfile;
    //this part reads the in and outfile
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
    int n = 100;
    int saver = 0;
    vector<int> binaryindex;
    //this while loop will take the binary string from the file into a string
    while(!infile.eof()) {
        infile.get(c);
        binarystring.push_back(c);
    }
    //the splitter will calculate how long each chunk will be
    //the finalnum will make sure to get the correct number at the final n chunk. This helps if the string is of odd number.
    int splitter = int(binarystring.length()/n);
    int finalnum = binarystring.length() - splitter * (n - 1);
    //this is where the start and ending index of each N chunks will be found.
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

    //a struct is called and the ints are created in order to store the start and ending index of each N chunk. This will be stored in a list.
    for(int v = 0; v<n; v++){
        struct boundary boundaries;
        savecounter = v * 2;
        boundaries.start = binaryindex[savecounter];
        boundaries.end = binaryindex[savecounter + 1];
        boundlist.push_back(boundaries);
    }
    //this forloop will create a N threads in order to execute the compressor
    for(int d = 0; d<boundlist.size();d++){
        pthread_create(&thread_id[d], NULL, compressor, &boundlist[d]);
    }

    char * temp = NULL;
    //this for loop will make sure that each thread is done and is printed out toward the output file in order.
    for(int r = 0; r<n; r++){
        pthread_join(thread_id[r], (void**)&temp);
        outfile<<temp;
    }

    infile.close();
    outfile.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    cout<<duration.count()<<endl;
    return 0;
}