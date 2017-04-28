/******************************************************************************/
/* pgn.h                                                                      */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include "move.h"
/// <summary>
/// This function saves the current game to a PGN file.
/// </summary>
/// <param name="gameResult">The result of the current game.</param>
/// <param name="savedMove">The moves array given in pointer.</param>
/// <param name="saveIndex">The counter for the savedMove array.</param>
/// <param name="spectate">True if user was spectating. False if not.</param>
/// <param name="userColor">The color user has played in this game.</param>
void savePGN(const result gameResult, const Move *savedMove, const int saveIndex, const bool spectate, const int userColor);