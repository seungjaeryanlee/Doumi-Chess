/******************************************************************************/
/* timer.h                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include <Windows.h>

class Timer {
private:
     LARGE_INTEGER frequency;
     LARGE_INTEGER beginTime;
     LARGE_INTEGER endTime;
     bool hasStarted;
     bool isRunning;
public:
     Timer();
     void start();
     void stop();
     double duration();
};


/// <summary>
/// This function starts a timer with an index.
/// </summary>
/// <param name="beginTime">The pointer that will be given the start time of the timer.</param>
/// <param name="timerIndex">The index of the timer.</param>
/// <returns>The frequency of the timer</returns>
LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex);
/// <summary>
/// This function ends a timer with given index.
/// </summary>
/// <param name="endTime">The pointer that will be given the end time of the timer.</param>
/// <param name="timerIndex">The index of the timer.</param>
void stopTimer(LARGE_INTEGER *endTime, int timerIndex);
/// <summary>
/// This function prints the elapsed time of the timer using time it began, time it ended and the timer's frequency.
/// </summary>
/// <param name="beginTime">The time the timer began running.</param>
/// <param name="endTime">The time the timer stopped.</param>
/// <param name="frequency">The frequency given at startTimer()</param>
/// <param name="timerIndex">The index of the timer.</param>
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);
/// <summary>
/// This function returns the elapsed time of the timer using time it began, time it ended and the timer's frequency.
/// </summary>
/// <param name="beginTime">The time the timer began running.</param>
/// <param name="endTime">The time the timer stopped.</param>
/// <param name="frequency">The frequency given at startTimer()</param>
/// <param name="timerIndex">The index of the timer.</param>
/// <returns>The amount of time the timer ran in milliseconds</returns>
double elapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);