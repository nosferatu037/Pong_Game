/* 
 * File:   main.cpp
 * Author: nosferatu
 *
 * Created on June 26, 2015, 3:32 PM
 */

#include <cstdlib>
#include <ncurses.h>
#include <iostream>
#include "game.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Game gameRun;
    gameRun.run();
    
//    srand(time(NULL));
//    int t = 10;
//    while(t > 0)
//    {
//        cout << 1 - 2 * ((rand() % 100)/100.0) << endl;
//        t--;
//    }
    
    return 0;
}

