// chessboard.cpp
// CS 341: Fall 2023
// Jimmy Patel
//

#include "chessboard.h"
#include <map>
#include <string>

// define containers for color mappings (2 colors) and piece mappings (6 pieces)

using namespace std;

ChessboardIterator Chessboard::begin() {
  // fill out
  return ChessboardIterator(*this, 0);
}

ChessboardIterator Chessboard::end() {
  // fill out
  return ChessboardIterator(*this, 64);
}

// print the chessboard with white at the bottom (traditional view)
void Chessboard::print() {
  // fill out

  for (int y = 0; y < 8; ++y) {
    ChessboardIterator it = begin();

    // Move the iterator to the start of the current row
    for (int x = 0; x < y * 8; ++x) {
      ++it;
    }

    for (int x = 0; x < 8; ++x) {
      Square square = *it;

      // Print the piece symbol based on its color and type
      bool occupied;
      Color color;
      Piece piece;

      square.get(occupied, color, piece);

      std::string symbol =
          " . "; // Initialize with two empty spaces for unoccupied squares

      if (occupied) {
        switch (piece) {
        case Rook:
          symbol = (color == White) ? "bR " : "wR ";
          break;
        case Knight:
          symbol = (color == White) ? "bN " : "wN ";
          break;
        case Bishop:
          symbol = (color == White) ? "bB " : "wB ";
          break;
        case Queen:
          symbol = (color == White) ? "bQ " : "wQ ";
          break;
        case King:
          symbol = (color == White) ? "bK " : "wK ";
          break;
        case Pawn:
          symbol = (color == White) ? "bP " : "wP ";
          break;
        default:
          break;
        }
      }

      // Print the symbol
      cout << symbol;

      // Move to the next square in the same row
      ++it;
    }

    // Start a new line for the next row
    cout << endl;
  }
}