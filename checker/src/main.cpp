#include <iostream>
#include <fstream>
#include "Placement.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 5) {
        cout << "Usage: ./[EXE_FILE] "; 
        cout << "[BLIF_FILE] ";
        cout << "[PL_FILE] "; 
        cout << "[CORE_WIDTH] ";
        cout << "[CORE_HEIGHT]" << endl;
        exit(1);
    }

    Placement placement(stoi(argv[3]), stoi(argv[4]));
    placement.blifParse(argv[1]);
    placement.plParse(argv[2]);
    placement.initial();

    cout << "starting to check model " << placement.model() << " placement ..." << endl;
    fstream fout_csv;
    fout_csv.open("result.csv", ios::out | ios::app);
    fout_csv << placement.model() << ", ";
    if(placement.checkCorrectness()) {
        cout << "model " << placement.model() << " check successful" << endl;
        cout << "Total HPWL: " << placement.calcHPWL() << endl;
        fout_csv << placement.calcHPWL() << endl;
    }
    else {
        cout << "model " << placement.model() << " check failed" << endl;
        fout_csv << "-" << endl;
    }

    // placement.printNets();
    // placement.printRowCells();
    // placement.printRowCells();
    // placement.printCells();
    // placement.printNets();
}
