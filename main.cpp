//
//  main.cpp
//  2048
//
//  Created by Karna Sethia on 27/02/2015.
//  Copyright (c) 2015 Karna Sethia. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

void moveUp(int grid[][4], int gridChange[][4]);
void moveDown(int grid[][4], int gridChange[][4]);
void moveLeft(int grid[][4], int gridChange[][4]);
void moveRight(int grid[][4], int gridChange[][4]);
bool wrongInput(string dir);
void printGrid(int grid[][4]);
bool gameOver(string dir, int grid[][4]);
bool noMove(int grid[][4], int gridCopy[][4]);
void updateGrid(string dir, int grid[][4], int gridEmpty[][4], int gridChange[][4], int gridCopy[][4]);

int main () {
    
    int grid[4][4];
    int gridEmpty[4][4]; //2d array to see if an element in grid is empty or not
    int gridChange[4][4]; //2d array used to keep track of merges made per turn
    int gridCopy[4][4]; //2d array used to check if grid[4][4]== (grid[4][4] after a move is made)
    
    ifstream infile;
    string filename;
    cout << "enter initial configuration file name:" << endl;       //if the user has an initial config file that they want to run
    cin >> filename;                                                //it is opened here
    infile.open(filename.c_str());
    if (!infile.is_open()) {
        cout << "file not found, using default start configuration" << endl;    //default config is 2 in the bottom right corner
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                grid[i][j] = 0;
            }
        }
        grid[3][3]=2;               //default is set here
        printGrid(grid);
    } else {                        //if a file is found, input the configuration into the grid
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                infile >> grid[i][j];
            }
        }
        printGrid(grid);
    }
    
    string dir; //direction - uese WASD
    
    for (int i=0; i<4; i++) {           //initilaise shadow grids
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
            gridCopy[i][j]=0;
        }
    }
    
    while (gameOver(dir, grid) == false) {      //continues running untill gameOver returns TRUE
        cin >>dir;
        updateGrid(dir, grid, gridEmpty, gridChange, gridCopy);
    }
    
    cout << "game over" << endl;        //print 'game over' when gameOver returns TRUE
    
    
    return 0;
}

void updateGrid(string dir, int grid[][4], int gridEmpty[][4], int gridChange[][4], int gridCopy[][4]){
    //deals with fully updating the grid after every move.
    //from the directional move to generating the random 2 and checking if certain
    //conditions are met in order to procede with the final printing of the grid.
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridCopy[i][j] = grid[i][j];
        }
    }
    
    if (dir == "w") {               //w corresponds to a shift up
        moveUp(grid, gridChange);
    } else if (dir == "s"){         //s is a shift down
        moveDown(grid, gridChange);
    } else if (dir == "a"){         //a is a shift left
        moveLeft(grid, gridChange);
    } else if (dir == "d"){         //d is a shift right
        moveRight(grid, gridChange);
    }
    
    int change=0;
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            gridEmpty[i][j]=0;
        }
    }
    
    bool no_move = noMove(grid, gridCopy);
    
    srand((unsigned)time(NULL));
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (grid[i][j]==0) {
                gridEmpty[i][j]=1;      //posible empty elements in the grid to place the randomly generated 2
            }
        }
    }
    do {
        int numi=rand()%4;      //generate a random number between 1 and 4
        int numj=rand()%4;
        //cout << numi << "\t" << numj <<endl; //used for debugging
        if (gridEmpty[numi][numj]==1 && change==0 && wrongInput(dir)==false && no_move==false) {
            grid[numi][numj]=2;     //place the 2, in the randomly chosen posiiton
            change++;
        }
    } while (change==0 && no_move==false);
    
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            gridEmpty[i][j]=0;
        }
    }
    
    if (no_move==false) {
        printGrid(grid);
    }
    
}

void printGrid(int grid[][4]){
    //is used to print the grid to the user
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            cout << grid[i][j] << "\t";
        }
            cout << endl;
    }
}

