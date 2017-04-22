/******************************************************************************/
/* timer.cpp                                                                   */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <iostream>
#include "timer.h"

Timer::Timer() {
     hasStarted = false;
     isRunning = false;
}
void Timer::start() {
     hasStarted = true;
     isRunning = true;
     QueryPerformanceFrequency(&frequency);
     QueryPerformanceCounter(&beginTime);
}
void Timer::stop() {
     QueryPerformanceCounter(&endTime);
}
double Timer::duration() {
     if (!hasStarted) { return 0; }
     if (isRunning) { QueryPerformanceCounter(&endTime); }
     return double((endTime.QuadPart - beginTime.QuadPart) * 1000 / frequency.QuadPart);
}

LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex) {
     LARGE_INTEGER frequency;  // ticks per second

     // std::cout << "Timer " << timerIndex << " starting!" << std::endl;

     // get ticks per second
     QueryPerformanceFrequency(&frequency);

     // start timer
     QueryPerformanceCounter(beginTime);

     return frequency;
}
void stopTimer(LARGE_INTEGER *endTime, int timerIndex) {
     QueryPerformanceCounter(endTime);
     // std::cout << "Timer " << timerIndex << " ended!" << std::endl;
}
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex) {
     // in millisecond
     double elapsedTime = (endTime.QuadPart - beginTime.QuadPart) * 1000.0 / frequency.QuadPart;
     std::cout << "Timer " << timerIndex << ": " << elapsedTime << " ms elapsed." << std::endl;
}
double elapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex) {
     // in millisecond
     return (endTime.QuadPart - beginTime.QuadPart) * 1000.0 / frequency.QuadPart;
}
