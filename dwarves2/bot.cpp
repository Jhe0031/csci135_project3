/*
Author: Jia Qi He
Course: CSCI-135
Instructor: Genedy Maryash
Assignment: Project 3B

Chop trees in all directions and build defense*/

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
int enough_wood = 300;

enum Section {CHOPWOOD, BUILDFENCE};
Section current_section = CHOPWOOD;

int setup_check[] = {0, 0, 0, 0, 0, 0};
enum Build_Section {WALK_BS, BUILD_BS, SETUP_BS};
Build_Section current_bs = SETUP_BS;

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

bool isNextToATree (Dwarf &dwarf, int r, int c) {
  if ((dwarf.look(r, c) == PINE_TREE) || (dwarf.look(r, c) == APPLE_TREE)) {
    return true;
  } else {
    return false;
  }
}

void onAction(Dwarf &dwarf, int day, int hours, int minutes, ostream &log) {
  // Get current position of the dwarf
  int r = dwarf.row();
  int c = dwarf.col();
  // First phase: Collect enough wood to build the fences
  // Second phase: Build at least 30 fences

  // Switch Section
  if (dwarf.lumber() > enough_wood) {
    log << "Building fences" << endl;
    current_section = BUILDFENCE; // keep collecting wood until goal is reached
  }
  switch(current_section) {
    case CHOPWOOD: {
      if (isNextToATree(dwarf, r, c+1)) {
        // If there is a tree, chop it
        log << "Found a tree -- chop" << endl;
        dwarf.start_chop(EAST);
        return;
      } else if (isNextToATree(dwarf, r, c-1)) {
        // If there is a tree, chop it
        log << "Found a tree -- chop" << endl;
        dwarf.start_chop(WEST);
        return;
      } else if (isNextToATree(dwarf, r+1, c)) {
        // If there is a tree, chop it
        log << "Found a tree -- chop" << endl;
        dwarf.start_chop(SOUTH);
        return;
      } else if (isNextToATree(dwarf, r-1, c)) {
        // If there is a tree, chop it
        log << "Found a tree -- chop" << endl;
        dwarf.start_chop(NORTH);
        return;
      } else {
        // Otherwise, move to a random location
        int rr = 0; int cc = 0;
        if ((isNextToATree(dwarf, r+1, c+1)) || (isNextToATree(dwarf, r+2, c))) {
          rr = (r+1);
          cc = c;
        } else if ((isNextToATree(dwarf, r-1, c-1)) || (isNextToATree(dwarf, r-2, c))) {
          rr = (r-1);
          cc = c;
        } else if (isNextToATree(dwarf, r, c+2)) {
          rr = r;
          cc = (c+1);
        } else if (isNextToATree(dwarf, r, c-2)) {
          rr = r;
          cc = (c-1);
        } else if (isNextToATree(dwarf, r+2, c+2)) {
          rr = (r+2);
          cc = c;
        } else if (isNextToATree(dwarf, r-2, c-2)) {
          rr = (r-2);
          cc = c;
        } else {
          rr = rand() % ROWS;
          cc = rand() % COLS;
        }
        log << "Walk to " << rr << " " << cc << endl;
        dwarf.start_walk(rr, cc);
        return;
      }
      break;
    }
    // Building fences section
    case BUILDFENCE : {
      // Check to see if dwarf is in starting position
      if (setup_check[dwarf.name()] == 0) {
        current_bs = SETUP_BS;
      } else {
        // First chop away trees. If not blocked, build or walk
        if (isNextToATree(dwarf, r, c+1)) {
          dwarf.start_chop(EAST);
        } else if (isNextToATree(dwarf, r, c-1)) {
          dwarf.start_chop(WEST);
        } else if (dwarf.look(r, c-1) != FENCE) {
          current_bs = BUILD_BS;
        } else if (dwarf.look(r, c-1) == FENCE) {
          current_bs = WALK_BS;
        }
      }
      // Possible build section action
      switch(current_bs) {
        case WALK_BS : {
          // After building wall, move over
          dwarf.start_walk((r), (++c));
          log << dwarf.name() << "/ - Building - WALK" << endl;
          break;
        }
        case BUILD_BS : {
          if ((r == (dwarf.name() + 5)) && (c < 12)) {
            // Build wall to the left of every step
            if (isNextToATree(dwarf, r-1, c)) {
              dwarf.start_chop(WEST);
            } else if (isNextToATree(dwarf, r+1, c)) {
              dwarf.start_chop(EAST);
            } {
              dwarf.start_build(WEST);
            }
            log << dwarf.name() << "/ - Building - BUILD" << endl;
          } else {
            dwarf.start_walk(dwarf.name(), c);
            log << r << " " << c << "Not in right spot" << endl;
          }
          break;
        }
        case SETUP_BS : {
          // Try to walk to starting spot
          if (isNextToATree(dwarf, dwarf.name() + 5, 5)) {
              dwarf.start_walk(dwarf.name() + 5, 4);
              log << dwarf.name() << "-Tree- Move left" << endl;
            } else if (isNextToATree(dwarf, dwarf.name() + 5, 4)) {
              dwarf.start_walk(dwarf.name() + 5, 6);
              log << dwarf.name() << "-Tree- Move right" << endl;
            } else {
              dwarf.start_walk(dwarf.name() + 5, 5);
              log << dwarf.name() << "-Tree- Move left" << endl;
            }
            if ((r == dwarf.name() + 5) && (c == 5)) {
              // If setup is successful, update dwarf
              setup_check[dwarf.name()] = 1;
            } else {
              // If not try to spot left and chop tree
              dwarf.start_walk(dwarf.name() + 5, 4);
            }
            log << dwarf.name() << "/ - Building - SETUP" << endl;
            break;
          }
        }
        break;
      }
    }
  }