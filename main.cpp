#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "SongPlayer.h"
#include "MMA8452.h"
#include <string>
#include <list>
#include <mpr121.h>
#define ROW 5
#define COL 4

//TicTacToe and Connect 4 variables
Serial blue(p28,p27);
uLCD_4DGL uLCD(p13,p14,p11);
I2C i2c1(p9, p10);
Mpr121 mpr121(&i2c1, Mpr121::ADD_VSS);
Mpr121 mpr1212(&i2c1, Mpr121::ADD_VDD);
InterruptIn interrupt(p26);
InterruptIn interrupt2(p25);
PinDetect pb0(p24);
PinDetect pb1(p23);
PinDetect pb2(p23);
SongPlayer mySpeaker(p21);
volatile bool xcount = false;
volatile bool ocount = false;
volatile bool drawCon = false;
volatile bool rcount = false;
volatile bool wcount = false;
volatile bool t = false;
volatile bool c = false;
volatile bool a = false;
volatile char bnum = 0;
volatile int xam = 0;
volatile int oam = 0;
volatile char arr[3][3] = {{'a', 'b', 'c'},
                           {'d', 'e', 'f'},
                           {'g', 'h', 'i'}};
                           
char board[5][4] = {{'x', 'x', 'x', 'x'},
                    {'x', 'x', 'x', 'x'},
                    {'x', 'x', 'x', 'x'},
                    {'x', 'x', 'x', 'x'},
                    {'x', 'x', 'x', 'x'}};
                    
//Speaker variables
float note1[1]= {1568.0};
float duration1[1]= {0.24};
float note2[1]= {1396.9};
float duration2[1]= {0.24};
int choiceP1, choiceP2;
bool player1Turn = true;

//Play Archery variables
Serial pc(USBTX,USBRX);
PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
MMA8452 acc(p9, p10, 40000);
volatile bool firstTime = true;

//Creates the board for TicTacToe
void createboardtic() {
    uLCD.color(WHITE);
    uLCD.printf("TIC TAC TOE!!!\n");
    uLCD.line(42, 10, 42, 127, 0xFFFFFF);
    uLCD.line(84, 10, 84, 127, 0xFFFFFF);
    uLCD.line(0, 47, 127, 47, 0xFFFFFF);
    uLCD.line(0, 89, 127, 89, 0xFFFFFF);
}

//Creates the board for Connect 4
void createboardcon() {
    uLCD.color(WHITE);
    uLCD.printf("CONNECT 4!!!\n");
    uLCD.line(1, 10, 1, 127, 0xFFFFFF);
    uLCD.line(32, 10, 32, 127, 0xFFFFFF);
    uLCD.line(63, 10, 63, 127, 0xFFFFFF);
    uLCD.line(94, 10, 94, 127, 0xFFFFFF);
    uLCD.line(126, 10, 126, 127, 0xFFFFFF);
    
    uLCD.line(1, 10, 127, 10, 0xFFFFFF);
    uLCD.line(1, 33, 127, 33, 0xFFFFFF);
    uLCD.line(1, 56, 127, 56, 0xFFFFFF);
    uLCD.line(1, 79, 127, 79, 0xFFFFFF);
    uLCD.line(1, 101, 127, 101, 0xFFFFFF);
    uLCD.line(1, 127, 127, 127, 0xFFFFFF);
}

//Creates the board for Archery
void createBoardAr() {
    uLCD.color(WHITE);
    uLCD.cls();
    uLCD.circle(128/2, 128/2, 10, WHITE);
    uLCD.circle(128/2, 128/2, 20, WHITE);
    uLCD.circle(128/2, 128/2, 30, WHITE);
    uLCD.circle(128/2, 128/2, 40, WHITE);
    uLCD.circle(128/2, 128/2, 50, WHITE);
    uLCD.circle(128/2, 128/2, 60, WHITE);
    uLCD.circle(128/2, 128/2, 70, WHITE);
    uLCD.circle(128/2, 128/2, 80, WHITE);
}

