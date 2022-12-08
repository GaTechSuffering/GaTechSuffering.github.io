/* Christine Ta
 * Professor James Hamblen
 * ECE 4180: Embedded Systems Design
 * Date Last Modified: 30 November 2022
 * 
 * Description: Final Project v1 C++ only
 * Basic Connect4 game with two players
 */

#include <stdlib.h>
#include <iostream>
using namespace std;

//Board can be arbitrary row x col
#define ROW 5
#define COL 4

char board[ROW][COL];
int choiceP1, choiceP2;
bool player1Turn = true;
bool drawCon = false;

//Initialise board
void InitialiseBoard() {
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            board[i][j] = 'x';
        }
    }
}

//Print board
void displayBoard() {
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            cout << " " << board[i][j];
        }
        cout << endl;
    }
}

bool victoryCond() {
    int Rcount = 0;
    int Ycount = 0;

    //Horizontal Check
    //Player 1
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board[i][j] == 'R') {
                Rcount++;
                if(Rcount == 4) {
                    return true;
                }
            } else {
                Rcount = 0;
            }
        }
        Rcount = 0;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Player 2
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board[i][j] == 'Y') {
                Ycount++;
                if(Ycount == 4) {
                    return true;
                }
            } else {
                Ycount = 0;
            }
        }
        Ycount = 0;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Vertical Check
    for(int j = 0; j < COL; j++) {
        for(int i = 0; i < ROW; i++) {
            if(board[i][j] == 'R') {
                Rcount++;
                if(Rcount == 4) {
                    return true;
                }
            } else {
                Rcount = 0;
            }
        }
        Rcount = 0;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    for(int j = 0; j < COL; j++) {
        for(int i = 0; i < ROW; i++) {
            if(board[i][j] == 'Y') {
                Ycount++;
                if(Ycount == 4) {
                    return true;
                }
            } else {
                Ycount = 0;
            }
        }
        Ycount = 0;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;
    
    //Forward Diagonals
    int row = ROW;
    int col = COL;
    while(row >= 0 && col >= 0) {
        if(board[row][col] == 'R') {
            Rcount++;
            if(Rcount == 4) {
                return true;
            }
        } else {
            Rcount = 0;
        }
        row--;
        col--;
    }

    //Resets variables
    row = 0;
    col = 0;
    while(row <= ROW && col <= COL) {
        if(board[row][col] == 'R') {
            Rcount++;
            if(Rcount == 4) {
                return true;
            }
        } else {
            Rcount = 0;
        }
        row++;
        col++;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Backward Diagonals
    row = 0;
    col = COL;
    while (row <= ROW && col >= 0) {
        if (board[row][col] == 'R') {
            Rcount++;
            if (Rcount == 4) {
                return true;
            }
        } else {
            Rcount = 0;
        }
        row++;
        col--;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Resets variables
    row = -1;
    col = COL-1;
    while (row <= ROW && col >= 0) {
        if (board[row][col] == 'R') {
            Rcount++;
            if (Rcount == 4) {
                return true;
            }
        } else {
            Rcount = 0;
        }
        row++;
        col--;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Player 2
    //Forward Diagonals
    row = ROW;
    col = COL;
    while(row >= 0 && col >= 0) {
        if(board[row][col] == 'Y') {
            Ycount++;
            if(Ycount == 4) {
                return true;
            }
        } else {
            Ycount = 0;
        }
        row--;
        col--;
    }

    //Resets variables
    row = 0;
    col = 0;
    while(row <= ROW && col <= COL) {
        if(board[row][col] == 'Y') {
            Ycount++;
            if(Ycount == 4) {
                return true;
            }
        } else {
            Ycount = 0;
        }
        row++;
        col++;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Backwards Diagonal
    row = 0;
    col = COL;
    while (row <= ROW && col >= 0) {
        if (board[row][col] == 'W') {
            Ycount++;
            if (Ycount == 4) {
                return true;
            }
        } else {
            Ycount = 0;
        }
        row++;
        col--;
    }

    //Resets values for next check
    Ycount = 0;
    Rcount = 0;

    //Resets variables
    row = -1;
    col = COL;
    while (row <= ROW && col >= 0) {
        if (board[row][col] == 'W') {
            Ycount++;
            if(Ycount == 4) {
                return true;
            }
        } else {
            Ycount = 0;
        }
        row++;
        col--;
    }

    //For when there is a draw
    int fillBoard = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board[i][j] == 'R' || board[i][j] == 'Y') {
                fillBoard++;
            }
        }
    }
    if(fillBoard == ROW*COL) {
        drawCon = true;
        return true;
    }

    return false;
}

//Updates the board
void updateBoard(int choice) {
    if(player1Turn == true) {
        for(int i = 0; i < ROW; i++) {
            if(board[i][choice] == 'R' || board[i][choice] == 'Y') {
                return;
            }
            if(board[i+1][choice] != 'x') {
                board[i][choice] = 'R';
                bool win = victoryCond();
                if(drawCon == true) {
                    cout << "This is a draw!" << endl;
                    return;
                }
                if(win == false) {
                    player1Turn = false;
                }
                return;
            }
        }
    } 
    if(player1Turn == false) {
        for(int i = 0; i < ROW; i++) {
            if(board[i][choice] == 'R' || board[i][choice] == 'Y') {
                return;
            }
            if(board[i+1][choice] != 'x') {
                board[i][choice] = 'Y';
                bool win = victoryCond();
                if(drawCon == true) {
                    cout << "This is a draw!" << endl;
                    return;
                }
                if(win == false) {
                    player1Turn = true;
                }
                return;
            }
        }
    }
}

void p1Turn() {
    if(player1Turn == true) {
        cout << "Player 1's turn [R]: " << endl;

        //Input from user
        cin >> choiceP1;

        if(choiceP1 >= COL || choiceP1 < 0) {
            cout << "Illegal move!\n"; 
        } else {
            updateBoard(choiceP1);
        }
    }
}

void p2Turn() {
    if(player1Turn == false) {
        cout << "Player 2's turn [Y]: " << endl;

        //Input from user
        cin >> choiceP2;

        if(choiceP2 >= COL || choiceP2 < 0) {
            cout << "Illegal move!\n"; 
        } else {
            updateBoard(choiceP2);
        }
    }
}

int main() {
    cout << "Connect 4" << endl;
    cout << "Player 1 is R, Player 2 is Y" << endl;

    InitialiseBoard();
    displayBoard();

    while(!victoryCond()) {
        p1Turn();
        displayBoard();
        p2Turn();
        displayBoard();
    }

    if(player1Turn == true && drawCon == false) {
        cout << "Player 1 wins" << endl;
    } else if(player1Turn == false && drawCon == false) {
        cout << "Player 2 wins" << endl;
    } else {
        cout << "It's a draw!" << endl;
    }
}