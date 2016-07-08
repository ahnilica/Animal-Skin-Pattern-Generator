/*
    Generates animal-skin inspired patterns in black and white by the use of
    an Activator-Inhibitor Celuar Automata.
*/

#define GRID_SIZE 100

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
using namespace std;

class CA {

  struct Cell {
    int row, col;
    Cell(int x, int y) {
      row = x; col = y;
    }
  };

public:
    CA(double h_par, double J1_par, double J2_par, int R1_par, int R2_par, int maxSteps_par, string expID_par);
    void runCA();

private:
    //input paramiters
    string expID;
    double J1, J2, h;
    int R1, R2;

    int numChanges, step, maxSteps;

    vector< vector<int> > cellStates;
    vector<Cell> cellList;
    vector<Cell> randCells;

    int dist(Cell cell1, Cell cell2);
    void updateGrid();
    void updateCell(Cell cell);
    void makePGM(int rows, int cols, string fname);
    void printGrid();
};

void CA::printGrid() {
    for(int i=0; i < cellStates.size(); i++) {
        for(int j=0; j < cellStates[i].size(); j++) {
            cout << cellStates[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}

CA::CA(double h_par, double J1_par, double J2_par, int R1_par, int R2_par, int maxSteps_par, string expID_par) {

    cout << "Initializing Grid" << endl;

    //set paramiter values from command line args
    h = h_par;
    J1 = J1_par;
    J2 = J2_par;
    R1 = R1_par;
    R2 = R2_par;
    maxSteps = maxSteps_par;
    expID = expID_par;
    numChanges = -1;
    step = 0;

    //seed random number generator
    srand(time(NULL));

    //set cell state grid to size of cell gird
    cellStates.resize(GRID_SIZE);
    for(int i=0; i < cellStates.size(); i++) {
      cellStates[i].resize(GRID_SIZE);
    }

    //initialize cell state grid with random 0 or 1 values
    for(int i=0; i < cellStates.size(); i++) {
        for(int j=0; j < cellStates[i].size(); j++) {
            cellStates[i][j] = rand() % 2;
        }
    }

    //initialize list of cell coordinates (used to select random cells)
    for(int i=0; i < cellStates.size(); i++) {
        for(int j=0; j < cellStates[i].size(); j++) {
        	  Cell c(i, j);
        	  cellList.push_back(c);
    	  }
    }

    //initlize another list of cell coordinates to shuffle (for random cell selection)
    for(int i=0; i < cellStates.size(); i++) {
        for(int j=0; j < cellStates[i].size(); j++) {
        	  Cell c(i, j);
        	  randCells.push_back(c);
    	  }
    }
}

void CA::runCA() {

  numChanges = 6;

  cout << "Starting System..." << endl;

  //update grid until it stabalizes
  while(numChanges > 5) {
    updateGrid();

    if(step >= maxSteps) {
      cout << endl << "Max Step Count Reached!" << endl << endl;
      break;
    }
    step++;

    cout << "Compleated Step " << step << endl;
    cout << "Number of Cell State Changes: " << numChanges << endl << endl;
  }

  cout << "Writting PGM File" << endl;
  makePGM(GRID_SIZE, GRID_SIZE, expID);
  cout << "Done!" << endl;
}

void CA::updateGrid() {
    numChanges = 0;

    //shuffles the list of cells for random selection
    random_shuffle(randCells.begin(), randCells.end());

    //update every cell in the grid in random order
    for(int i=0; i < randCells.size(); i++) {
        updateCell(randCells[i]);
    }
}

void CA::updateCell(Cell currentCell) {
    int N1, N2;
    double distance, value;

    N1 = 0;
    N2 = 0;

    int prevValue = cellStates[currentCell.row][currentCell.col];

    //calculate sums of N1 and N2
    for(int i=0; i < cellList.size(); i++) {
    	  distance = dist(currentCell, cellList[i]);
    	  if(distance < R1) {N1 += cellStates[cellList[i].row][cellList[i].col];}
    	  else if((distance >= R1) && (distance < R2)) {N2 += cellStates[cellList[i].row][cellList[i].col];}
    }

    //calculate new value of cell
    value = h + (N1 * J1) + (N2 * J2);

    //set value at either 0 or 1 based on calculated value
    if(value >= 0.0) {
        if(prevValue == 0) {
        	  cellStates[currentCell.row][currentCell.col] = 1;
        	  numChanges++;
        }
    }
    else {
        if(prevValue == 1) {
        	  cellStates[currentCell.row][currentCell.col] = 0;
        	  numChanges++;
        }
    }
}

int CA::dist(Cell cell1, Cell cell2) {
    //computes the L1 metric distance between two cells (aka grid distance)
    int xdist, ydist;

    xdist = abs(cell1.row - cell2.row);
    if(xdist > (GRID_SIZE / 2)) {xdist = GRID_SIZE - xdist;}

    ydist = abs(cell1.col - cell2.col);
    if(ydist > (GRID_SIZE / 2)) {ydist = GRID_SIZE - ydist;}

    return xdist + ydist;
}

void CA::makePGM(int rows, int cols, string fname) {
    //create new pgm file
    const string fileExtension = ".pgm";
    fname.append(fileExtension);

    ofstream outputFile;
    outputFile.open(fname.c_str());

    if(outputFile.fail()) {
        cerr<<"Failed to create PGM Image"<<endl;
    	  exit(1);
    }

    //write pgm header
    outputFile << "P2" << endl << rows << " " << cols << endl << "505" <<endl;

    //write pgm content
    for(int i=0; i < cellStates.size(); i++) {
        for(int j=0; j < cellStates[i].size(); j++) {
        	  if(cellStates[i][j] == 1) {outputFile << "0" << " ";}
        	  else {outputFile << "505" << " ";}
    	  }
    	  outputFile << endl;
    }
    outputFile.close();
}

int main(int argc, char * argv[]) {
    //pasrse command line args
    if(argc == 1) {
        //preset to interesting setting if no paramiters specified
        CA ca(1.5, 0.5, -0.5, 10, 14, 25, "output");
        ca.runCA();
    }
    else if(argc == 7) {
        //no step count specified
        CA ca(atof(argv[1]), atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), 25, argv[6]);
        ca.runCA();
    }
    else if(argc == 8) {
        CA ca(atof(argv[1]), atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
        ca.runCA();
    }
    else {
        cerr<<"Invalid Command Line Args"<<endl;
        cerr<<"Usage: h J1 J2 R1 R2 maxSteps ID"<<endl;
    	  exit(1);
    }
}
