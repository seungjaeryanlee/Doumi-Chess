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
     double duration();
};