//Calculates the score for Archery
int calculateScore(double distance, int score) {
    //distance *= 10;
    if(distance <= 10.0) score += 10;
    else if(distance <= 20.0 && distance > 10.0) score += 9;
    else if(distance <= 30.0 && distance > 20.0) score += 8;
    else if(distance <= 40.0 && distance > 30.0) score += 7;
    else if(distance <= 50.0 && distance > 40.0) score += 6;
    else if(distance <= 60.0 && distance > 50.0) score += 5;
    else if(distance <= 70.0 && distance > 60.0) score += 4;
    else if(distance <= 80.0 && distance > 70.0) score += 3;
    else score += 1;
    
    return score;
}

//Archery game
void playar() {
    pb0.mode(PullUp);
    double x, y, z, x_adj, y_adj, z_adj;
    bool player1TurnArch = true;
    int push_pb = 0;
    double distance = 0.0;
    double new_x, new_y;
    int rounds1 = 0;
    int rounds2 = 0;
    int score1 = 0;
    int score2 = 0;
    int d = 500;
    int r = 63;
    int i = 0;
    //createBoardAr();
    //firstTime = false;
    createBoardAr(); 
    while(1) {  
        i++;   
        uLCD.filled_circle(-1*x*d+r, -1*y*d+r, 2, BLACK);
        if(!acc.isXYZReady()) 
        {
            wait(0.01);
        }
        else
        { 
        acc.readXYZGravity(&x,&y,&z);
        led1 = abs(x);
        led2 = abs(y);
        led3 = abs(z);
        //wait(.25);
        uLCD.filled_circle(-1*x*d+r, -1*y*d+r, 2, RED);
        new_x = x*d;
        new_y = y*d;
        /*
        if(new_x > 128) {
            uLCD.filled_circle(128, new_y+r, 5, RED);
        } else if(new_x < 0) {
            uLCD.filled_circle(0, new_y+r, 5, RED);
        } else if(new_y > 128) {
            uLCD.filled_circle(new_x+r, 128, 5, RED);
        } else if(new_y < 0) {
            uLCD.filled_circle(new_x+r, 0, 5, RED);
        } else {
            uLCD.filled_circle(new_x+r, new_y+r, 5, RED);
        }
        */
        }
        if (i == 40)
        {
            createBoardAr();
            i = 0;
        }
        push_pb = pb0;
      
        if(push_pb == 0 && player1TurnArch == true) {
            mySpeaker.PlaySong(note2,duration2);
            x_adj = abs(new_x);
            y_adj = abs(new_y);
            //z_adj = abs(z);
            distance = (sqrt(x_adj*x_adj + y_adj*y_adj));
            player1TurnArch = false;
            rounds1++;
            score1 = calculateScore(distance, score1);
            push_pb = 1;
            uLCD.cls();
            uLCD.printf("Player 1's score: %d\n", score1);
            wait(2.0);
            createBoardAr();
        }
      
        if(push_pb == 0 && player1TurnArch == false) {
            mySpeaker.PlaySong(note1,duration1);
            x_adj = abs(new_x);
            y_adj = abs(new_y);
            //z_adj = abs(z);
            distance = (sqrt(x_adj*x_adj + y_adj*y_adj));
            player1TurnArch = true;
            rounds2++;
            score2 = calculateScore(distance, score2);
            push_pb = 1;
            uLCD.cls();
            uLCD.printf("Player 2's score: %d\n", score2);
            wait(2.0);
            createBoardAr();
        }
      
        if(rounds1 == 3 && rounds2 == 3) { 
            uLCD.cls();
            if(score1 > score2) uLCD.printf("Player 1 wins!");
            if(score1 < score2) uLCD.printf("Player 2 wins!");
            if(score1 == score2) uLCD.printf("It's a draw!");
            wait(2.0);
            break;
        }
    }
}

