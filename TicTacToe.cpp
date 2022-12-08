/* Christine Ta
 * Professor James Hamblen
 * ECE 4180: Embedded Systems Design
 * Date Last Modified: 15 November 2022
 * 
 * Description: Final Project v1 C++ only
 * Basic Tic-Tac-Toe game with two players
 * 
 * TO-DO:
 *      - ADD LCD/Bluetooth capability.
 */

//MBED MATERIAL

// #include <mbed.h>
// #include <mpr121.h>

// //Interrupt receiver obj on pin 26 and pin 25
// InterruptIn interrupt(p26);
// InterruptIn interrup(p25);

// //I2C for capacitive touchpad 1
// I2C i2c1(p9, p10);
// //I2C for capacitive touchpad 2
// I2C i2c2(p28, p27);

// //Setup capacitive touchpads 1 and 2
// Mpr121 mpr121_touch1(&i2c1, Mpr121::ADD_VSS);
// Mpr121 mpr121_touch2(&i2c2, Mpr121::ADD_VSS);

// void fallInterrupt() {
//     int key_code = 0;
//     int i = 0;
//     int value = mpr121_touch1.read(0x00);
//     value += mpr121_touch1.read(0x01) << 8;
// }

// int main() {
//     interrupt.fall(&fallInterrupt);
//     interrupt.mode(PullUp);
//     while(1) {}
// }

#include <stdlib.h>
#include <iostream>
using namespace std;

//Board is a 3 x 3
#define N 3

//Initialise board with 'blanks'
char board[N][N] = {{'1', '2', '3'},
                    {'4', '5', '6'},
                    {'7', '8', '9'}};
int choice, row, col, turn;
bool draw = false;

//Print board
void displayBoard() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << " | " << board[i][j] << " | ";
        }
        cout << endl;
        cout << " -------------------" << endl;
    }
}

void playerTurn() {
    if(turn == 0) {
        cout << "Player 1's turn [X]: " << endl;
    }
    if(turn == 1) {
        cout << "Player 2's turn [O]: " << endl;
    }

    //Input from user
    cin >> choice;

    switch(choice) {
        case 1: 
            row = 0; col = 0; break;
        case 2:
            row = 0; col = 1; break;
        case 3: 
            row = 0; col = 2; break;
        case 4:
            row = 1; col = 0; break;
        case 5: 
            row = 1; col = 1; break;
        case 6:
            row = 1; col = 2; break;
        case 7: 
            row = 2; col = 0; break;
        case 8:
            row = 2; col = 1; break;
        case 9: 
            row = 2; col = 2; break;
        default:
            break;
    }

    //If location already has X or O, invalid move.
    //If location is empty, place X/O there.
    if(turn == 0 && board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = 'X';
        turn = 1;
    } else if(turn == 1 && board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = 'O';
        turn = 0;
    } else {
        cout << "Invalid move" << endl;
        playerTurn();
    }

    displayBoard();
}

bool victoryCond() {
    for(int i = 0; i < N; i++) {
        //Check rows and columns for win
        if(board[i][0] == board[i][1] && board[i][0] == board[i][2] ||
            board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
                return false;
        }
    }

    //Diagonals
    if(board[0][0] == board[1][1] && board[0][0] == board[2][2] ||
        board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
            return false;
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(board[i][j] != 'X' && board[i][j] != 'O') {
                return true;
            }
        }
    }

    return false;
}

int main() {
    cout << "Tic Tac Toe" << endl;
    cout << "Player 1 is X, Player 2 is O" << endl;

    while(victoryCond()) {
        displayBoard();
        playerTurn();
        victoryCond();
    }

    if(turn == 0 && draw == false) {
        cout << "Player 1 wins" << endl;
    } else if(turn == 1 && draw == false) {
        cout << "Player 2 wins" << endl;
    } else {
        cout << "It's a draw" << endl;
    }
}