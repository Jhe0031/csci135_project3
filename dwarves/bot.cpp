/*
Author: Jia Qi He
Course: CSCI-135
Instructor: Genady Maryash
Assignment: Project 3.A

Help the dwarf collect at least 100 lumber in 18 hours
*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "bot.h"

using namespace std;

const int MAX_ROWS = 40;
const int MAX_COLS = 40;
const int MAX_NUM = 10;

int ROWS;  // global variables
int COLS;
int NUM;

/* onStart:
An Initialization procedure called at the start of the game.
You can use it to initialize certain global variables, or do
something else before the actual simulation starts.
Parameters:
    rows: number of rows
    cols: number of columns
    num:  number of dwarfs
    log:  a cout-like log */

void onStart(int rows, int cols, int num, std::ostream &log) {
  log << "Start!" << endl; // Print a greeting message

  ROWS = rows; // Save values in global variables
  COLS = cols;
  NUM = num;
}

/* onAction:
A procedure called each time an idle dwarf is choosing
their next action.
Parameters:
    dwarf:   dwarf choosing an action
    day:     day (1+)
    hours:   number of hours in 24-hour format (0-23)
    minutes: number of minutes (0-59)
    log:     a cout-like log  */

bool isNextToATree(Dwarf & dwarf, int r, int c) {
  if (dwarf.look(r, c) == PINE_TREE) {
    return true;
  } else if (dwarf.look(r, c) == APPLE_TREE) {
    return true;
  } else {
    return false;
  }
}

void onAction(Dwarf &dwarf, int day, int hours, int minutes, ostream &log) {
  // Get current position of the dwarf
  int r = dwarf.row();
  int c = dwarf.col();

  // If there is a pine/apple tree, chop it
  if (isNextToATree(dwarf, r-1, c) == true) {
    log << "Found a tree -- chop" << endl;
    dwarf.start_chop(NORTH);
    return;
  } else if (isNextToATree(dwarf, r+1, c) == true) {
    log << "Found a tree -- chop" << endl;
    dwarf.start_chop(SOUTH);
    return;
  } else if (isNextToATree(dwarf, r, c+1) == true) {
    log << "Found a tree -- chop" << endl;
    dwarf.start_chop(EAST);
    return;
  } else if (isNextToATree(dwarf, r, c-1) == true) {
    log << "Found a tree -- chop" << endl;
    dwarf.start_chop(WEST);
    return;
  } else {
    // Find the first tree and continue chopping by rows
    for (int i=0; i < ROWS; i++) {
      for (int j=0; j < COLS; j++) {
        if (dwarf.look(i, j) == PINE_TREE) {
          if (j == 0) {
            while (dwarf.look(i, j) == PINE_TREE || dwarf.look(i, j) == APPLE_TREE) {
              j = j+1;
            }
          } else {
            j = j-1;
          }
          log << "Walk to " << i << " " << j << endl;
          dwarf.start_walk(i, j);
          return;
        } else if (dwarf.look(i, j) == APPLE_TREE) {
          if (j == 0) {
            while (dwarf.look(i, j) == PINE_TREE || dwarf.look(i, j) == APPLE_TREE) {
              j = j+1;
            }
          } else {
            j = j-1;
          }
          log << "Walk to " << i << " " << j << endl;
          dwarf.start_walk(i, j);
          return;
        }
      }
    }
  }
}
