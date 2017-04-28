/******************************************************************************/
/* timer.h                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include <chrono>

class Timer {
private:
     std::chrono::time_point<std::chrono::system_clock> startTime;
     std::chrono::time_point<std::chrono::system_clock> endTime;
     bool hasStarted;
     bool isRunning;
public:
     Timer();

     /// <summary>
     /// This function starts the timer.
     /// </summary>
     void start();
     /// <summary>
     /// This function stops the timer.
     /// </summary>
     void stop();
     /// <summary>
     /// This function returns the elapsed time of the timer.
     /// </summary>
     /// <returns>The amount of time the timer ran in milliseconds</returns>
     std::chrono::high_resolution_clock::duration duration_lossless();

     //FIXME: Documentation
     uint64_t duration_nano();
     uint64_t duration_milli();
};