#include <fstream>
#include <string>
#include<iostream>
using namespace std;

int main() {
    
    string filename = "plan.txt";
    
    // Open file for reading
    ifstream fin; 
    fin.open(filename);
    
    // Check if file opened correctly
    if(!fin.is_open()) {
        cout << "Could not open file" << endl;
        return 1;
    }
    
    // Read contents of file
    string line; 
    while(getline(fin, line)) {
        cout << line << endl;  
    }
    
    // Close input file 
    fin.close();
    
    // Open file for writing   
    ofstream fout;
    fout.open(filename,ios::app);  

    fout << "This is a new line" << endl;
    
    // Close output file
    fout.close();
        
    return 0;
}