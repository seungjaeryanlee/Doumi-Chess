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
     startTime = std::chrono::high_resolution_clock::now();
}
void Timer::stop() {
     endTime = std::chrono::high_resolution_clock::now();
}
std::chrono::high_resolution_clock::duration Timer::duration_lossless() {
     if (!hasStarted) { return std::chrono::nanoseconds::zero(); }
     if (isRunning) { stop(); }
     return (endTime-startTime);
}
uint64_t Timer::duration_nano() {
     return std::chrono::duration_cast<std::chrono::nanoseconds>(duration_lossless()).count();
}
uint64_t Timer::duration_milli() {
     return std::chrono::duration_cast<std::chrono::milliseconds>(duration_lossless()).count();
}