//Victory condition for Connect 4
bool victoryCondcon() {
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
            if(board[i][j] == 'W') {
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
            if(board[i][j] == 'W') {
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
        if(board[row][col] == 'W') {
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
        if(board[row][col] == 'W') {
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
            if(board[i][j] == 'R' || board[i][j] == 'W') {
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

//Places red dots for Connect 4
void placer(int value) {
    if (wcount == false) {
        switch(value) {
            case 1:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][0] == 'x') {
                        board[i][0] = 'R';
                        uLCD.filled_circle(15, ((i * 23) + 22), 3, WHITE);
                        break;
                    } else if (i == 0 and board[i][0] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 2:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][1] == 'x') {
                        board[i][1] = 'R';
                        uLCD.filled_circle(45, ((i * 23) + 22), 3, WHITE);
                        break;
                    } else if (i == 0 and board[i][1] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 3:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][2] == 'x') {
                        board[i][2] = 'R';
                        uLCD.filled_circle(75, ((i * 23) + 22), 3, WHITE);
                        break;
                    } else if (i == 0 and board[i][2] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 4:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][3] == 'x') {
                        board[i][3] = 'R';
                        uLCD.filled_circle(105, ((i * 23) + 22), 3, WHITE);
                        break;
                    } else if (i == 0 and board[i][3] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            default:
                break;
        }
    }
    wcount = true;
    rcount = false;
}

//Places white dots for Connect 4
void placew(int value) {
    if (rcount == false) {
        switch(value) {
            case 12:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][0] == 'x') {
                        board[i][0] = 'W';
                        uLCD.filled_circle(15, ((i * 23) + 22), 3, RED);
                        break;
                    } else if (i == 0 and board[i][0] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 11:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][1] == 'x') {
                        board[i][1] = 'W';
                        uLCD.filled_circle(45, ((i * 23) + 22), 3, RED);
                        break;
                    } else if (i == 0 and board[i][1] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 10:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][2] == 'x') {
                        board[i][2] = 'W';
                        uLCD.filled_circle(75, ((i * 23) + 22), 3, RED);
                        break;
                    } else if (i == 0 and board[i][2] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            case 9:
                for (int i = 4; i >= 0; i--) {
                    if (board[i][3] == 'x') {
                        board[i][3] = 'W';
                        uLCD.filled_circle(105, ((i * 23) + 22), 3, RED);
                        break;
                    } else if (i == 0 and board[i][3] != 'x') {
                        return;       
                    } else {
                        continue;
                    }
                }
                break;
            default:
                break;
        }
    }
    rcount = true;
    wcount = false;
}

//Places 'x' for TicTacToe
void placex(int value) {
    if (ocount == false) {   
        switch(value) {
            case 11: 
                if ((arr[0][2] == 'x') or (arr[0][2] == 'o')) {
                    return;
                }
                uLCD.line(87, 13, 123, 44, 0xFFFFFF);
                uLCD.line(87, 44, 123, 13, 0xFFFFFF);
                arr[0][2] = 'x';
                break;
            case 10:
                if ((arr[1][2] == 'x') or (arr[1][2] == 'o')) {
                    return;
                }
                uLCD.line(87, 50, 123, 81, 0xFFFFFF);
                uLCD.line(87, 81, 123, 50, 0xFFFFFF);
                arr[1][2] = 'x';
                break;
            case 9:
                if ((arr[2][2] == 'x') or (arr[2][2] == 'o')) {
                    return;
                }
                uLCD.line(87, 87, 123, 118, 0xFFFFFF);
                uLCD.line(87, 118, 123, 87, 0xFFFFFF); 
                arr[2][2] = 'x'; 
                break;
            case 7:
                if ((arr[0][1] == 'x') or (arr[0][1] == 'o')) {
                    return;
                }
                uLCD.line(45, 13, 81, 44, 0xFFFFFF);
                uLCD.line(45, 44, 81, 13, 0xFFFFFF);
                arr[0][1] = 'x';
                break;
            case 6:
                if ((arr[1][1] == 'x') or (arr[1][1] == 'o')) {
                    return;
                }
                uLCD.line(45, 50, 81, 81, 0xFFFFFF);
                uLCD.line(45, 81, 81, 50, 0xFFFFFF); 
                arr[1][1] = 'x';
                break;
            case 5:
                if ((arr[2][1] == 'x') or (arr[2][1] == 'o')) {
                    return;
                }
                uLCD.line(45, 87, 81, 118, 0xFFFFFF);
                uLCD.line(45, 118, 81, 87, 0xFFFFFF);
                arr[2][1] = 'x';
                break;
            case 3: 
                if ((arr[0][0] == 'x') or (arr[0][0] == 'o')) {
                    return;
                }
                uLCD.line(3, 13, 39, 44, 0xFFFFFF);
                uLCD.line(3, 44, 39, 13, 0xFFFFFF);
                arr[0][0] = 'x';
                break;
            case 2:
                if ((arr[1][0] == 'x') or (arr[1][0] == 'o')) {
                    return;
                }
                uLCD.line(3, 50, 39, 81, 0xFFFFFF);
                uLCD.line(3, 81, 39, 50, 0xFFFFFF);
                arr[1][0] = 'x';
                break;
            case 1: 
                if ((arr[2][0] == 'x') or (arr[2][0] == 'o')) {
                    return;
                }
                uLCD.line(3, 87, 39, 118, 0xFFFFFF);
                uLCD.line(3, 118, 39, 87, 0xFFFFFF);
                arr[2][0] = 'x';
                break;
            default:
                break;
        }
        xcount = false;
        ocount = true;
    }
}