void moveUp(int grid[][4], int gridChange[][4]){
    //deals with everything in this specific direction.
    //checks for merges first then moves
    //all four direction functions are similar
    for (int i=0; i<4; i++) {           //no changes have been mage before the turn, therefor set gridChange to 0
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
    for (int k=0; k<5; k++) {
        for (int j=0; j<4; j++) {
            for (int i=0; i<3; i++) {
                if (grid[i][j]==grid[i+1][j] && gridChange[i+1][j]==0 && grid[i][j]!=0) {       //checks for merges
                    grid[i][j]=2*grid[i][j];
                    grid[i+1][j]=0;
                    gridChange[i][j]=1;
                    gridChange[i+1][j]=1;
                }
                if (grid[i][j]==0 && grid[i+1][j]!=0) {     //shifts up
                    grid[i][j]=grid[i+1][j];
                    grid[i+1][j]=0;
                }
            }
        }
    }
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
}

void moveDown(int grid[][4], int gridChange[][4]){
    //similar to moveUp();
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
    for (int k=0; k<5; k++) {
    for (int j=0; j<4; j++) {
        for (int i=3; i>0; i--) {
            if (grid[i][j]==grid[i-1][j] && gridChange[i-1][j]==0 && grid[i][j]!=0) {
                grid[i][j]=2*grid[i][j];
                grid[i-1][j]=0;
                gridChange[i][j]=1;
                gridChange[i-1][j]=1;
            }
            if (grid[i][j]==0 && grid[i-1][j]!=0) {
                grid[i][j]=grid[i-1][j];
                grid[i-1][j]=0;
            }
        }
    }
    }
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
}

void moveLeft(int grid[][4], int gridChange[][4]){ //change for LEFT
    //similar to moveUp();
    //however the for loops are inverted
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
    for (int k=0; k<5; k++) {
        for (int i=0; i<4; i++) {
            for (int j=0; j<3; j++) {
                if (grid[i][j]==grid[i][j+1] && gridChange[i][j+1]==0 && grid[i][j]!=0) {
                    grid[i][j]=2*grid[i][j];
                    grid[i][j+1]=0;
                    gridChange[i][j]=1;
                    gridChange[i][j+1]=1;
                }
                if (grid[i][j]==0 && grid[i][j+1]!=0) {
                    grid[i][j]=grid[i][j+1];
                    grid[i][j+1]=0;
                }
            }
        }
    }
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
}

void moveRight(int grid[][4], int gridChange[][4]){
    //similar to moveLeft();
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
    for (int k=0; k<5; k++) {
        for (int i=0; i<4; i++) {
            for (int j=3; j>0; j--) {
                if (grid[i][j]==grid[i][j-1] && gridChange[i][j-1]==0 && grid[i][j]!=0) {
                    grid[i][j]=2*grid[i][j];
                    grid[i][j-1]=0;
                    gridChange[i][j]=1;
                    gridChange[i][j-1]=1;
                }
                if (grid[i][j]==0 && grid[i][j-1]!=0) {
                    grid[i][j]=grid[i][j-1];
                    grid[i][j-1]=0;
                }
            }
        }
    }
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            gridChange[i][j]=0;
        }
    }
    
}

bool wrongInput(string dir){
    //checks to see if input other than 'wasd' is used.
    //if user does enter wrong input, the grid shouldn't update or print
    if (dir!="w") {
        if (dir!="a") {
            if (dir!="s") {
                if (dir!="d") {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool noMove (int grid[][4], int gridCopy[][4]){
    // used to check if grid is the same as before a move.
    //if it is, don't print and dont generate random 2
    int count=0;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (gridCopy[i][j]==grid[i][j]) {
                count++;
            }
        }
    }
    if (count==16) {
        return true;
    }
    
    return false;
}

bool gameOver(string dir, int grid[][4]){
    //game over condition
    int end=0;
    for (int j=0; j<4; j++) {
        for (int i=0; i<3; i++) {
            if (grid[i][j]!=grid[i+1][j] && grid[i][j]!=0 && grid[i+1][j]!=0) { //checks to see whether any merges are possible or if there are any 0 elements in the grid
                end++;
            }
        }
    }
    
    for (int i=0; i<4; i++) {
        for (int j=0; j<3; j++) {
            if (grid[i][j]!=grid[i][j+1] && grid[i][j]!=0 && grid[i][j+1]!=0) {
                end++;
            }
        }
    }
    
    if (end==24) {      //each nested for loop has 12 checks, with 2 nested for loops, end must equal 24 to return TRUE
        return true;
    }
    
    if (dir == "end") {
        return true;
    }
    
    return false;
}






