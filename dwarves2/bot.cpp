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
bool start = true;
bool up, down, lefty, righty, startB = false;

bool isNextToATree(Dwarf & dwarf, int r, int c){
  if (dwarf.look(r, c+1) == PINE_TREE || dwarf.look(r, c+1) == APPLE_TREE)
    return true;
  if (dwarf.look(r, c-1) == PINE_TREE || dwarf.look(r, c-1) == APPLE_TREE)
    return true;
  if (dwarf.look(r+1, c) == PINE_TREE || dwarf.look(r+1, c) == APPLE_TREE)
    return true;
  if (dwarf.look(r-1, c) == PINE_TREE || dwarf.look(r-1, c) == APPLE_TREE)
    return true;
  return false;
}

bool isNextToAFence(Dwarf & dwarf, int r, int c){
  if (dwarf.look(r, c+2) == FENCE)
    return true;
  if (dwarf.look(r, c-2) == FENCE)
    return true;
  if (dwarf.look(r+2, c) == FENCE)
    return true;
  if (dwarf.look(r-2, c) == FENCE)
    return true;
  return false;
}
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

void onAction(Dwarf &dwarf, int day, int hours, int minutes, ostream &log) {
  // Get current position of the dwarf
  int r = dwarf.row();
  int c = dwarf.col();

  if(startB && r == 10 && c == 10){
    dwarf.start_build(NORTH);
    startB = false;
    return;
  }
  
  if(dwarf.look(r+1,c) == FENCE){
    dwarf.start_walk(r-1,c);
    // up = false;
    return;
  }else if(dwarf.look(r-1,c) == FENCE){
    dwarf.start_walk(r+1,c);
    // down = false;
    return;
  }else if(dwarf.look(r,c+1) == FENCE){
    dwarf.start_walk(r,c-1);
    // lefty = false;
    return;
  }else if(dwarf.look(r,c-1) == FENCE){
    dwarf.start_walk(r,c+1);
    // righty = false;
    return;
  }
  
  // Look if there is a tree next to the dwarf
  if (isNextToATree(dwarf, r, c)) {
    log << "Found a tree -- chop" << endl;
    // If there is a pine tree, chop it
    if (dwarf.look(r, c+1) == PINE_TREE || dwarf.look(r, c+1) == APPLE_TREE){
      dwarf.start_chop(EAST);
      return;
    }      
    if (dwarf.look(r, c-1) == PINE_TREE || dwarf.look(r, c-1) == APPLE_TREE){
      dwarf.start_chop(WEST);
      return;
    }     
    if (dwarf.look(r+1, c) == PINE_TREE || dwarf.look(r+1, c) == APPLE_TREE){
      dwarf.start_chop(SOUTH);
      return;
    }      
    if (dwarf.look(r-1, c) == PINE_TREE || dwarf.look(r-1, c) == APPLE_TREE){
      dwarf.start_chop(NORTH);
      return;
    } 
  }
  else {   
    int rr, cc;
    //Instead move to where a tree is nearby

    /**Need to mod the additions because it will go out of bounds and then
    choose a random location I use 3 if statements to look in terms of
    columns, rows then both rows and cols

    **/
    for(int i = 1; i < ROWS; i++){

      if(isNextToATree(dwarf, (r+i) % ROWS, (c+i) % COLS) &&
   !(dwarf.look((r+i) % ROWS, (c+i) % COLS) == PINE_TREE ||
     dwarf.look((r+i) % ROWS, (c+i) % COLS) == APPLE_TREE)){
  rr = (r + i) % ROWS;
  cc = (c + i) % COLS;
  log << "Walk to " << rr << " " << cc << endl;
  dwarf.start_walk(rr,cc);
  return;
      }     

      if(isNextToATree(dwarf, r, (c+i) % COLS) &&
   !(dwarf.look(r, (c+i) % COLS) == PINE_TREE ||
     dwarf.look(r, (c+i) % COLS) == APPLE_TREE)){
  rr = r;
  cc = (c + i) % COLS;
  log << "Walk to " << rr << " " << cc << endl;
  dwarf.start_walk(rr,cc);
  return;
      }

      if(isNextToATree(dwarf, (r+i) % ROWS, c) &&
   !(dwarf.look((r+i) % ROWS, c) == PINE_TREE ||
     dwarf.look((r+i) % ROWS, c) == APPLE_TREE)){
  rr = (r + i) % ROWS;
  cc = c;
  log << "Walk to " << rr << " " << cc << endl;
  dwarf.start_walk(rr,cc);
  return;
      }
 
    }

    //After all trees are cut down
    /**
       Dwarves will have to start building. The first dwarf with 50 lumber will start a fence
       and all the dwarves will only build fences near that fence. So going to write a function that
       will detect a fence location.
     **/

    if(dwarf.lumber() < 10){
      rr = rand() % ROWS + 30;
      cc = rand() % COLS + 30;
      log << "Moving away" << endl;
      dwarf.start_walk(rr ,cc);
    }

    //start building after reaching 30 wood
    if(dwarf.lumber() > 30 && start){
      log << "BUILDING " << endl;
      log << "Walk to 10 10 " << endl;
      // if(dwarf.look(10,10) == DWARF || dwarf.look(10,10) == PINE_TREE){
      //  dwarf.start_walk(10,11);
      // }else{
      //  dwarf.start_walk(10, 10); 
      // }
      dwarf.start_walk(10,10);
      start = false;
      startB = true;
      return;
    }
   
    if (isNextToAFence(dwarf, r, c) && dwarf.lumber() >= 10) {
      log << "Found a fence -- build" << endl;
      if (dwarf.look(r, c+2) == FENCE){
  dwarf.start_build(EAST);
  lefty = true;
  return;
    }      
    if (dwarf.look(r, c-2) == FENCE){
      dwarf.start_build(WEST);
      righty = true;
      return;
    }     
    if (dwarf.look(r+2, c) == FENCE){
      dwarf.start_build(SOUTH);
      up = true;
      return;
    }      
    if (dwarf.look(r-2, c) == FENCE){
      dwarf.start_build(NORTH);
      down = true;
      return;
    } 
  }
  else {    
    for(int j = 0; j < ROWS; j++){
      if(isNextToAFence(dwarf, (r+j) % ROWS, (c+j) % COLS) &&
   !(dwarf.look((r+j) % ROWS, (c+j) % COLS) == FENCE)){
  rr = (r + j) % ROWS;
  cc = (c + j) % COLS;
  log << "Walk to " << rr << " " << cc << endl;
  dwarf.start_walk(rr,cc);
  return;
      }     

      
      /*
  Find fences and determine the direction of it.
  Need to find fences more efficiently 
  Possibly move all dwarves to center so we have an idea of where to look
  
       */


      if(isNextToAFence(dwarf, r, (c+j) % COLS) &&
         !(dwarf.look(r, (c+j) % COLS) == FENCE)){
        rr = r;
        cc = (c + j) % COLS;
        log << "Walk to " << rr << " " << cc << endl;
        dwarf.start_walk(rr,cc);
        return;
      }

      if(isNextToAFence(dwarf, (r+j) % ROWS, c) &&
         !(dwarf.look((r+j) % ROWS, c) == FENCE)){
        rr = (r + j) % ROWS;
        cc = c;
        log << "Walk to " << rr << " " << cc << endl;
        dwarf.start_walk(rr,cc);
        return;
      }
 
    }

  }
    

    // Otherwise, move to a random location
    rr = rand() % ROWS;
    cc = rand() % COLS;
    log << "RANDOM: Walk to " << rr << " " << cc << endl;
    dwarf.start_walk(rr, cc);
    return;
  }
}