/******************************************************************************/
/* pgn.cpp                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include "pgn.h"
#include "board.h"
#include <string>
#include <fstream>
#include <iomanip> 

void savePGN(const result gameResult, const Move *savedMove, const int saveIndex, const bool spectate, const int userColor) {
     std::ofstream pgn;
     pgn.open("output.pgn");
     pgn << "[Event \"Friendly Match\"]\n";
     pgn << "[Site \"Princeton, NJ USA\"]\n";
     time_t now = time(0);
     struct tm timeInfo;
     localtime_s(&timeInfo, &now);
     pgn << "[Date \"" << timeInfo.tm_year + 1900 << "." << std::setfill('0')
          << std::setw(2) << timeInfo.tm_mon + 1 << "." << std::setfill('0')
          << std::setw(2) << timeInfo.tm_mday << "\"]\n";
     pgn << "[Round \"1\"]\n";
     if (spectate) {
          pgn << "[White \"Computer\"]\n";
          pgn << "[Black \"Computer\"]\n";
     }
     else if (userColor == WHITE) {
          pgn << "[White \"User\"]\n";
          pgn << "[Black \"Computer\"]\n";
     }
     else {
          pgn << "[White \"Computer\"]\n";
          pgn << "[Black \"User\"]\n";
     }
     switch (gameResult) {
     case BLACK_WIN:
          pgn << "[Result \"0-1\"]\n";
          break;
     case TIE:
          pgn << "[Result \"1/2-1/2\"]\n";
          break;
     case WHITE_WIN:
          pgn << "[Result \"1-0\"]\n";
          break;
     case NOT_FINISHED:
          pgn << "[Result \"*\"]\n";
     }
     for (int i = 0; i < saveIndex; i++) {
          if (i % 2 == 0) {
               pgn << i / 2 + 1 << ". ";
          }
          int initial = savedMove[i].getInitial();
          int terminal = savedMove[i].getTerminal();
          int type = savedMove[i].getType();

          switch (type) {
          case KINGSIDE_CASTLING:
               pgn << "O-O ";
               break;
          case QUEENSIDE_CASTLING:
               pgn << "O-O-O ";
               break;
          case KNIGHT_PROMOTION:
               pgn << numberToFilerank(terminal) << "=N";
               break;
          case BISHOP_PROMOTION:
               pgn << numberToFilerank(terminal) << "=B";
               break;
          case ROOK_PROMOTION:
               pgn << numberToFilerank(terminal) << "=R";
               break;
          case QUEEN_PROMOTION:
               pgn << numberToFilerank(terminal) << "=Q";
               break;
          case ENPASSANT:
               // FIXME: Find correct way to write in PGN format
               pgn << numberToFilerank(initial) << numberToFilerank(terminal) << " ";
               break;
          case DOUBLEMOVE:
               pgn << numberToFilerank(terminal) << " ";
               break;
          case NORMAL:
               // FIXME: Find correct way to write in PGN format
               pgn << numberToFilerank(initial) << numberToFilerank(terminal) << " ";
               break;
          }
          // FIXME: Find correct way to write in PGN format: Check captures
          // FIXME: Find correct way to write in PGN format: Check checks (CHECKCEPTION)
          // FIXME: Find correct way to write in PGN format: Check checkmates
     }

     // Print Game Result
     switch (gameResult) {
     case BLACK_WIN:
          pgn << "0-1\n";
          break;
     case TIE:
          pgn << "1/2-1/2\n";
          break;
     case WHITE_WIN:
          pgn << "1-0\n";
          break;
     case NOT_FINISHED:
          pgn << "*\n";
     }

     pgn.close();

}