//Places 'o' for TicTacToe
void placeo(int value) {
    if (xcount == false) {
        switch(value) {
            case 1: 
                if ((arr[0][2] == 'x') or (arr[0][2] == 'o')) {
                    return;
                }
                uLCD.circle(105, 28, 16, 0xFFFFFF);
                arr[0][2] = 'o';
                break;
            case 2:
                if ((arr[1][2] == 'x') or (arr[1][2] == 'o')) {
                    return;
                }
                uLCD.circle(105, 66, 16, 0xFFFFFF);
                arr[1][2] = 'o';
                break;
            case 3:
                if ((arr[2][2] == 'x') or (arr[2][2] == 'o')) {
                    return;
                }
                uLCD.circle(105, 102, 16, 0xFFFFFF); 
                arr[2][2] = 'o';
                break;
            case 5:
                if ((arr[0][1] == 'x') or (arr[0][1] == 'o')) {
                    return;
                }
                uLCD.circle(63, 28, 16, 0xFFFFFF);
                arr[0][1] = 'o';
                break;
            case 6:
                if ((arr[1][1] == 'x') or (arr[1][1] == 'o')) {
                    return;
                }
                uLCD.circle(63, 66, 16, 0xFFFFFF);
                arr[1][1] = 'o';
                break;
            case 7:
                if ((arr[2][1] == 'x') or (arr[2][1] == 'o')) {
                    return;
                }
                uLCD.circle(63, 102, 16, 0xFFFFFF);
                arr[2][1] = 'o';
                break;
            case 9: 
                if ((arr[0][0] == 'x') or (arr[0][0] == 'o')) {
                    return;
                }
                uLCD.circle(21, 28, 16, 0xFFFFFF);
                arr[0][0] = 'o';
                break;
            case 10:
                if ((arr[1][0] == 'x') or (arr[1][0] == 'o')) {
                    return;
                }
                uLCD.circle(21, 66, 16, 0xFFFFFF);
                arr[1][0] = 'o';
                break;
            case 11: 
                if ((arr[2][0] == 'x') or (arr[2][0] == 'o')) {
                    return;
                }
                uLCD.circle(21, 102, 16, 0xFFFFFF);
                arr[2][0] = 'o';
                break;
            default:
                break;
        }
        xcount = true;
        ocount = false;
    }
}

//Interrupt 1for capacitive touchpad - works for TicTacToe and Connect 4
void fallInterrupt() { 
    int key_code = 0;
    int value = mpr121.read(0x00);
    value += mpr121.read(0x01)<<8;
    
    for(int i = 0; i < 12; i++) {
        if(((value>>i)&0x01)==1) {
            key_code = i+1;
        }
    }
    if(t == true) {
        placex(key_code);
        mySpeaker.PlaySong(note1,duration1);
    }
    if(c == true) {
        placer(key_code);
        mySpeaker.PlaySong(note1,duration1);
    }
}

