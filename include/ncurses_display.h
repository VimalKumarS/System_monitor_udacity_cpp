#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);

void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);

// this is for testing and local debugging
//void Display_test(System& system, int n = 10);
//void DisplaySystem_test(System& system);
//void DisplayProcesses_test(std::vector<Process>& processes, int n);

};  // namespace NCursesDisplay

#endif