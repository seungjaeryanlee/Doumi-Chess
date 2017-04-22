/******************************************************************************/
/* timer.cpp                                                                  */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

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