//Interrupt 2 for capacitive touchpad - works for TicTacToe and Connect 4
void fallInterrupt2() {
    int key_code = 0;
    int value = mpr1212.read(0x00);
    value += mpr1212.read(0x01)<<8;

    for(int i = 0; i < 12; i++) {
        if (((value>>i)&0x01)==1) {
            key_code = i+1;
        }
    }
    if(t == true){
        placeo(key_code);
        mySpeaker.PlaySong(note2,duration2);
    }
    if (c == true) {
        placew(key_code);
        mySpeaker.PlaySong(note2,duration2);
    }
}

//Victory condition for TicTacToe
bool victoryCond() {
    for(int i = 0; i <= 2 ; i++) {
        //Check rows and columns for win
        if(arr[i][0] == arr[i][1] && arr[i][0] == arr[i][2] ||
            arr[0][i] == arr[1][i] && arr[0][i] == arr[2][i]) {
                return false;
        }
    }

    //Diagonals
    if(arr[0][0] == arr[1][1] && arr[0][0] == arr[2][2] ||
        arr[0][2] == arr[1][1] && arr[0][2] == arr[2][0]) {
            return false;
    }
    
    //For when there is a draw
    int fillBoard = 0;
    for(int i = 0; i <= 2; i++) {
        for(int j = 0; j <= 2; j++) {
            if(arr[i][j] == 'x' || arr[i][j]  == 'o') {
                fillBoard++;
            }
        }
    }
    if(fillBoard == 9) {
        ocount = true;
        xcount = true;
        return false;
    }

    for(int i = 0; i <= 2; i++) {
        for(int j = 0; j <= 2; j++) {
            if(arr[i][j] != 'x' && arr[i][j] != 'o') {
                return true;
            }
        }
    }
}

//Starts TicTacToe game
void playtic() {    
    createboardtic();
    
    while(1) {
        if(victoryCond() == false) {
            uLCD.cls();
            arr[0][0] = 'a'; arr[0][1] = 'b'; arr[0][2] = 'c'; arr[1][0] = 'd'; arr[1][1] = 'e';
            arr[1][2] = 'f'; arr[2][0] = 'g'; arr[2][1] = 'h'; arr[2][2] = 'i';
            if(ocount == false) {
                oam += 1;
                uLCD.printf("PLAYER O WINS!\n");
                if (oam == 2) {
                    uLCD.printf("GAME OVER\nPlayer O Won\nBest of 3");
                    wait(5);
                } else {
                    wait(5);
                    uLCD.cls();
                    playtic();
                }
            } else if(xcount == false) {
                xam += 1;
                uLCD.printf("PLAYER X WINS!\n");
                if (xam == 2) {
                    uLCD.printf("GAME OVER\nPlayer X Won\nBest of 3");
                    wait(5);
                } else {
                    wait(5);
                    uLCD.cls();
                    playtic();
                }
            } else if(ocount == true && xcount == true) {
                 uLCD.printf("It is a draw!!\n");
                 ocount = true;
                 xcount = false;
                 wait(5);
                 uLCD.cls();
                 playtic();
            }
            uLCD.cls();
            //gameselect();
            break;
        }  
        if(victoryCond() == true) {
            continue;
        }     
    }
}

//Starts Connect 4 game
void playcon() {
    createboardcon();
    
    while(1) {
        if(victoryCondcon() == false) {
            continue;
        } else if (victoryCondcon() == true) {
            uLCD.cls();
            board[0][0] = 'x'; board[0][1] = 'x'; board[0][2] = 'x'; board[0][3] = 'x';
            board[1][0] = 'x'; board[1][1] = 'x'; board[1][2] = 'x'; board[1][3] = 'x';
            board[2][0] = 'x'; board[2][1] = 'x'; board[2][2] = 'x'; board[2][3] = 'x';
            board[3][0] = 'x'; board[3][1] = 'x'; board[3][2] = 'x'; board[3][3] = 'x';
            board[4][0] = 'x'; board[4][1] = 'x'; board[4][2] = 'x'; board[4][3] = 'x';
            
            uLCD.cls();
            if(drawCon == true) {
                uLCD.printf("It is a draw!");
                break;
            }
            if(wcount == false) {
                uLCD.printf("Player 1 Won!");
                break;
            }
            if(rcount == false) {
                uLCD.printf("Player 2 Won!");
                break;
            }
            while(1) {
                wait(1);
            }
        }
            
    }
}

//Select game from game menu
void gameselect() {
    int i = 0;
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    uLCD.color(RED);
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.printf(" SELECT\n  GAME");
    uLCD.color(WHITE);
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.printf("\n\n\n\n\n    TIC-TAC-TOE\n\n");
    uLCD.printf("    CONNECT 4\n\n");
    uLCD.printf("    ARCHERY");
    while(1) {
        if(blue.getc() == '!') {
            if(blue.getc() == 'B') {
                bnum = blue.getc();
                if(bnum == '1') {
                    if(i == 1) {
                        uLCD.cls();
                        t = true;
                        c = false;
                        a = false;
                        playtic();
                    } else if(i == 2) {
                        uLCD.cls();
                        t = false;
                        c = true;
                        a = false;
                        playcon();
                    } else if(i == 3) {
                        uLCD.cls();
                        t = false;
                        c = false;
                        a = true;
                        playar();                    
                    }
                }
            }
        }
        else if(pb2 == false) {
            if(i == 1) {
                uLCD.cls();
                t = true;
                c = false;
                a = false;
                playtic();
            } else if(i == 2) {
                uLCD.cls();
                t = false;
                c = true;
                a = false;
                playcon();
            } else if(i == 3) {
                uLCD.cls();
                t = false;
                c = false;
                a = true;
                playar();                    
            }
        }
        if(blue.getc()=='!') {
            if(blue.getc()=='B') {
                bnum = blue.getc();
                if(bnum == '6') {
                    if (i >= 3) {
                        uLCD.color(BLACK);
                        uLCD.locate(1,10);
                        uLCD.printf("--");
                        uLCD.locate(1,6);
                        uLCD.color(RED);
                        uLCD.printf("--");
                        i = 0;
                    }
                    uLCD.color(BLACK);
                    uLCD.locate(1,(6 + (2*(i-1))));
                    uLCD.printf("--");
                    uLCD.locate(1,(6 + (2*i)));
                    uLCD.color(RED);
                    uLCD.printf("--");
                    i+=1;
                } else if(bnum == '5') {
                    if(i <= 1) {
                        uLCD.color(BLACK);
                        uLCD.locate(1,6);
                        uLCD.printf("--");
                        uLCD.locate(1,10);
                        uLCD.color(RED);
                        uLCD.printf("--");
                        i = 3;
                    } else if(i != 1) {
                        uLCD.color(BLACK);
                        uLCD.locate(1,(6 + ((2*i)-2)));
                        uLCD.printf("--");
                        uLCD.locate(1,(6 + ((2*(i-1))-2)));
                        uLCD.color(RED);
                        uLCD.printf("--");
                        i-=1;
                    }
                }
            }
        } else if(pb1 == false) {
            if(i >= 3) {
                uLCD.color(BLACK);
                uLCD.locate(1,10);
                uLCD.printf("--");
                uLCD.locate(1,6);
                uLCD.color(RED);
                uLCD.printf("--");
                i = 0;
            }
            uLCD.color(BLACK);
            uLCD.locate(1,(6 + (2*(i-1))));
            uLCD.printf("--");
            uLCD.locate(1,(6 + (2*i)));
            uLCD.color(RED);
            uLCD.printf("--");
            i+=1;
            
            while(pb1 == false) {
                wait(.1);
                
                if (pb1 == true) {
                    break;
                }
            }
        }
    }
}

int main() {
    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);
    interrupt2.fall(&fallInterrupt2);
    interrupt2.mode(PullUp);
    gameselect();
}